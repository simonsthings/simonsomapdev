/** @mainpage DMA CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for the
 * DMA module across various devices. The CSL developer is expected to refer to
 * this document while designing APIs for these modules. Some of the listed APIs
 * may not be applicable to a given DMA module. While in other cases this list
 * of APIs may not be sufficient to cover all the features of a particular DMA
 * Module. The CSL developer should use his discretion in designing new APIs or
 * extending the existing ones to cover these.
 *
 * @subsection aaa Terms and Abbreviations
 *   -# CSL:  Chip Support Library
 *   -# API:  Application Programmer Interface
 *
 * @subsection References
 *    -# CSL-001-DES, CSL 3.x Design Specification DocumentVersion 1.03
 *    -# MGS3_401, Megastar3 Generic DMA Functional Specification, Version 1.1
 *
 */

/** @file csl_dpll.h
 *
 *  @brief    Header file for functional layer of CSL
 *
 *  Description
 *    - The different enumerations, structure definitions
 *      and function declarations
 *
 *  Modification 2
 *    - modified on: 30/03/2004
 *    - reason: Doxygenation of the comments to enable document generation
 *
 *  Modification 1
 *    - modified on: 29/03/2004
 *    - reason: Created the initail file from OMAP5905 CSL release on 7th Oct 2003
 *
 *  @date 29th March, 2004
 *  @author Prakash V. Gudnavar.
 */

#ifndef _CSL_DMA_H_
#define _CSL_DMA_H_

#include <csl_error.h>
#include <csl_types.h>
#include <cslr_dma.h>

/**************************************************************************\
* DMA global typedef declarations
\**************************************************************************/

/** @brief Enumerations for Burst enable/disable     
 */
typedef enum {
    /** Burst disabled */
    CSL_DMA_BURSTEN_DISABLE    =              0,
    /** Burst enabled */
    CSL_DMA_BURSTEN_ENABLE     =              2
} CSL_DmaBurstEn;

/** @brief Enumerations for Type of port 
 * 
 * This enumeration is used to select source/destination port.
 */
typedef enum {
    /** SARAM port */
    CSL_DMA_PORT_SARAM         =              0,
    /** DARAM port */
    CSL_DMA_PORT_DARAM         =              1,
    /** EMIF port */
    CSL_DMA_PORT_EMIF          =              2,
    /** RHEA port */
    CSL_DMA_PORT_RHEA          =              3,
    /** API port */
    CSL_DMA_PORT_API           =              4
} CSL_DmaPort;

/** @brief Enumerations for Addressing mode 
 * 
 * This enumeration is used to select addressing mode.
 */
typedef enum {
    /** Constant addressing mode */
    CSL_DMA_AMODE_CONST        =              0,
    /** Post increment addressing mode */
    CSL_DMA_AMODE_POSTINC      =              1,
    /** Single indexed addressing mode */
    CSL_DMA_AMODE_SNGLIND      =              2,
    /** Double indexed addressing mode */
    CSL_DMA_AMODE_DBLIND       =              3
} CSL_DmaAmode;

/** @brief Enumerations for Element size 
 * 
 * This enumeration is used to select the element size.
 */
typedef enum {
    /** Element size 8 bits */
    CSL_DMA_DATASIZE_8BIT      =              0,
    /** Element size 16 bits */
    CSL_DMA_DATASIZE_16BIT     =              1,
    /** Element size 32 bits */
    CSL_DMA_DATASIZE_32BIT     =              2
} CSL_DmaDataSize;

/** @brief Enumerations for Priority of channel 
 * 
 * This enumeration is used to select the priority of transfer.
 */
typedef enum {
    /** Low priority transfer */
    CSL_DMA_PRIORITY_LO        =              0,
    /** High priority transfer */
    CSL_DMA_PRIORITY_HI        =              1
} CSL_DmaPriority;

/** @brief Enumerations for Hardware synchronization 
 * 
 * This enumeration is used to select the hardware synchronization for data 
 * transfers.
 */
typedef enum {
    /** Channel not synchronized on request */
    CSL_DMA_SYNC_NONE           =       0,
    /** Channel synchronized on MCSI_1 Transmit */
    CSL_DMA_SYNC_MCSI1TX        =       1,
    /** Channel synchronized on MCSI_1 Receive */
    CSL_DMA_SYNC_MCSI1RX        =       2,
    /** Channel synchronized on MCSI_2 Transmit */
    CSL_DMA_SYNC_MCSI2TX        =       3,
    /** Channel synchronized on MCSI_2 Receive */
    CSL_DMA_SYNC_MCSI2RX        =       4,
    /** Channel synchronized on external DMA request zero */
    CSL_DMA_SYNC_MPUIO2         =       5,
    /** Channel synchronized on external DMA request one */
    CSL_DMA_SYNC_MPUIO4         =       6,
    /** Channel synchronized on McBSP1 Transmit */
    CSL_DMA_SYNC_MCBSP1TX       =       8,
    /** Channel synchronized on McBSP1 Receive */
    CSL_DMA_SYNC_MCBSP1RX       =       9,
    /** Channel synchronized on McBSP1 Transmit */
    CSL_DMA_SYNC_MCBSP3TX       =       10,
    /** Channel synchronized on McBSP1 Receive */
    CSL_DMA_SYNC_MCBSP3RX       =       11,
    /** Channel synchronized on UART1 Transmit */
    CSL_DMA_SYNC_UART1TX        =       12,
    /** Channel synchronized on UART1 Recieve */
    CSL_DMA_SYNC_UART1RX        =       13,
    /** Channel synchronized on UART2 Transmit */
    CSL_DMA_SYNC_UART2TX        =       14,
    /** Channel synchronized on UART2 Recieve */
    CSL_DMA_SYNC_UART2RX        =       15,
    /** Channel synchronized on UART3 Transmit */
    CSL_DMA_SYNC_UART3TX        =       18,
    /** Channel synchronized on UART3 Recieve */
    CSL_DMA_SYNC_UART3RX        =       19
} CSL_DmaSync;

/** @brief Enumerations for Frame Synchronization 
 * 
 * This enumeration is used to select the synchronization for transfer.
 */
typedef enum {
    /** Element synchronized transfer */
    CSL_DMA_FRAMESYNC_DISABLE   =             0,
    /** Frame synchronized transfer */
    CSL_DMA_FRAMESYNC_ENABLE    =             1
} CSL_DmaFrameSync;

/** @brief Enumerations for type having values ON and OFF
 *    
 * This enumeration is used to select the state of certain parameters of 
 * channel that have two states ON and OFF. 
 */
typedef enum {
    /** On - Some property is to turned ON */
    CSL_DMA_ONOFF_OFF           =             0,
    /** Off - Some property is to turned OFF */
    CSL_DMA_ONOFF_ON            =             1
} CSL_DmaOnOff;

/** @brief Enumerations for DMA operations       
 * 
 * This enumeration is used to select the control command that has to be 
 * passed to the CSL_dmaHwControl function.
 */
typedef enum {
    /** Start data transfer (argument type: NULL ) */
    CSL_DMA_CMD_START               =             0,
    /** Stop data transfer (argument type: NULL ) */
    CSL_DMA_CMD_STOP                =             1,
    /** Change source address (argument type: Uint32 * ) */
    CSL_DMA_CMD_CHNG_SRC            =             2,
    /** Change destination address (argument type: Uint32 * ) */
    CSL_DMA_CMD_CHNG_DST            =             3,
    /** Change element count (argument type: Uint16 * ) */
    CSL_DMA_CMD_CHNG_ELEMENT_COUNT  =             4,
    /** Change frame count (argument type: Uint16 * ) */
    CSL_DMA_CMD_CHNG_FRAME_COUNT    =             5,
    /** Clear Channel Status Register (argument type: NULL * ) */
    CSL_DMA_CMD_CLEAR_CHANNEL_STATUS =            6,  
    /** Notify h/w the end of register programming (argument type: NULL * ) */
    CSL_DMA_CMD_NOTIFY_ENDPROG      =             7  
} CSL_DmaHwControlCmd;

/** @brief Enumerations for index mode 
 * 
 * This enumeration is used to select the indexing mode to be same or 
 * different for source and destination.
 */
typedef enum {
    /** Source and destination index modes are same */
    CSL_DMA_INDEXMODE_SAME          =             0,
    /** Source and destination index modes are different */
    CSL_DMA_INDEXMODE_DIFF          =             1
} CSL_DmaIndexMode;

/** @brief Enumerations for h/w status query 
 * 
 * This enumeration is used to select the hardware status query that has to be
 * passed to the CSL_dmaGetHwStatus function.
 */
typedef enum {
    /** Get status of DMA channel */
    CSL_DMA_QUERY_STATUS            =             0
} CSL_DmaHwStatusQuery;

/** @brief Enumerations for chip context query 
 *
 * This enumeration is used to select the chip context query that has to be 
 * passed to the CSL_dmaGetChipCtxt function.
 */
typedef enum {
    /** Get event ID for a DMA channel */
    CSL_DMA_CHIPCTXTQUERY_EVENTID       =             0
} CSL_DmaChipCtxtQuery;

/** Defines reset values of CSL_DmaHwSetupSrcPort structure members. */
#define CSL_DMA_DEFAULTS_SRCPORT  { \
    (CSL_DmaBurstEn)CSL_DMA_CSDP_SRCBEN_RESETVAL, \
    (CSL_DmaOnOff)CSL_DMA_CSDP_SRCPACK_RESETVAL, \
    (CSL_DmaPort)CSL_DMA_CSDP_SRC_RESETVAL, \
    (CSL_DmaAmode)CSL_DMA_CCR_SRCAMODE_RESETVAL, \
    ((((Uint32)CSL_DMA_CSSAU_CSSAU_RESETVAL >> 1) << 16) + (((Uint32)CSL_DMA_CSSAL_CSSAL_RESETVAL >> 1) & 0xffff)), \
    (Uint16)CSL_DMA_CSFI_CSFI_RESETVAL, \
    (Uint16)CSL_DMA_CSEI_CSEI_RESETVAL \
    }

/** Defines reset values of CSL_DmaHwSetupDstPort structure members. */
#define CSL_DMA_DEFAULTS_DSTPORT  { \
    (CSL_DmaBurstEn)CSL_DMA_CSDP_DSTBEN_RESETVAL, \
    (CSL_DmaOnOff)CSL_DMA_CSDP_DSTPACK_RESETVAL, \
    (CSL_DmaPort)CSL_DMA_CSDP_DST_RESETVAL, \
    (CSL_DmaAmode)CSL_DMA_CCR_DSTAMODE_RESETVAL, \
    ((((Uint32)CSL_DMA_CDSAU_CDSAU_RESETVAL >> 1) << 16) + (((Uint32)CSL_DMA_CDSAL_CDSAL_RESETVAL >> 1) & 0xffff)), \
    (Uint16)CSL_DMA_CDFI_CDFI_RESETVAL, \
    (Uint16)CSL_DMA_CDEI_CDEI_RESETVAL \
    }

/** Defines reset values of CSL_DmaHwSetupChannel structure members. */
#define CSL_DMA_DEFAULTS_CHANNEL  { \
    (CSL_DmaDataSize)CSL_DMA_CSDP_DATATYPE_RESETVAL, \
    (Uint16)CSL_DMA_CEN_CEN_RESETVAL, \
    (Uint16)CSL_DMA_CFN_CFN_RESETVAL, \
    (CSL_DmaPriority)CSL_DMA_CCR_PRIO_RESETVAL, \
    (CSL_DmaSync)CSL_DMA_CCR_SYNC_RESETVAL, \
    (CSL_DmaFrameSync)CSL_DMA_CCR_FS_RESETVAL, \
    (CSL_DmaOnOff)CSL_DMA_CCR_ENDPROG_RESETVAL, \
    (CSL_DmaOnOff)CSL_DMA_CCR_REPEAT_RESETVAL, \
    (CSL_DmaOnOff)CSL_DMA_CCR_AUTOINIT_RESETVAL \
    } 

/** Defines reset values of CSL_DmaHwSetupIntr structure members. */    
#define CSL_DMA_DEFAULTS_INTR  { \
    (CSL_DmaOnOff)CSL_DMA_CICR_BLOCKIE_RESETVAL, \
    (CSL_DmaOnOff)CSL_DMA_CICR_LASTIE_RESETVAL, \
    (CSL_DmaOnOff)CSL_DMA_CICR_FRAMEIE_RESETVAL, \
    (CSL_DmaOnOff)CSL_DMA_CICR_FIRSTHALFIE_RESETVAL, \
    (CSL_DmaOnOff)CSL_DMA_CICR_DROPIE_RESETVAL, \
    (CSL_DmaOnOff)CSL_DMA_CICR_TIMEOUTIE_RESETVAL \
    } 
/** Defines reset values of CSL_DmaHwSetupGlobal structure members. */
#define CSL_DMA_DEFAULTS_GLOBAL  { \
    (CSL_DmaOnOff)CSL_DMA_GCR_AUTOGATINGON_RESETVAL, \
    (Bool)CSL_DMA_GCR_FREE_RESETVAL, \
    (Bool)CSL_DMA_GCR_APIEXCL_RESETVAL, \
    (CSL_DmaPriority)CSL_DMA_GCR_APIPRIO_RESETVAL, \
    (Bool)CSL_DMA_GTCR_SARAMTCE_RESETVAL, \
    (Bool)CSL_DMA_GTCR_DARAMTCE_RESETVAL, \
    (CSL_DmaIndexMode)CSL_DMA_GSCR_INDEXMODE_RESETVAL \
    }

/** @brief Structure for setup of Source port 
 *
 * This is of type structure CSL_DmaSetupPort explained elsewhere. 
 */
typedef struct CSL_DmaHwSetupPort CSL_DmaHwSetupSrcPort;

/** @brief Structure for setup of Destination port
 *
 * This is of type structure CSL_DmaSetupPort explained elsewhere. 
 */
typedef struct CSL_DmaHwSetupPort CSL_DmaHwSetupDstPort;

/** @brief Structure that is used to setup Source/Destination port 
 *
 * This structure is part of the CSL_DmaHwSetup structure, and contains 
 * parameters for setting source or destination ports. This is passed as 
 * a parmeter to the CSL_dmaHwSetup function. Also the structure is used 
 * to collect the global setup information from the CSL_dmaGetHwStatus 
 * function.
 */
typedef struct CSL_DmaHwSetupPort {
    /** Burst enable */
    CSL_DmaBurstEn  burstEn;
    /** Packing */
    CSL_DmaOnOff  packing;
    /** Port */
    CSL_DmaPort  port;
    /** Addressing mode */
    CSL_DmaAmode  amode;
    /** Start Address (word address) */
    Uint32  staddr;
    /** Frame index */
    Int16   frmIndex;
    /** Element index */
    Int16   elmIndex;
} CSL_DmaHwSetupPort;

/** @brief Structure that is used to setup DMA channel
 *
 * This structure captures setup parameters corresponding to a channel. This 
 * structure is part of the CSL_DmaHwSetup structure, and is passed as a 
 * member of the CSL_DmaHwSetup parameter to the CSL_dmaHwSetup function. 
 * Also the structure is used to collect the global setup information from 
 * the CSL_dmaGetHwStatus function.
 */
typedef struct CSL_DmaHwSetupChannel {
    /** Data size for transfer(8/16/32 bit) */
    CSL_DmaDataSize datatype;
    /** Element Count */
    Uint16  elmCnt;
    /** Frame count */
    Uint16  frmCnt;
    /** Channel Priority */
    CSL_DmaPriority     prio;
    /** Synchronization Control */
    CSL_DmaSync sync;
    /** Frame synchronization */
    CSL_DmaFrameSync    fs;
    /** End of Programming Status Bit */
    CSL_DmaOnOff    endprog;
    /** Repetitive Operation */
    CSL_DmaOnOff    repeat;
    /** Auto-initialization */
    CSL_DmaOnOff    autoinit;
} CSL_DmaHwSetupChannel;

/** @brief Structure that is used to configure interrupts
 *
 * This structure captures info on what interrupts need to be enabled. This 
 * structure is part of the CSL_DmaHwSetup structure, and is passed as a 
 * member of the CSL_DmaHwSetup to the CSL_dmaHwSetup function. Also the 
 * structure is used to collect the global setup information from the 
 * CSL_dmaGetHwStatus function.
 */
typedef struct CSL_DmaHwSetupIntr {
    /** End-of-block Interrupt Enable */
    CSL_DmaOnOff    blockie;
    /** Last Interrupt Enable */
    CSL_DmaOnOff    lastie;
    /** Frame Interrupt Enable */
    CSL_DmaOnOff    frameie;
    /** Half Interrupt Enable */
    CSL_DmaOnOff    firsthalfie;
    /** Drop Interrupt Enable */
    CSL_DmaOnOff    dropie;
    /** Timeout Interrupt Enable */
    CSL_DmaOnOff    timeoutie;
} CSL_DmaHwSetupIntr;

/** @brief Structure that is used to setup global settings of DMA 
 *
 * This structure is part of the CSL_DmaHwSetup structure, and is passed as a 
 * member of the CSL_DmaHwSetup to the CSL_dmaHwSetup OR CSL_dmaOpen function.
 * Also the structure is used to collect the global setup information from 
 * the CSL_dmaGetHwStatus function.
 */
typedef struct CSL_DmaHwSetupGlobal {
    /** DMA clocks auto gating enable */
    CSL_DmaOnOff    autogating;
    /** DMA reaction to the suspend signal */
    Bool    free;
    /** Exclusive priority for API channel */
    Bool    apiexcl;
    /** Priority for API channel */
    CSL_DmaPriority apiprio;
    /** SARAM timeout counter enable */
    Bool    stce;
    /** DARAM timeout counter enable */
    Bool    dtce;
    /** Source/Destination Index Mode Control(Same or Different) */
    CSL_DmaIndexMode    indexMode;
} CSL_DmaHwSetupGlobal;

/** @brief Structure that is used to setup DMA 
 *
 * This structure is used to pass setup parameters to the CSL_dmaHwSetup 
 * function, which in turn writes to various registers of the DMA hardware 
 * to configure the peripheral for a particular data transfer. 
 */
typedef struct CSL_DmaHwSetup {
    /** Pointer to the source port setup structure */
    CSL_DmaHwSetupSrcPort   *srcStruct;
    /** Pointer to the destination port setup structure */
    CSL_DmaHwSetupDstPort   *dstStruct;
    /** Pointer to the channel setup structure */
    CSL_DmaHwSetupChannel   *channelStruct;
    /** Pointer to the interrupt setup structure */
    CSL_DmaHwSetupIntr      *intrStruct;
    /** Pointer to the global setup structure */
    CSL_DmaHwSetupGlobal    *globalStruct;
} CSL_DmaHwSetup;

/** @brief Structure that is used to get status of DMA 
 *
 * The CSL_DmaStatus structure is used to get status of DMA. This structure 
 * is passed as an argument to CSL_dmaGetHwStatus function when the query 
 * used is CSL_DMA_QUERY_STATUS
 */
typedef struct CSL_DmaStatus {
    /** Timeout occured? */
    Bool        timeout;
    /** Event dropped during transfer ? */
    Bool        eventDrop;
    /** Has first half of current frame been transferred? */
    Bool        halfframe;
    /** Has a complete frame been transferred? */
    Bool        frame;
    /** Has transfer of last frame started? */
    Bool        lastframe;
    /** Has current transfer in the channel been finished? (another may have started if DMA_CCR */
    Bool        block;
    /** Is channel waiting for synchronized GDMA request to be scheduled? */
    Bool        sync;
    /** Source address counter */
    Uint16      srcAddrCntr;
    /** Destination address counter */
    Uint16      dstAddrCntr;
} CSL_DmaStatus;

/** @brief This object contains the reference to the instance of DMA
 * opened using the @a CSL_dmaOpen()
 *
 * The pointer to this, (typedefed as handle), is passed to all DMA CSL APIs. */
typedef struct CSL_DmaObj {
    /** This is the mode in which the CSL instance is opened */
    CSL_OpenMode            openMode;
    /** This is a unique identifier to the instance of DMA being
     * referred to by this object */
    CSL_Uid                 uid;
    /** This is the variable that contains the current state of a
     * resource being shared by current instance of DMA with
     * other peripherals */
    CSL_Xio                 xio;
    /** This is a pointer to the global registers of the instance of DMA 
     * referred to by this object */
    CSL_DmaGlobalRegsOvly   gRegs;
    /** This is the instance of DMA being referred to by this object */
    CSL_DmaNum              dmaNum;
    /** This is a pointer to the channel registers of the instance of DMA channel
     * referred to by this object */
    CSL_DmaChaRegsOvly      regs;
    /** This is the instance of DMA channel being referred to by this object */
    CSL_DmaChaNum           chaNum;
} CSL_DmaObj;

typedef struct CSL_DmaObj *CSL_DmaHandle;

/**************************************************************************\
* DMA global function declarations
\**************************************************************************/
/**************************************************************************\
* DMA global function declarations
\**************************************************************************/

/** 
 * This is DMA module's init function. Currently it does nothing.
 * Needs to be called before using DMA module.
 *
 * <b> Usage Constraints: </b>
 * @a CSL_sysInit() should have been called successfully.
 * 
 * @return Returns the status of the operation
 * CSL_SOK, if success   
 *
 */

CSL_Status CSL_dmaInit(
    void
);

/**
 *  The open call sets up the data structures for the particular
 *  instance of DMA channel. The channel can be re-opened anytime
 *  after it has been normally closed if so required. DMA channel Hardware
 *  setup will be performed at the end of the open call only if the
 *  HwSetup Pointer supplied was non-NULL. The handle returned by
 *  this call is input as an essential argument for rest of the APIs
 *  described for this module.
 *
 *      <b> Usage Constraints:</b>
 *  The CSL system as well as DMA must be successfully initialized via
 *  CSL_sysInit() and CSL_dmaInit() before calling this function. Memory
 *  for the @a CSL_DmaObj must be allocated outside this call. This object
 *  must be retained while usage of this peripheral.
 *
 * @b Example:
 * @verbatim


        CSL_DmaObj     dmaObj;
        CSL_DmaHwSetup dmaSetup;
        CSL_Status       status;
        ...
        hdma = CSL_dmaOpen( &dmaObj,
                        CSL_DMA_0,
                        CSL_DMA_CHA_0,
                        CSL_EXCLUSIVE,
                        &dmaSetup,
                        &status
                      );
   @endverbatim
 * @return Returns a handle @a CSL_dmaHandle to the requested instance of
 * DMA channel if the call is successful, otherwise, a @a NULL is returned.
 * Please check for the appropriate status condition. Status field will 
 * contain:
 *      CSL_SOK  - if open call is success.
 *      CSL_ESYS_INUSE - Requested channel or all(if any DMA, any channel is 
 *  requested) the DMA channel are currently in use.
 *
 */
 CSL_DmaHandle  CSL_dmaOpen(
    /** Pointer to the object that holds reference to the
     *  instance of DMA requested after the call
         */
    CSL_DmaObj            *dmaObj,
    /** Instance of DMA to which a handle is requested
         */
    CSL_DmaNum            dmaNum,
    /** Instance of DMA channel to which a handle is requested
         */
    CSL_DmaChaNum         chaNum,
    /** Specifies if dma should be opened with exclusive or
     *  share access to the associate pins
         */
    CSL_OpenMode          openMode,
    /** If a valid structure is passed (not @a NULL), then
     *  the @a CSL_dmaHwSetup() is called with this parameter
         */
    CSL_DmaHwSetup        *hwSetup,
    /** This returns the status (success/errors) of the call
         */
    CSL_Status            *status
);

/**
 *  The Close call releases the resource and appropriate shared pins.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_dmaInit() and @a CSL_dmaOpen() must be called successfully
 *  in that order before CSL_dmaClose() can be called.
 *
 *  @b Example:
 *  @verbatim

      CSL_DmaHandle hDma;
      ...
      CSL_dmaClose(hDma);
    @endverbatim
 *
 * @return returns the status of the operation (see @a CSL_Status)
 * Status is:
 *              CSL_SOK, if close function succeeds.
 *              CSL_ESYS_BADHANDLE, if the handle is not valid. 
 *
 */
 CSL_Status  CSL_dmaClose(
    /** Pointer to the object that holds reference to the
     *  instance of DMA channel requested after the CSL_dmaOpen(...) call
         */
    CSL_DmaHandle         hDma
);

/**
 * This function initializes the device registers with
 * the appropriate values provided through the HwSetup Data structure.
 * This function needs to be called only if the HwSetup Structure was
 * not previously passed through the Open call.
 * For information passed through the HwSetup Data structure refer
 * @a CSL_DmaHwSetup.
 *
 * <b> Usage Constraints:</b>
 * Both @a CSL_dmaInit() and @a CSL_dmaOpen() must be called successfully
 * in that order before CSL_dmaHwSetup() can be called. The user has to
 * allocate space for & fill in the main setup structure appropriately before
 * calling this function
 *
 * @b Example:
 * @verbatim
     CSL_DmaHandle hDma;
     CSL_Status status;
     CSL_DmaHwSetup  setup;
         CSL_DmaHwSetupGlobal  globalSetup = {…};
         CSL_DmaHwSetupSrcPort srcSetup = {…};
         CSL_DmaHwSetupDstPort dstSetup = {…};
         CSL_DmaHwSetupChannel channelSetup = {…} ;
         CSL_DmaHwSetupIntr intrSetup = {…};
         setup.srcStruct  = &srcSetup ;
         setup.dstStruct  = &dstSetup ;
         setup.channelStruct  = &channelSetup ;
         setup.globalStruct  = &globalSetup ;
     status = CSL_dmaHwSetup (hDma, &hwSetup);
   @endverbatim
 *
 * @return Returns the status of the setup operation (see @a CSL_Status)
 * Status is:
 * CSL_SOK - successful completion of the setup
 * CSL_ESYS_INVPARAMS - hwSetup structure is not initialized.
 */
 CSL_Status  CSL_dmaHwSetup(
    /** Pointer to the object that holds reference to the
     *  instance of DMA channel requested after the call
         */
    CSL_DmaHandle         hDma,
    /** Pointer to setup structure which contains the
     *  information to program DMA to a useful state
         */
    CSL_DmaHwSetup        *setup
);

/** Gets the current hardware setup of DMA. The obtaining of setup through 
 * this API is the reverse operation of @a CSL_dmaHwSetup() function.
 * 
 * <b> Usage Constraints: </b>
 * Both @a CSL_dmaInit() and @a CSL_dmaOpen() must be called successfully
 * in that order before @a CSL_dmaGetHwSetup() can be called. The argument
 * is a setup structure which consists of pointers to sub-structures & so on.
 * Only those sub-structures which are required should be populated. The other
 * pointers must be set to NULL.
 * 
 * @b Example:
 * @verbatim

 
    CSL_DmaHandle hDma;
    CSL_DmaHwSetup  setup;
        CSL_DmaHwSetupGlobal  globalSetup ;
        CSL_DmaHwSetupSrcPort srcSetup ;
        CSL_DmaHwSetupDstPort dstSetup ;
        CSL_DmaHwSetupChannel channelSetup '
        CSL_DmaHwSetupIntr intrSetup ;
        
        setup.srcStruct  = &srcSetup ;
        setup.dstStruct  = &dstSetup ;
        setup.channelStruct  = &channelSetup ;
        setup.globalStruct  = &globalSetup ;
        setup.intrStruct = NULL ;
    ...
    ...
        CSL_dmaGetHwSetup(hDma, &hwSetup);
   @endverbatim
 * 
 * @return returns the status of the operation
 * 
 */
CSL_Status  CSL_dmaGetHwSetup(
    /** pointer to the object that holds reference to the
     * instance of DMA requested after the call */
    CSL_DmaHandle                         hDma,
    /** the query to this API which indicates the status/setup
     * to be returned */
    CSL_DmaHwSetup                        *setup
);
 
/**
 *  Control operations for the DMA channel. For a particular control
 *  operation, the pointer to the corresponding data type needs
 *  to be passed as argument in HwControl function Call. All the
 *  arguments (Structure elements included) passed to  the HwControl
 *  function are inputs. For the list of commands supported and argument
 *  type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_DmaHwControlCmd
 *
 *      <b> Usage Constraints:</b>
 *  Both @a CSL_dmaInit() and @a CSL_dmaOpen() must be called successfully
 *  in that order before CSL_dmaHwControl() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_DmaHwControlCmd
 *
 *  @b Example:
 *  @verbatim
    1. Code snippet to start data transfer:
        CSL_DmaHandle hDma;
        CSL_Status status;
        ...
        status = CSL_dmaHwControl(hDma, CSL_DMA_CMD_START, NULL);
      
        2. Code snippet to change source address:
            CSL_Uint32 srcAddr2 = 0x3017;
            status = CSL_dmaHwControl(hDma, CSL_DMA_CMD_CHNG_SRC, &srcAddr2);
                                   
    @endverbatim
 *
 * @return Returns the status of the operation (see @a CSL_Status)
 * Status is:
 *      CSL_SOK - successful completion of the operation.
 *  CSL_ESYS_INVCMD - command passed is not a valid control command for DMA
 *
 */

 CSL_Status  CSL_dmaHwControl(
    /** Pointer to the object that holds reference to the
     *  instance of DMA channel requested after the call
         */
    CSL_DmaHandle     hDma,
    /** The command to this API, which indicates the action to be taken
         */
    CSL_DmaHwControlCmd   cmd,
    /** An optional argument @a void* casted
         */
    void          *voidPtr
);

/**
 *  This function is used to read the current device configuration
 *  and the value present in associated registers. User should allocate
 *  memory for the said data type and pass its pointer as an unadorned
 *  void* argument to the status query call. For details about the various
 *  status queries supported and the associated data structure to record the
 *  response, refer to @a CSL_DmaHwStatusQuery.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_dmaInit() and @a CSL_dmaOpen() must be called successfully
 *  in that order before @a CSL_dmaGetHwStatus() can be called. For the
 *  argument type that can be void* casted & passed with a particular command,
 *  refer to @a CSL_DmaHwStatusQuery
 *
 *  @b Example:
 *  @verbatim
     Code snippet to get DMA status 
      
     CSL_DmaHandle hDma;
     CSL_Status status;
     CSL_DmaStatus dmaStatus ;
     ...
     
     status = CSL_dmaGetHwStatus(hDma,
                                  CSL_DMA_QUERY_STATUS,
                                  &dmaStatus);
   @endverbatim
 *
 *  @return Returns the status of the operation (see @a CSL_Status)
 * Status is:
 *              CSL_SOK - successful completion of the query
 *              CSL_ESYS_INVQUERY - query command not supported
 */

 CSL_Status  CSL_dmaGetHwStatus(
    /** Pointer to the object that holds reference to the
     *  instance of DMA channel requested after the call
         */
    CSL_DmaHandle           hDma,
    /** The query to this API, which indicates the status
     *  to be returned
         */
    CSL_DmaHwStatusQuery    query,
    /** Placeholder to return the status; @a void* casted
         */
    void                    *response
);

/**
 *  This function is used to read the chip context info. User should allocate
 *  memory for the said data type and pass its pointer as an unadorned
 *  void* argument to the status query call. For details about the various
 *  status queries supported and the associated data structure to record the
 *  response, refer to @a CSL_DmaChipCtxtQuery.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_dmaInit() and @a CSL_dmaOpen() must be called successfully
 *  in that order before @a CSL_dmaGetHwStatus() can be called. For the
 *  argument type that can be void* casted & passed with a particular command,
 *  refer to @a CSL_DmaChipCtxtQuery
 *
 *  @b Example:
 *  @verbatim
     Code snippet to get event ID 
      
     CSL_DmaHandle hDma;
     CSL_Status status;
     CSL_IntcEventId eventId ;
     ...
     
     status = CSL_DmaChipCtxtQuery(hDma,
                                  CSL_DMA_CHIPCTXTQUERY_EVENTID,
                                  &eventId);
   @endverbatim
 *
 *  @return Returns the status of the operation (see @a CSL_Status)
 *  Status is:
 *              CSL_SOK - successful completion of the query
 *              CSL_ESYS_INVQUERY - query command not supported
 */
 CSL_Status  CSL_dmaGetChipCtxt(
    /** Pointer to the object that holds reference to the
     *  instance of DMA channel requested after the call
         */
    CSL_DmaHandle           hDma,
    /** The chip context query, which indicates the chip context
     *  to be returned
         */
    CSL_DmaChipCtxtQuery    query,
    /** Placeholder to return the chip context; @a void* casted
         */
    void                    *response
);


#endif
