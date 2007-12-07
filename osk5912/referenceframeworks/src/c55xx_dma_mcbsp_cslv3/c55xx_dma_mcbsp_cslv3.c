/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== c55xx_dma_mcbsp_cslv3.c ========
 *
 *  This driver is based on Chip Support Library (CSL) v3.0
 */

#include <std.h>

#include <stdplus.h>

#include <atm.h>
#include <hwi.h>
#include <que.h>

#include <csl.h>
#include <csl_dma.h>
#include <csl_intc.h>
#include <csl_mcbsp.h>

#include <iom.h>

#include <c55xx_dma_mcbsp_cslv3.h>

#define NUMPORTS    CSL_MCBSP_CNT

#define NUMCHANS     2  /* IOM_INPUT and IOM_OUTPUT */
#define INPUT        0
#define OUTPUT       1

/* Channel Object -- intialized by mdCreateChan() */
typedef struct ChanObj {
    Uns         inUse;          /* TRUE => channel has been opened */

    Uns         dmaId;          /* DMA channel */

    void        *devp;          /* needed for McBSP access */

    CSL_DmaHandle hDma;         /* DMA handle */
    
    IOM_Packet  *dataPacket;    /* current active I/O packet */

    IOM_Packet  *nextPacket;    /* used to submit next packet for chaining */
    QUE_Obj     pendList;       /* list of packets for I/O */

    IOM_TiomCallback cbFxn;     /* used to notify client when I/O complete */
    Ptr         cbArg;
} ChanObj, *ChanHandle;

#define CHANOBJINIT { \
    FALSE,              /* inUse */             \
    0,                  /* dmaId */             \
    NULL,               /* devp */              \
    NULL,               /* hDma */              \
    NULL,               /* dataPacket */        \
    NULL,               /* nextPacket */        \
    { NULL, NULL },     /* pendList */          \
    NULL,               /* cbFxn */             \
    NULL                /* cbArg */             \
}

/* Device Object -- intialized by mdBindDev() */
typedef struct PortObj {
    Uns             inUse;  
    CSL_McbspHandle hMcbsp;     /* McBSP handle */ 
    ChanObj         chans[NUMCHANS];
    Uns             rxIerMask[2];
    Uns             txIerMask[2];
} PortObj, *PortHandle;

#define PORTOBJINIT { \
    FALSE,              /* inUse */             \
    NULL,               /* hMcbsp */            \
    {                   /* chans */             \
        CHANOBJINIT,                            \
        CHANOBJINIT                             \
    },                                          \
    {                                           \
        NULL,                                   \
        NULL                                    \
    },                                          \
    {                                           \
        NULL,                                   \
        NULL                                    \
    }                                           \
}

#if NUMPORTS == 2
static PortObj ports[NUMPORTS] = {              \
    PORTOBJINIT,                                \
    PORTOBJINIT                                 \
};
#elif NUMPORTS == 3
PortObj ports[NUMPORTS] = {                     \
    PORTOBJINIT,                                \
    PORTOBJINIT,                                \
    PORTOBJINIT                                 \
};
#else
#error Number of serials ports undefined!!
#endif


/*
 * Forward declaration of IOM interface functions.
 */
static Int mdBindDev(Ptr *devp, Int devid, Ptr devParams);
static Int mdControlChan(Ptr chanp, Uns cmd, Ptr args);
static Int mdCreateChan(Ptr *chanp, Ptr devp, String name, Int mode,
        Ptr chanParams, IOM_TiomCallback cbFxn, Ptr cbArg);
static Int mdDeleteChan(Ptr chanp);
static Int mdSubmitChan(Ptr chanp, IOM_Packet *packet);

/*
 * Public IOM interface table.
 */
IOM_Fxns C55XX_DMA_MCBSP_CSLV3_FXNS = {
    mdBindDev,
    IOM_UNBINDDEVNOTIMPL,
    mdControlChan,
    mdCreateChan,
    mdDeleteChan,
    mdSubmitChan,
};

/*
 * local functions
 */
static Void configDma(ChanHandle chan, IOM_Packet *packet);
static Void dmaIsr(ChanHandle chan);
static Void abortio(ChanHandle chan);

/*
 *  ======== mdBindDev ========
 */
#pragma CODE_SECTION(mdBindDev, ".text:init")
static Int mdBindDev(Ptr *devp, Int devid, Ptr devParams)
{
    C55XX_DMA_MCBSP_CSLV3_DevParams *params = 
        (C55XX_DMA_MCBSP_CSLV3_DevParams *)devParams;
    PortHandle port = (PortHandle)&ports[devid];
    CSL_Status status;

    if (ATM_setu(&port->inUse, TRUE)) {
        return (IOM_EBADIO);
    }

    /* this driver requires parameters, there are no valid defaults */
    if (params == NULL) {
        return (IOM_EBADARGS);
    }

    /* Check the version number */
    if (params->versionId != C55XX_DMA_MCBSP_CSLV3_VERSION_2){
        /* Unsupported Version */
        return(IOM_EBADARGS);
    }

    /* open the McBSP */
    port->hMcbsp = CSL_mcbspOpen(params->mcbspObj, (CSL_McbspNum)devid, 
        CSL_EXCLUSIVE, NULL, &status);
    
    if (status != CSL_SOK) {
        return (IOM_EBADIO);
    }
    
    /* 
     * Set all McBSP registers to thier RESET values, to ensure
     * no stale register values from previous use/run
     */
    CSL_mcbspHwControl(port->hMcbsp, CSL_MCBSP_CMD_REG_RESET, NULL);
    
    /* Configure the McBSP */
    CSL_mcbspHwSetup(port->hMcbsp, params->mcbspCfg);   

    port->chans[INPUT].dmaId = params->rxDmaId;
    port->chans[OUTPUT].dmaId = params->txDmaId;

    /* store the interrupt masks */
    port->rxIerMask[0] = params->rxIerMask[0];
    port->rxIerMask[1] = params->rxIerMask[1];
    port->txIerMask[0] = params->txIerMask[0];
    port->txIerMask[1] = params->txIerMask[1];
    
    *devp = port;
    
    return (IOM_COMPLETED);
}

/*
 *  ======== mdControlChan ========
 */
static Int mdControlChan(Ptr chanp, Uns cmd, Ptr args)
{
    /*
     * If a channel timeouts(in IOM class driver) a calldown is made to
     * mdControlChan w/ cmd = IOM_CHAN_TIMED out. Timeout processing is
     * optionally implemented here. If not performed return status of
     * IOM_ENOTIMPL.
     */

    /*
     *  Channel timed out. Perform needed channel cleanup.
     */
    if (cmd == IOM_CHAN_TIMEDOUT) {
        abortio(chanp);
    }
    else {
        return (IOM_ENOTIMPL); /* return IOM_ENOTIMPL for codes not handled */
    }

    return (IOM_COMPLETED);
}


/*
 *  ======== mdCreateChan ========
 */
static Int mdCreateChan(Ptr *chanp, Ptr devp, String name, Int mode,
                Ptr chanParams, IOM_TiomCallback cbFxn, Ptr cbArg)
{
    PortHandle                  port = (PortHandle)devp;
    C55XX_DMA_MCBSP_CSLV3_ChanParams    *params =
        (C55XX_DMA_MCBSP_CSLV3_ChanParams *)chanParams;
    HWI_Attrs                   attrs;
    ChanHandle                  chan;
    CSL_IntcObj                 intcObj;
    CSL_IntcHandle              hIntc;
    CSL_IntcEventId             dmaEventId;
    CSL_Status                  status;                 

    chan = (mode == IOM_INPUT) ? &port->chans[INPUT] : &port->chans[OUTPUT];

    /* this driver requires channel parameters, no reasonable default */
    if (params == NULL) {
        return (IOM_EBADARGS);
    }

    /*
     * Check check if channel is already in use.
     * Use ATM_setu() for atomic test-and-set.
     */
    if (ATM_setu((Uns *)&chan->inUse, TRUE)) {
        return (IOM_EBADIO);    /* ERROR! channel is already open! */
    }

    QUE_new(&chan->pendList);

    chan->devp = devp;
    
    chan->dataPacket = NULL; 
    chan->nextPacket = NULL;

    chan->cbFxn = cbFxn;
    chan->cbArg = cbArg;

    /*
     *  open and configure DMA. 
     *  '0' for dmaNum does *not* indicate Channel Num. It merely
     *  indicates '1st instance of a DMA peripheral'
     */       
    chan->hDma = CSL_dmaOpen(params->dmaObj, (CSL_DmaNum)0, 
        (CSL_DmaChaNum)chan->dmaId, CSL_EXCLUSIVE, params->dmaCfg, &status);
    
    if (status != CSL_SOK) {
        return (IOM_EBADIO);
    }

    /* plug interrupt vector */
    attrs.ier0mask = (mode == IOM_INPUT) ? port->rxIerMask[0] : port->txIerMask[0];
    attrs.ier1mask = (mode == IOM_INPUT) ? port->rxIerMask[1] : port->txIerMask[1];
    attrs.arg = (Arg)chan;

    /* Get DMA Event Id */
    CSL_dmaGetChipCtxt(chan->hDma, CSL_DMA_CHIPCTXTQUERY_EVENTID, &dmaEventId);
    
    /* Open the Interrupt Controller to obtain the interrupt EventId for DMA */
    hIntc = CSL_intcOpen(&intcObj, dmaEventId, CSL_INTC_VECTID_DEFAULT, 
        NULL, NULL);
    
    /*  Plug in vector with dispather */
    HWI_dispatchPlug(hIntc->eventId, (Fxn)dmaIsr, &attrs);

    /* Enable the appropriate DMA interrupt */
    CSL_intcEventEnable(hIntc->eventId, NULL);

    *chanp = chan;

    return (IOM_COMPLETED);             /* success */
}

/*
 *  ======== mdDeleteChan ========
 *  Mark the channel available and disable the appropriate interrupt.
 */
static Int mdDeleteChan(Ptr chanp)
{
    ChanHandle                  chan = (ChanHandle)chanp;
    CSL_IntcObj                 intcObj;
    CSL_IntcHandle              hIntc;
    CSL_IntcEventId             dmaEventId;

    /* Get DMA Event Id */
    CSL_dmaGetChipCtxt(chan->hDma, CSL_DMA_CHIPCTXTQUERY_EVENTID, &dmaEventId);
    
    /* Open the Interrupt Controller to obtain the interrupt EventId for DMA */
    hIntc = CSL_intcOpen(&intcObj, dmaEventId, CSL_INTC_VECTID_DEFAULT, 
        NULL, NULL);

    /* Disable the DMA interrupt */
    CSL_intcEventDisable(hIntc->eventId, NULL);

    /* close the DMA channel */
    CSL_dmaClose(chan->hDma);

    chan->inUse = FALSE;

    return (IOM_COMPLETED);
}

/*
 *  ======== mdSubmitChan ========
 */
static Int mdSubmitChan(Ptr chanp, IOM_Packet *packet)
{
    ChanHandle      chan = (ChanHandle)chanp;
    PortHandle      port = (PortHandle)chan->devp;
    Uns             imask;
    CSL_BitMask16   resetCntl;

    if (packet->cmd == IOM_FLUSH || packet->cmd == IOM_ABORT) {

        abortio(chan);

        packet->status = IOM_COMPLETED; /* flush/abort pkt completed */

        return (IOM_COMPLETED);
    }

    imask = HWI_disable();    /* Begin critical section */

    if (chan->dataPacket == NULL) {
        /*
         *  Startup code-path. Must start the DMA *before* starting
         *  the MCBSP Rx, Tx.
         */
        chan->dataPacket = packet;
        configDma(chan, packet);
        if (packet->cmd == IOM_READ) {
            /* start DMA Rx with prev loaded addr, size */
            CSL_dmaHwControl(chan->hDma, CSL_DMA_CMD_START, NULL);
            
            /* enable MCBSP Receive */
            resetCntl = CSL_MCBSP_CTRL_RX_ENABLE;
            CSL_mcbspHwControl(port->hMcbsp, CSL_MCBSP_CMD_RESET_CONTROL, 
               &resetCntl);
        } else {
            
            /* start DMA Tx with prev loaded addr, size */
            CSL_dmaHwControl(chan->hDma, CSL_DMA_CMD_START, NULL);
            
            /* enable MCBSP Transmit */
            resetCntl = CSL_MCBSP_CTRL_TX_ENABLE;
            CSL_mcbspHwControl(port->hMcbsp, CSL_MCBSP_CMD_RESET_CONTROL, 
                &resetCntl);
        }
    } else if (chan->nextPacket == NULL) {
        /*
         *  Standard runtime scenario. Set up addresses, sizes for the 
         *  next DMA Rx or Tx job
         */ 
        chan->nextPacket = packet;
        configDma(chan, packet); 
    } else {

        /*
         *  Standard runtime scenario. Enqueue the IOM data packet.
         *  QUE_enqueue used instead of QUE_put for efficiency since 
         *  interrupts are disabled
         */
         QUE_enqueue(&chan->pendList, packet);
    }

    HWI_restore(imask);    /* End critical section */

    return (IOM_PENDING);
}

/*
 *  ======== C55XX_DMA_MCBSP_CSLV3_init ========
 */
#pragma CODE_SECTION(C55XX_DMA_MCBSP_CSLV3_init, ".text:init")
Void C55XX_DMA_MCBSP_CSLV3_init(Void)
{
    /* Call all CSL module init functions */
    CSL_mcbspInit();
    CSL_dmaInit();
    CSL_intcInit();

}

/*
 *  ======== configDma ========
 */
static Void configDma(ChanHandle chan, IOM_Packet *packet)
{   
    /*
     *  Set the address and packet size for the appropriate DMA channel
     *  and start transfer.
     */ 
    if (packet->cmd == IOM_READ) {

        /* Change the packet address */
        CSL_dmaHwControl(chan->hDma, CSL_DMA_CMD_CHNG_DST, &packet->addr);
        
        /* Modify size of packet */
        CSL_dmaHwControl(chan->hDma, CSL_DMA_CMD_CHNG_ELEMENT_COUNT, 
            &packet->size);

        /* Set ENDPROG to indicate the end of register programming */
        CSL_FINS(chan->hDma->regs->CCR, DMA_CCR_ENDPROG, 1);

    }
    else {     /* IOM_WRITE command */

        /* Change the packet address */
        CSL_dmaHwControl(chan->hDma, CSL_DMA_CMD_CHNG_SRC, &packet->addr);
          
        /* Modify size of packet */
        CSL_dmaHwControl(chan->hDma, CSL_DMA_CMD_CHNG_ELEMENT_COUNT, 
            &packet->size);

        /* Set ENDPROG to indicate the end of register programming */
        CSL_FINS(chan->hDma->regs->CCR, DMA_CCR_ENDPROG, 1);
    }
}


/*
 *  ======== dmaIsr ========
 */
Void dmaIsr(ChanHandle chan)
{    
    IOM_Packet *packet = chan->dataPacket; // Hold on to packet for callback
    volatile Uint16 temp;                  // To read DMA CSR for new interrupts
   
    /* 
     *  Need to clear the C55 DMA channel's CSR bits to ensure we continue
     *  to get DMA completion interrupts. This is required since we're using 
     *  Autoinit mode hence not starting DMA on each frame (which itself
     *  reads the CSR). Hence we need to do read the DMA CSR ourselves.
     */
    temp = chan->hDma->regs->CSR;

    if (packet == NULL) {
        /*  return if spurious interrupt */
        return;
    }
    packet->status = IOM_COMPLETED;

    /*
     *  Done with current packet. Prepare for next job by 
     *  setting dataPacket to nextPacket.
     *  Then get the next one off the QUE list.
     */
    chan->dataPacket = chan->nextPacket;
    chan->nextPacket = QUE_get(&chan->pendList);
    
    /*  Check to see if the QUE is empty */
    if (chan->nextPacket == (IOM_Packet *)&chan->pendList) {
        chan->nextPacket = NULL;
    }
    else {
        configDma(chan, chan->nextPacket);
    }
    
    (*chan->cbFxn)(chan->cbArg, packet);
}

/*
 *  ======== abortio ========
 *  Aborts uncompleted i/o packet requests.
 */
static Void abortio(ChanHandle chan)
{
    IOM_Packet *tmpPacket;

    /* Stop the DMA channel first */
    CSL_dmaHwControl(chan->hDma, CSL_DMA_CMD_STOP, NULL);

    /* 
     * No need to disable interrupts around the shared variable structures
     * since DMA has been stopped
     */

    /*  Process the dataPacket if not null */
    if (chan->dataPacket != NULL) {
        chan->dataPacket->status = IOM_ABORTED;
        (*chan->cbFxn)(chan->cbArg, chan->dataPacket);
        chan->dataPacket = NULL;
    }

    /* process the nextPacket if not null */
    if (chan->nextPacket != NULL) {
        chan->nextPacket->status = IOM_ABORTED;
        (*chan->cbFxn)(chan->cbArg, chan->nextPacket);
        chan->nextPacket = NULL;
    }

    /*  Process all the remaining packets on the channel's pendList */
    tmpPacket = QUE_get(&chan->pendList);
    while (tmpPacket != (IOM_Packet *)&chan->pendList) {
        tmpPacket->status = IOM_ABORTED;   /* abort queued requests */
        (*chan->cbFxn)(chan->cbArg, tmpPacket);

        tmpPacket = QUE_get(&chan->pendList);
    }
}


