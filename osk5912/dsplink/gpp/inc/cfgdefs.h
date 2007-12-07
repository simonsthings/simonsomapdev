/** ============================================================================
 *  @file   cfgdefs.h
 *
 *  @path   $(DSPLINK)\gpp\inc
 *
 *  @desc   Defines the global data structure for CFG.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
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
 *              Number of components of driver.
 *  @field  queueLength
 *              Length of queue supported by driver.
 *  @field  linkTables
 *              Number of Link tables in "this" configuration.
 *  @field  mmuTables
 *              Number of MMU tables in "this" configuration.
 *  @field  numMqas
 *              Number of MQA's for messaging.
 *  @field  numMqts
 *              Number of MQA's for messaging.
 *  @field  localMqt
 *              The id of the MQT which is to be used as Local MQT.
 *  ============================================================================
 */
typedef struct CFG_Driver_tag {
    Char8  driverName [CFG_MAX_STRLEN]    ;
    Uint32 components                     ;
    Uint32 queueLength                    ;
    Uint32 linkTables                     ;
    Uint32 mmuTables                      ;
#if defined (MSGQ_COMPONENT)
    Uint32 numMqas                        ;
    Uint32 numMqts                        ;
    Uint32 localMqt                       ;
#endif /* if defined (MSGQ_COMPONENT) */
} CFG_Driver ;


/** ============================================================================
 *  @name   CFG_Gpp
 *
 *  @desc   Driver configuration structure.
 *
 *  @field  gppName
 *              Name of GPP Processor.
 *  @field  numDsps
 *              Number of DSPs.
 *  ============================================================================
 */
typedef struct CFG_Gpp_tag {
    Char8   gppName [CFG_MAX_STRLEN] ;
    Uint32  numDsps ;
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
 *  @field  execName
 *              Name of executable to load.
 *  @field  loaderInterface
 *              Function pointer interface for accessing the loader.
 *  @field  linkTable
 *              Table number of the link(s) toward this DSP
 *  @field  linkTableSize
 *              Size of the link table.
 *  @field  autoStart
 *              AutoStart flag.
 *  @field  resetVector
 *              Address of reset vector of DSP.
 *  @field  wordSize
 *              Word size of DSP in bytes.
 *  @field  endian
 *              Endian info of DSP.
 *  @field  mmuFlag
 *              Is MMU used?
 *  @field  mmuTable
 *              Table number of the MMU entries for this DSP.
 *  @field  mmuTableSize
 *              Number of entries in MMU table.
 *  @field  interface
 *              Function pointer interface for accessing the DSP.
 *  @field  mqtId
 *              The id of the MQT which is to be used for this DSP.
 *  ============================================================================
 */
typedef struct CFG_Dsp_tag {
    Char8   dspName    [CFG_MAX_STRLEN] ;
    Uint32  dspArch         ;
    Char8   execName   [CFG_MAX_STRLEN] ;
    Pvoid   loaderInterface ;
    Uint32  linkTable       ;
    Uint32  linkTableSize   ;
    Uint32  autoStart       ;
    Uint32  resetVector     ;
    Uint32  wordSize        ;
    Uint32  endian          ;
    Uint32  mmuFlag         ;
    Uint32  mmuTable        ;
    Uint32  mmuTableSize    ;
    Pvoid   interface       ;
#if defined (MSGQ_COMPONENT)
    Uint32  mqtId           ;
#endif /* if defined (MSGQ_COMPONENT) */
} CFG_Dsp ;


/** ============================================================================
 *  @name   CFG_Link
 *
 *  @desc   Link configuration structure.
 *
 *  @field  linkName
 *              Name of Link.
 *  @field  abbr
 *              Abbreviation of the link name.
 *  @field  baseChnlId
 *              Base channel ID for this link.
 *  @field  numChannels
 *              Number of channels for this link.
 *  @field  maxBufSize
 *              Maximum size of data buffer on this link.
 *  @field  interfaceTable
 *              Interface function table address.
 *  @field  argument1
 *              Link specific argument 1.
 *              The significance of this argument is specific to a link driver.
 *  @field  argument2
 *              Link specific argument 2.
 *              The significance of this argument is specific to a link driver.
 *  ============================================================================
 */
typedef struct CFG_Link_tag {
    Char8   linkName [CFG_MAX_STRLEN] ;
    Char8   abbr     [CFG_MAX_STRLEN] ;
    Uint32  baseChnlId ;
    Uint32  numChannels ;
    Uint32  maxBufSize ;
    Pvoid   interfaceTable ;
    Uint32  argument1 ;
    Uint32  argument2 ;
} CFG_Link ;


/** ============================================================================
 *  @name   CFG_MmuEntry
 *
 *  @desc   Defines an entry in the MMU table.
 *
 *  @field  entry
 *              Entry number.
 *  @field  virtualAddress
 *              virtual address field of entry.
 *  @field  physicalAddress
 *              physical address field of entry.
 *  @field  size
 *              Size field of entry.
 *  @field  access
 *              Access Permission.
 *  @field  preserve
 *              Preserve field of entry.
 *  @field  mapInGpp
 *              Flag indicating whether DSP address is mapped to GPP
 *              address space.
 *  ============================================================================
 */
typedef struct CFG_MmuEntry_tag {
    Uint32  entry           ;
    Uint32  virtualAddress  ;
    Uint32  physicalAddress ;
    Uint32  size            ;
    Uint32  access          ;
    Uint32  preserve        ;
    Uint32  mapInGpp        ;
} CFG_MmuEntry ;


#if defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @name   CFG_Mqa
 *
 *  @desc   This structure defines the MQA configuration structure.
 *
 *  @field  mqaName
 *              Name of the MQA. For debugging purposes only.
 *  @field  interface
 *              Function pointer interface to access the functions for this MQA.
 *  ============================================================================
 */
typedef struct CFG_Mqa_tag {
    Char8  mqaName    [CFG_MAX_STRLEN] ;
    Pvoid  interface                   ;
} CFG_Mqa ;


/** ============================================================================
 *  @name   CFG_Mqt
 *
 *  @desc   This structure defines the MQT configuration structure.
 *
 *  @field  mqtName
 *              Name of the MQT. For debugging purposes only.
 *  @field  interface
 *              Function pointer interface to access the functions for this MQT.
 *  @field  linkId
 *              ID of the link used by this MQT.
 *  ============================================================================
 */
typedef struct CFG_Mqt_tag {
    Char8  mqtName    [CFG_MAX_STRLEN] ;
    Pvoid  interface                   ;
    Uint32 linkId                      ;
} CFG_Mqt ;
#endif /* if defined (MSGQ_COMPONENT) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (CFGDEFS_H) */
