/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSL_DMA_H_
#define _CSL_DMA_H_

#include <csl_error.h>
#include <csl_types.h>
#include <cslr_dma.h>

/**************************************************************************\
* DMA global typedef declarations
\**************************************************************************/
/* Enumerations for DMA instances                */
typedef enum {
    CSL_DMA_ANY                    =                 -1,
    CSL_DMA_0                      =                  0
} CSL_DmaNum;

/* Enumerations for DMA channels                 */
typedef enum {
    CSL_DMA_CHA_ANY                =                 -1,
    CSL_DMA_CHA_0                  =                  0,
    CSL_DMA_CHA_1                  =                  1,
    CSL_DMA_CHA_2                  =                  2,
    CSL_DMA_CHA_3                  =                  3,
    CSL_DMA_CHA_4                  =                  4,
    CSL_DMA_CHA_5                  =                  5
} CSL_DmaChaNum;

/* Enumerations for Burst enable/disable         */
typedef enum {
    CSL_DMA_BURSTEN_DISABLE        =                  0,
    CSL_DMA_BURSTEN_ENABLE         =                  2
} CSL_DmaBurstEn;

/* Enumerations for Type of port                 */
typedef enum {
    CSL_DMA_PORT_SARAM             =                  0,
    CSL_DMA_PORT_DARAM             =                  1,
    CSL_DMA_PORT_EMIF              =                  2,
    CSL_DMA_PORT_RHEA              =                  3,
    CSL_DMA_PORT_API               =                  4
} CSL_DmaPort;

/* Enumerations for Addressing mode              */
typedef enum {
    CSL_DMA_AMODE_CONST            =                  0,
    CSL_DMA_AMODE_POSTINC          =                  1,
    CSL_DMA_AMODE_SNGLIND          =                  2,
    CSL_DMA_AMODE_DBLIND           =                  3
} CSL_DmaAmode;

/* Enumerations for Element size                 */
typedef enum {
    CSL_DMA_DATASIZE_8BIT          =                  0,
    CSL_DMA_DATASIZE_16BIT         =                  1,
    CSL_DMA_DATASIZE_32BIT         =                  2
} CSL_DmaDataSize;

/* Enumerations for Priority of channel          */
typedef enum {
    CSL_DMA_PRIORITY_LO            =                  0,
    CSL_DMA_PRIORITY_HI            =                  1
} CSL_DmaPriority;

/* Enumerations for Hardware synchronization     */
typedef enum {
    CSL_DMA_SYNC_NONE              =                  0,
    CSL_DMA_SYNC_MCSI1TX           =                  1,
    CSL_DMA_SYNC_MCSI1RX           =                  2,
    CSL_DMA_SYNC_MCSI2TX           =                  3,
    CSL_DMA_SYNC_MCSI2RX           =                  4,
    CSL_DMA_SYNC_MPUIO2            =                  5,
    CSL_DMA_SYNC_MPUIO4            =                  6,
    CSL_DMA_SYNC_FREE1             =                  7,
    CSL_DMA_SYNC_MCBSP1TX          =                  8,
    CSL_DMA_SYNC_MCBSP1RX          =                  9,
    CSL_DMA_SYNC_MCBSP3TX          =                 10,
    CSL_DMA_SYNC_MCBSP3RX          =                 11,
    CSL_DMA_SYNC_UART1TX           =                 12,
    CSL_DMA_SYNC_UART1RX           =                 13,
    CSL_DMA_SYNC_UART2TX           =                 14,
    CSL_DMA_SYNC_UART2RX           =                 15,
    CSL_DMA_SYNC_FREE2             =                 16,
    CSL_DMA_SYNC_FREE3             =                 17,
    CSL_DMA_SYNC_UART3TX           =                 18,
    CSL_DMA_SYNC_UART3RX           =                 19
} CSL_DmaSync;

/* Enumerations for Frame Synchronization        */
typedef enum {
    CSL_DMA_FRAMESYNC_DISABLE   =                     0,
    CSL_DMA_FRAMESYNC_ENABLE    =                     1
} CSL_DmaFrameSync;

/* Enumerations for type having values ON and    */
typedef enum {
    CSL_DMA_ONOFF_OFF                   =                     0,
    CSL_DMA_ONOFF_ON                    =                     1
} CSL_DmaOnOff;

/* Enumerations for DMA operations               */
typedef enum {
    CSL_DMA_CMD_START                           =                     0,
    CSL_DMA_CMD_STOP                            =                     1,
    CSL_DMA_CMD_CHNG_SRC                        =                     2,
    CSL_DMA_CMD_CHNG_DST                        =                     3,
        CSL_DMA_CMD_CHNG_ELEMENT_COUNT  =                         4,
        CSL_DMA_CMD_CHNG_FRAME_COUNT    =                         5
} CSL_DmaHwControlCmd;

/* Enumerations index mode                       */
typedef enum {
    CSL_DMA_INDEXMODE_SAME                      =                     0,
    CSL_DMA_INDEXMODE_DIFF                      =                     1
} CSL_DmaIndexMode;

/* Enumerations for h/w status query                     */
typedef enum {
    CSL_DMA_QUERY_STATUS                        =                     0,
    CSL_DMA_QUERY_CURRENT_HWSETUP       =                     1
} CSL_DmaHwStatusQuery;

/* Enumerations for chip context query                   */
typedef enum {
    CSL_DMA_CHIPCTXTQUERY_EVENTID               =                     0
} CSL_DmaChipCtxtQuery;

#define CSL_DMA_DEFAULTS_SRCPORT  { \
        (CSL_DmaBurstEn)CSL_DMA_CSDP_SRCBEN_RESETVAL, \
        (CSL_DmaOnOff)CSL_DMA_CSDP_SRCPACK_RESETVAL, \
        (CSL_DmaPort)CSL_DMA_CSDP_SRC_RESETVAL, \
        (CSL_DmaAmode)CSL_DMA_CCR_SRCAMODE_RESETVAL, \
        ((((Uint32)CSL_DMA_CSSAU_CSSAU_RESETVAL >> 1) << 16) + (((Uint32)CSL_DMA_CSSAL_CSSAL_RESETVAL >> 1) & 0xffff)), \
        (Uint16)CSL_DMA_CSFI_CSFI_RESETVAL, \
        (Uint16)CSL_DMA_CSEI_CSEI_RESETVAL \
    }


#define CSL_DMA_DEFAULTS_DSTPORT  { \
        (CSL_DmaBurstEn)CSL_DMA_CSDP_DSTBEN_RESETVAL, \
        (CSL_DmaOnOff)CSL_DMA_CSDP_DSTPACK_RESETVAL, \
        (CSL_DmaPort)CSL_DMA_CSDP_DST_RESETVAL, \
        (CSL_DmaAmode)CSL_DMA_CCR_DSTAMODE_RESETVAL, \
        ((((Uint32)CSL_DMA_CDSAU_CDSAU_RESETVAL >> 1) << 16) + (((Uint32)CSL_DMA_CDSAL_CDSAL_RESETVAL >> 1) & 0xffff)), \
        (Uint16)CSL_DMA_CDFI_CDFI_RESETVAL, \
        (Uint16)CSL_DMA_CDEI_CDEI_RESETVAL \
    }


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

#define CSL_DMA_DEFAULTS_INTR  { \
        (CSL_DmaOnOff)CSL_DMA_CICR_BLOCKIE_RESETVAL, \
        (CSL_DmaOnOff)CSL_DMA_CICR_LASTIE_RESETVAL, \
        (CSL_DmaOnOff)CSL_DMA_CICR_FRAMEIE_RESETVAL, \
        (CSL_DmaOnOff)CSL_DMA_CICR_FIRSTHALFIE_RESETVAL, \
        (CSL_DmaOnOff)CSL_DMA_CICR_DROPIE_RESETVAL, \
        (CSL_DmaOnOff)CSL_DMA_CICR_TIMEOUTIE_RESETVAL \
    } 

#define CSL_DMA_DEFAULTS_GLOBAL  { \
        (CSL_DmaOnOff)CSL_DMA_GCR_AUTOGATINGON_RESETVAL, \
        (CSL_Bool)CSL_DMA_GCR_FREE_RESETVAL, \
        (CSL_Bool)CSL_DMA_GCR_APIEXCL_RESETVAL, \
        (CSL_DmaPriority)CSL_DMA_GCR_APIPRIO_RESETVAL, \
        (CSL_Bool)CSL_DMA_GTCR_SARAMTCE_RESETVAL, \
        (CSL_Bool)CSL_DMA_GTCR_DARAMTCE_RESETVAL, \
        (CSL_DmaIndexMode)CSL_DMA_GSCR_INDEXMODE_RESETVAL \
    }


typedef struct CSL_DmaHwSetupPort CSL_DmaHwSetupSrcPort;

typedef struct CSL_DmaHwSetupPort CSL_DmaHwSetupDstPort;

typedef struct CSL_DmaHwSetupPort {
    CSL_DmaBurstEn          burstEn;
    CSL_DmaOnOff            packing;
    CSL_DmaPort             port;
    CSL_DmaAmode            amode;
    Uint32                  staddr;
    Int16                   frmIndex;
    Int16                   elmIndex;
} CSL_DmaHwSetupPort;

typedef struct CSL_DmaHwSetupChannel {
    CSL_DmaDataSize         datatype;
    Uint16                  elmCnt;
    Uint16                  frmCnt;
    CSL_DmaPriority         prio;
    CSL_DmaSync             sync;
    CSL_DmaFrameSync        fs;
    CSL_DmaOnOff            endprog;
    CSL_DmaOnOff            repeat;
    CSL_DmaOnOff            autoinit;
} CSL_DmaHwSetupChannel;

typedef struct CSL_DmaHwSetupIntr {
    CSL_DmaOnOff            blockie;
    CSL_DmaOnOff            lastie;
    CSL_DmaOnOff            frameie;
    CSL_DmaOnOff            firsthalfie;
    CSL_DmaOnOff            dropie;
    CSL_DmaOnOff            timeoutie;
} CSL_DmaHwSetupIntr;

typedef struct CSL_DmaHwSetupGlobal {
    CSL_DmaOnOff            autogating;
    CSL_Bool                free;
    CSL_Bool                apiexcl;
    CSL_DmaPriority         apiprio;
    CSL_Bool                stce;
    CSL_Bool                dtce;
    CSL_DmaIndexMode        indexMode;
} CSL_DmaHwSetupGlobal;

typedef struct CSL_DmaHwSetup {
    CSL_DmaHwSetupSrcPort   *srcStruct;
    CSL_DmaHwSetupDstPort   *dstStruct;
    CSL_DmaHwSetupChannel   *channelStruct;
    CSL_DmaHwSetupIntr      *intrStruct;
    CSL_DmaHwSetupGlobal    *globalStruct;
} CSL_DmaHwSetup;

typedef struct CSL_DmaStatus {
    CSL_Bool            timeout;
    CSL_Bool            eventDrop;
    CSL_Bool            halfframe;
    CSL_Bool            frame;
    CSL_Bool            lastframe;
    CSL_Bool            block;
    CSL_Bool            sync;
    Uint16                  srcAddrCntr;
    Uint16                  dstAddrCntr;
} CSL_DmaStatus;


typedef struct CSL_DmaObj {
    CSL_OpenMode                        openMode;       /* Always the first member of <PERI>Obj */
    CSL_Uid                                     uid;    /* Always the second member of <PERI>Obj */
    CSL_Xio                                     xio;    /* Always the third member of <PERI>Obj */
    CSL_DmaGlobalRegsOvly       gRegs;
    CSL_DmaNum                          dmaNum;
    CSL_DmaChaRegsOvly          regs;
    CSL_DmaChaNum                       chaNum;
} CSL_DmaObj;

typedef struct CSL_DmaObj *CSL_DmaHandle;

/**************************************************************************\
* DMA global function declarations
\**************************************************************************/
/*
 * ======================================================
 *   @func   CSL_dmaOpen
 *   @desc
 *         Open a DMA channel to get access to the resources for a particular channel
 *         (passed as an integer). On success, returns a valid handle which can be
 *         used to access the channel resources. On failure (when the channel is
 *         already open), returns NULL.
 *
 *
 *   @arg  dmaObj
 *              Pointer to the DMA Handle Object - to be allocated by the user
 *   @arg  dmaNum
 *              Instance Number of the DMA
 *   @arg  chaNum
 *              Channel number.
 *   @arg  openMode
 *              Open mode (Shared/Exclusive)
 *   @arg  hwSetup
 *              Pointer to setup structure
 *   @arg  status
 *              Pointer to CSL Error Status
 *   @ret  Handle
 *
 *
 * ======================================================
*/

 CSL_DmaHandle  CSL_dmaOpen(
    CSL_DmaObj                *dmaObj,
    CSL_DmaNum                dmaNum,
    CSL_DmaChaNum             chaNum,
    CSL_OpenMode              openMode,
    CSL_DmaHwSetup            *hwSetup,
    CSL_Status                *status
);

/*
 * ======================================================
 *   @func   CSL_dmaClose
 *   @desc   Close (Invalidate) a DMA channel (passed as handle) after it has finished
 *                       operating. The channel cannot be accessed any more.
 *
 *   @arg  hDma
 *              Handle to channel
 *   @ret  CSL_Status
 *
 * ======================================================
*/

 CSL_Status  CSL_dmaClose(
    CSL_DmaHandle             hDma
);

/*
 * ======================================================
 *   @func   CSL_dmaHwSetup
 *   @desc
 *         Configures a DMA channel using the different config structures.
 *         Sets up a particular DMA channel (passed as handle) using the
 *         configurations specified in the different structures (CSL_dmaHwSetupSrc),
 *         CSL_dmaHwSetupDst, CSL_dmaHwSetupCommon and CSL_dmaHwSetupIntr). This function
 *         calls the respective functions.
 *         The function will return an error if cannot complete the request
 *
 *   @arg  hDma
 *              Handle to the DMA Channel
 *   @arg  setup
 *              DMA setup structure for the DMA Channel
 *   @ret  CSL_Status
 *
 *
 * ======================================================
*/

 CSL_Status  CSL_dmaHwSetup(
    CSL_DmaHandle             hDma,
    CSL_DmaHwSetup            *setup
);

/*
 * ======================================================
 *   @func   CSL_dmaHwControl
 *   @desc   Controls the parameters of a particular DMA channel (passed as handle)
 *           There are various parameters such as
 *                 1) Change the Destination Address
 *                 2) Change the Source Address
 *           Usage Constraints:
 *           Channel should have been opened before with a valid handle
 *           If there are no shadow registers then we should ensure that transaction
 *           was completed before we use this function.
 *
 *          The are various Operations that can be done here are:
 *           1) Change the Destination Address
 *           2) Change the Source Address
 *           3) START/ENABLE the Channel
 *           4) STOP/DISABLE the Channel
 *
 *           The function will return an error if cannot complete the request
 *
 *
 *   @arg  hDma
 *              Handle to the channel
 *   @arg  cmd
 *              The operation to be performed.
 *   @arg  voidPtr
 *              This is a void pointer, and will be cast to the required type.
 *   @ret  CSL_Status
 *
 *
 * ======================================================
*/

 CSL_Status  CSL_dmaHwControl(
    CSL_DmaHandle         hDma,
    CSL_DmaHwControlCmd   cmd,
    void                  *voidPtr
);

/*
 * ======================================================
 *   @func   CSL_dmaInit
 *   @desc
 *         This is DMA module's init function. Currently it does nothing.
 *         Needs to be called before using DMA module.
 *
 *   @ret  void
 *
 *
 * ======================================================
*/

 void  CSL_dmaInit(
    void
);

/*
 * ======================================================
 *   @func   _CSL_dmaGetAttrs
 *   @desc
 *   @arg  dmaNum
 *              Instance number of DMA
 *   @arg  dmaChaNum
 *              Channel number
 *   @arg  hDma
 *              Handle to DMA instance
 *   @ret  void
 *
 * ======================================================
*/

 void  _CSL_dmaGetAttrs(
    CSL_InstNum                dmaNum,
    CSL_ChaNum                 dmaChaNum,
    CSL_DmaHandle              hDma
);

/*
 * ======================================================
 *   @func   CSL_dmaGetHwStatus
 *   @desc
 *         Traces the various parameters of a particular DMA channel (passed as handle)
 *
 *           Usage Constraints:
 *           Channel should have been opened before with a valid handle
 *           We should know that the registers can be read back.
 *
 *           The are various Operations that can be done here are:
 *           1) Trace the channel setup configuration
 *           2) Get the Status of the channel
 *
 *           The function will return an error if cannot complete the request
 *
 *   @arg  hDma
 *              Handle to the channel
 *   @arg  cmd
 *              The Values to be traced back
 *   @arg  voidPtr
 *              This is a void pointer, and will be cast to the required type.
 *   @ret  CSL_Status
 *
 *
 * ======================================================
*/

 CSL_Status  CSL_dmaGetHwStatus(
    CSL_DmaHandle                       hDma,
    CSL_DmaHwStatusQuery        query,
    void                                        *voidPtr
);

/*
 * ======================================================
 *   @func   CSL_dmaGetChipCtxt
 *   @desc
 *         Gets the chip cotext information a particular DMA channel (passed as handle)
 *
 *           Following are the types of chip context info that can be queried:
 *           1) Event ID 
 *
 *           The function will return an error if cannot complete the request
 *
 *   @arg  hDma
 *              Handle to the channel
 *   @arg  query
 *              The query that tells the type of chip context info to be queried
 *   @arg  voidPtr
 *              This is a void pointer, and will be cast to the required type.
 *   @ret  CSL_Status
 *
 *
 * ======================================================
*/

 CSL_Status  CSL_dmaGetChipCtxt(
    CSL_DmaHandle                       hDma,
    CSL_DmaChipCtxtQuery        query,
    void                                        *voidPtr
);


#endif
