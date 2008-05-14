/** ============================================================================
 *  @file   datadefs.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Definitions of constants and structures for the data driver.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (DATADEFS_H)
#define DATADEFS_H


USES (gpptypes.h)
USES (dsplink.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/*  ============================================================================
 *  @name   LinkDataInterface
 *
 *  @desc   Forward declaration of LinkDataInterface structure.
 *  ============================================================================
 */
typedef struct LinkDataInterface_tag LinkDataInterface ;


/** ============================================================================
 *  @name   DataObject_tag
 *
 *  @desc   Defines the a data driver object.
 *
 *  @field  name
 *              Name of the data driver.
 *              Defined only for debug build.
 *  @field  abbr
 *              Abbreviation of the data driver name.
 *              Defined only for debug build.
 *  @field  baseChnlId
 *              Base channel ID for the data driver.
 *  @field  numChannels
 *              Number of (virtual) channels supported by the data driver.
 *  @field  maxBufSize
 *              Maximum buffer size supported on the channels.
 *  @field  interface
 *              Pointer to the data driver interface table.
 *  @field  memEntry
 *              ID of the LINK mem information entry in the DSP memTable
 *  @field  poolId
 *              Identifier for the pool from where buffers are allocated.
 *  @field  size
 *              Size of the memory area used by the data driver.
 *  @field  arg1
 *              First argument specific to the data driver.
 *  @field  arg2
 *              Second argument specific to the data driver.
 *  ============================================================================
 */
struct DataObject_tag {
#if defined (DDSP_DEBUG)
    Char8               name [DSP_MAX_STRLEN] ;
    Char8               abbr [DSP_MAX_STRLEN] ;
#endif /* if defined (DDSP_DEBUG) */
    Uint32              baseChnlId  ;
    Uint32              numChannels ;
    Uint32              maxBufSize  ;
    LinkDataInterface * interface   ;
    Uint32              memEntry    ;
    Uint32              poolId      ;
    Uint32              size        ;
    Uint32              arg1        ;
    Uint32              arg2        ;
} ;


/** ============================================================================
 *  @func   FnDataInitialize
 *
 *  @desc   Signature of function that allocates and initializes resources
 *          used by the Data Driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    linkId
 *              Identifier of data link to be initialized.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EMEMORY
 *              Out of memory
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDataInitialize) (IN ProcessorId dspId,
                                        IN LinkId      linkId) ;

/** ============================================================================
 *  @func   FnDataFinalize
 *
 *  @desc   Signature of function that de-allocates and finalizes resources
 *          used by the data driver
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    linkId
 *              Identifier of data link to be finalized.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDataFinalize) (IN ProcessorId dspId,
                                      IN LinkId      linkId) ;

/** ============================================================================
 *  @func   FnDataOpenChannel
 *
 *  @desc   Signature of function that opens a channel for input/output.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    chnlId
 *              Channel ID on which IO is being requested.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Could not open the channel successfully.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDataOpenChannel) (IN ProcessorId dspId,
                                         IN ChannelId   chnlId) ;

/** ============================================================================
 *  @func   FnDataCloseChannel
 *
 *  @desc   Signature of function that closes a channel.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    chnlId
 *              Channel ID on which IO is being requested.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Could not open the channel successfully.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDataCloseChannel) (IN ProcessorId    dspId,
                                          IN ChannelId      chnlId) ;

/** ============================================================================
 *  @func   FnDataCancelIO
 *
 *  @desc   Signature of function that cancels a channel.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Could not open the channel successfully.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDataCancelIO) (IN ProcessorId    dspId,
                                      IN ChannelId      chnlId) ;

/** ============================================================================
 *  @func   FnDataRequest
 *
 *  @desc   Signature of function that de-allocates and finalizes resources
 *          used by the data driver
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    chnlId
 *              Channel ID on which IO is being requested.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              chnlId is invalid.
 *          DSP_EPOINTER
 *              Subcomponent is not initialized.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDataRequest) (IN ProcessorId dspId,
                                     IN ChannelId   chnlId) ;

#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   FnDataDebug
 *
 *  @desc   Signature of function that prints debug information for the data
 *          driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *
 *  @ret    None.
 *  ============================================================================
 */
typedef Void (*FnDataDebug) (IN ProcessorId dspId) ;
#endif /* if defined (DDSP_DEBUG) */


/** ============================================================================
 *  @name   LinkDataInterface_tag
 *
 *  @field  initialize
 *              Function pointer to the function to initialize the data driver.
 *  @field  finalize
 *              Function pointer to the function to finalize the data driver.
 *  @field  openChannel
 *              Function pointer to the Data Driver function to open channel.
 *  @field  closeChannel
 *              Function pointer to the Data Driver function to close channel.
 *  @field  cancelChannel
 *              Function pointer to the Data Driver function to cancel channel.
 *  @field  request
 *              Function pointer to the Data Driver function to request IO.
 *  @field  debug
 *              Function pointer to the Data Driver function to print debug
 *              information.
 *  ============================================================================
 */
struct LinkDataInterface_tag {
    FnDataInitialize        initialize ;
    FnDataFinalize          finalize ;
    FnDataOpenChannel       openChannel ;
    FnDataCloseChannel      closeChannel ;
    FnDataCancelIO          cancelChannel ;
    FnDataRequest           request ;
#if defined (DDSP_DEBUG)
    FnDataDebug             debug ;
#endif /* if defined (DDSP_DEBUG) */
} ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (DATADEFS_H) */
