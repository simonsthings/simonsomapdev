/** ============================================================================
 *  @file   cfgdefs.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Defines the global data structure for CFG.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (CFGDEFS_H)
#define CFGDEFS_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


USES (gpptypes.h)


/** ============================================================================
 *  @const  CFG_MAX_STRLEN
 *
 *  @desc   Maximum length of the strings used in CFG.
 *
 *  ============================================================================
 */
#define CFG_MAX_STRLEN  32


/** ============================================================================
 *  @name   CFG_Driver
 *
 *  @desc   Driver configuration structure.
 *
 *  @field  driverName
 *              Name of driver.
 *  @field  components
 *              Components selected in the LINK build configuration.
 *              This field is currently not used.
 *  @field  queueLength
 *              Queue length for number of buffers that can be simultaneously
 *              be issues on a channel.
 *  @field  numDsps
 *              Number of DSPs in the current configuration.
 *  @field  memTables
 *              Number of MEM tables in the current configuration.
 *  @field  numIpsTables
 *              Number of Inter Processor Signalling components tables
 *              in the current configuration.
 *  @field  numPools
 *              Number of memory pools supported by the current configuration.
 *  @field  numDataTables
 *              Number of data transfer drivers tables supported in current
 *              configuration.
 *  @field  maxMsgqs
 *              Maximum number of MSGQs supported in this configuration.
 *  @field  numMqts
 *              Number of Transports supported in current configuration.
 *  @field  probeRtcId
 *              The id of the timer which is to be used for PROBE.
 *              This field is defined only when probe compoennt is enabled.
 *  @field  probeRtcIntId
 *              The ISR id of the timer to be used for PROBE.
 *              This field is defined only when probe compoennt is enabled.
 *  ============================================================================
 */
typedef struct CFG_Driver_tag {
    Char8  driverName [CFG_MAX_STRLEN]    ;
    Uint32 components                     ;
#if defined (CHNL_COMPONENT)
    Uint32 queueLength                    ;
#endif /* if defined (CHNL_COMPONENT) */
    Uint32 numDsps                        ;
    Uint32 memTables                      ;
#if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT)
    Uint32 numIpsTables                   ;
    Uint32 numPools                       ;
#endif /* if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT) */
#if defined (CHNL_COMPONENT)
    Uint32 numDataTables                  ;
#endif /* if defined (CHNL_COMPONENT) */
#if defined (MSGQ_COMPONENT)
    Uint32 maxMsgqs                       ;
    Uint32 numMqts                        ;
#endif /* if defined (MSGQ_COMPONENT) */
#if defined (DDSP_PROBE)
    Uint32 probeRtcId                     ;
    Uint32 probeRtcIntId                  ;
#endif /* if defined (DDSP_PROBE) */
} CFG_Driver ;


/** ============================================================================
 *  @name   CFG_Gpp
 *
 *  @desc   Driver configuration structure.
 *
 *  @field  gppName
 *              Name of GPP Processor.
 *  ============================================================================
 */
typedef struct CFG_Gpp_tag {
    Char8   gppName [CFG_MAX_STRLEN] ;
} CFG_Gpp ;


/** ============================================================================
 *  @name   CFG_Dsp
 *
 *  @desc   Processor configuration structure.
 *
 *  @field  dspName
 *              Name of DSP processor.
 *  @field  dspArch
 *              Architecture of the DSP.
 *  @field  interface
 *              Function pointer interface for accessing the DSP.
 *  @field  loaderInterface
 *              Function pointer interface for accessing the loader.
 *  @field  autoStart
 *              AutoStart flag.
 *  @field  execName
 *              Name of executable to load.
 *              This field is currently not used.
 *  @field  resetVector
 *              Address of reset vector of DSP.
 *  @field  resetCodeSize
 *              Size of code at DSP Reset Vector.
 *  @field  maduSize
 *              Minimum addressable unit on the DSP.
 *  @field  endian
 *              Endian info of DSP.
 *  @field  memEntries
 *              Number of entries in MEM table.
 *  @field  memTable
 *              Table number of the MEM entries for this DSP.
 *  @field  linkdrvId
 *              Link Driver table identifier for this DSP.
 *  @field  numDataDrivers
 *              Size of the link table.
 *  @field  dataTableId
 *              Table number of the link(s) toward this DSP
 *  @field  mqtId
 *              The id of the MQT which is to be used for this DSP.
 *  ============================================================================
 */
typedef struct CFG_Dsp_tag {
    Char8   dspName    [CFG_MAX_STRLEN] ;
    Uint32  dspArch         ;
    Pvoid   interface       ;
    Pvoid   loaderInterface ;
    Uint32  autoStart       ;
    Char8   execName   [CFG_MAX_STRLEN] ;
    Uint32  resetVector     ;
    Uint32  resetCodeSize   ;
    Uint32  maduSize        ;
    Uint32  endian          ;
    Uint32  memEntries      ;
    Uint32  memTable        ;
#if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT)
    Uint32  linkdrvId       ;
#endif /* if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT) */
#if defined (CHNL_COMPONENT)
    Uint32  numDataDrivers  ;
    Uint32  dataTableId     ;
#endif /* if defined (CHNL_COMPONENT) */
#if defined (MSGQ_COMPONENT)
    Uint32  mqtId           ;
#endif /* if defined (MSGQ_COMPONENT) */
} CFG_Dsp ;


/** ============================================================================
 *  @name   CFG_MemEntry
 *
 *  @desc   Defines an entry in the MEM table.
 *
 *  @field  entry
 *              Entry number.
 *  @field  abbr
 *              Abbreviation to be used for generating base address for this
 *              memory block.
 *  @field  dspVirtualAddress
 *              virtual address field of entry.
 *  @field  physicalAddress
 *              physical address field of entry.
 *  @field  size
 *              Size field of entry.
 *  @field  mapInGpp
 *              Flag indicating whether DSP address is mapped to GPP
 *              address space.
 *  ============================================================================
 */
typedef struct CFG_MemEntry_tag {
    Uint32  entry             ;
    Char8   abbr [CFG_MAX_STRLEN] ;
    Uint32  dspVirtualAddress ;
    Uint32  physicalAddress   ;
    Uint32  size              ;
    Uint32  mapInGpp          ;
} CFG_MemEntry ;


#if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @name   CFG_Pools
 *
 *  @desc   Configuration information for buffer pools from which buffers
 *          for use with LINK can be allocated through the POOL interface(s).
 *          The buffers can be used for both - message as well as data
 *          transfer.
 *
 *  @field  poolName
 *              Name of the pool.
 *  @field  abbr
 *              Abbreviated name for generating macros.
 *  @field  initialize
 *              Initialization function for the buffer pool.
 *  @field  finalize
 *              Finalization function for the buffer pool.
 *  @field  interface
 *              Function interface table for the buffer pool.
 *  @field  dspId
 *              DSP Identifier for the pool.
 *  @field  memEntry
 *              MEM Entry for the memory area used by this buffer pool.
 *              This field is optional and can be specified as -1 in case
 *              a MEM entry is not required for the buffer pool (e.g. in case
 *              of PCPY Messaging).
 *  @field  addrOffset
 *              Offset of the pool from the base address of the memEntry used
 *              by the pool.
 *  @field  poolSize
 *              Size of the buffer pool.
 *  @field  arg1
 *              Optional pool specific argument.
 *  @field  arg2
 *              Optional pool specific argument.
 *  ============================================================================
 */
typedef struct CFG_Pools_tag {
    Char8              poolName [CFG_MAX_STRLEN] ;
    Char8              abbr     [CFG_MAX_STRLEN] ;
    FnPoolInitialize * initialize ;
    FnPoolFinalize *   finalize   ;
    Pvoid              interface  ;
    Uint32             dspId      ;
    Uint32             memEntry   ;
    Uint32             addrOffset ;
    Uint32             poolSize   ;
    Uint32             arg1       ;
    Uint32             arg2       ;
} CFG_Pools ;
#endif /* if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT) */


#if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @name   CFG_LinkDrv
 *
 *  @desc   Physical link driver configuration information.
 *
 *  @field  linkDrvName
 *              Name of the physical link driver configuration.
 *  @field  abbr
 *              Abbreviated name for generating macros.
 *  @field  interface
 *              Function pointer interface for the physical link driver.
 *  @field  memEntry
 *              MEM Entry for the memory area used by this physical link
 *              configuration information.
 *  @field  size
 *              Size of memory area for the physical link configuration
 *              information.
 *  @field  numIpsEntries
 *              Number of IPS table entries for this link driver.
 *  @field  ipsTableId
 *              IPS table id
 *  ============================================================================
 */
typedef struct CFG_LinkDrv_tag {
    Char8   linkDrvName [CFG_MAX_STRLEN] ;
    Char8   abbr        [CFG_MAX_STRLEN] ;
    Pvoid   interface     ;
    Uint32  memEntry      ;
    Uint32  size          ;
    Uint32  numIpsEntries ;
    Uint32  ipsTableId    ;
} CFG_LinkDrv ;
#endif /* if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT) */


#if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @name   CFG_Ips
 *
 *  @desc   Configuration information for the Interprocessor Signalling
 *          Component
 *
 *  @field  ipsName
 *              Name of the IPS component.
 *  @field  abbr
 *              Abbreviated name for generating macros.
 *  @field  initialize
 *              Function pointer for the initialization function.
 *  @field  finalize
 *              Function pointer for the finalization function.
 *  @field  queuePerChnl
 *              Number of IO requests to be queued within IPS for data transfer.
 *  @field  irpSize
 *              Size of each IRP packet.
 *  @field  memEntry
 *              MEM entry for the memory information used for this IPS
 *              component.
 *  @field  size
 *              Size of memory area configured for the IPS component.
 *  @field  arg1
 *              IPS specific optional argument.
 *  @field  arg2
 *              IPS specific optional argument.
 *  ============================================================================
 */
typedef struct CFG_Ips_tag {
    Char8             ipsName [CFG_MAX_STRLEN] ;
    Char8             abbr [CFG_MAX_STRLEN] ;
    FnIpsInitialize * initialize   ;
    FnIpsFinalize *   finalize     ;
    Uint32            queuePerChnl ;
    Uint32            irpSize      ;
    Uint32            memEntry     ;
    Uint32            size         ;
    Uint32            arg1         ;
    Uint32            arg2         ;
} CFG_Ips ;
#endif /* if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT) */


#if defined (CHNL_COMPONENT)
/** ============================================================================
 *  @name   CFG_DataDrv
 *
 *  @desc   Data streaming driver configuration structure.
 *
 *  @field  name
 *              Name of the data transfer driver.
 *  @field  abbr
 *              Abbreviation of the link name.
 *  @field  baseChnlId
 *              Base channel ID for this link.
 *  @field  numChannels
 *              Number of channels for this link.
 *  @field  maxBufSize
 *              Maximum size of data buffer on this link.
 *  @field  interface
 *              Interface function table address.
 *  @field  memEntryId
 *              MEM entry for the memory area for data streaming driver.
 *  @field  poolId
 *              Identifier for the pool from where buffers are allocated.
 *  @field  size
 *              Size of the area for data streaming driver.
 *  @field  arg1
 *              Link specific argument 1.
 *              The significance of this arg is specific to a link driver.
 *  @field  arg2
 *              Link specific argument 2.
 *              The significance of this argument is specific to a link driver.
 *  ============================================================================
 */
typedef struct CFG_DataDrv_tag {
    Char8   name [CFG_MAX_STRLEN] ;
    Char8   abbr [CFG_MAX_STRLEN] ;
    Uint32  baseChnlId     ;
    Uint32  numChannels    ;
    Uint32  maxBufSize     ;
    Pvoid   interface      ;
    Uint32  memEntryId     ;
    Uint32  poolId         ;
    Uint32  size           ;
    Uint32  arg1           ;
    Uint32  arg2           ;
} CFG_DataDrv ;
#endif /* if defined (CHNL_COMPONENT) */


#if defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @name   CFG_Mqt
 *
 *  @desc   This structure defines the MQT configuration structure.
 *
 *  @field  mqtName
 *              Name of the MQT. For debugging purposes only.
 *  @field  abbr
 *              Abbreviation of the MQT name.
 *  @field  interface
 *              Function pointer interface to access the functions for this MQT.
 *  @field  memEntryId
 *              MEM entry id for the memory area reserved for this MQT.
 *  @field  maxMsgSize
 *              Maximum size of message supported by MQT.
 *  @field  size
 *              Size of memory area for this MQT.
 *  @field  arg1
 *              Optional argument 1 for this MQT.
 *  @field  arg2
 *              Optional argument 2 for this MQT.
 *  ============================================================================
 */
typedef struct CFG_Mqt_tag {
    Char8   mqtName [CFG_MAX_STRLEN] ;
    Char8   abbr    [CFG_MAX_STRLEN] ;
    Pvoid   interface  ;
    Uint32  memEntryId ;
    Uint32  maxMsgSize ;
    Uint32  size       ;
    Uint32  arg1      ;
    Uint32  arg2      ;
} CFG_Mqt ;
#endif /* if defined (MSGQ_COMPONENT) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (CFGDEFS_H) */
