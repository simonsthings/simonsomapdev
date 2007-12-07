/** ============================================================================
 *  @file   linkdefs.h
 *
 *  @path   $(DSPLINK)\gpp\inc
 *
 *  @desc   Definitions of constants and structures for Link.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LINK_H)
#define LINK_H


USES (gpptypes.h)
USES (dsplink.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @name   LinkAttrs_tag
 *
 *  @desc   Defines the attributes of a link.
 *
 *  @field  linkName
 *              Name of physical link.
 *  @field  abbr
 *              Abbreviation for the driver name.
 *  @field  baseChnlId
 *              Start channel Id for the link.
 *  @field  numChannels
 *              Number of (virtual) channels on link.
 *  @field  maxBufSize
 *              Maximum buffer size supported on channel
 *  @field  interface
 *              The table of function pointers from DSP subcomponent.
 *  @field  argument1
 *              Link specific argument 1.
 *              The significance of this argument is specific to a link driver.
 *  @field  reserved2
 *              Link reserved argument 2.
 *              The significance of this argument is specific to a link driver.
 *  ============================================================================
 */
struct LinkAttrs_tag {
    Char8   linkName [DSP_MAX_STRLEN] ;
    Char8   abbr [DSP_MAX_STRLEN] ;
    Uint32  baseChnlId  ;
    Uint32  numChannels ;
    Uint32  maxBufSize  ;
    Void *  interface   ;
    Uint32  argument1   ;
    Uint32  reserved2   ;
} ;


/** ============================================================================
 *  @func   FnLinkInitialize
 *
 *  @desc   Signature of function that allocates and initializes resources
 *          used by the Link Driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    linkId
 *              Identifier of link to be initialized.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EMEMORY
 *              Out of memory
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLinkInitialize) (IN ProcessorId dspId,
                                        IN LinkId      linkId) ;

/** ============================================================================
 *  @func   FnLinkFinalize
 *
 *  @desc   Signature of function that de-allocates and finalizes resources
 *          used by the Shared Memory Driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    linkId
 *              Identifier of link to be finalized.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLinkFinalize) (IN ProcessorId dspId,
                                      IN LinkId      linkId) ;

/** ============================================================================
 *  @func   FnLinkOpenChannel
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
typedef DSP_STATUS (*FnLinkOpenChannel) (IN ProcessorId dspId,
                                         IN ChannelId chnlId) ;

/** ============================================================================
 *  @func   FnLinkCloseChannel
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
typedef DSP_STATUS (*FnLinkCloseChannel) (IN ProcessorId dspId,
                                          IN ChannelId chnlId) ;

/** ============================================================================
 *  @func   FnLinkCancelIO
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
typedef DSP_STATUS (*FnLinkCancelIO) (IN ProcessorId dspId,
                                      IN ChannelId chnlId) ;

/** ============================================================================
 *  @func   FnLinkIORequest
 *
 *  @desc   Signature of function that de-allocates and finalizes resources
 *          used by the Shared Memory Driver.
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
typedef DSP_STATUS (*FnLinkIORequest) (IN ProcessorId dspId,
                                       IN ChannelId   chnlId) ;

/** ============================================================================
 *  @func   FnLinkScheduleDpc
 *
 *  @desc   Signature of function that schedules a DPC for IO on
 *          the specified channel.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *
 *  @arg    chnlId
 *              Channel ID on which IO is being requested.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLinkScheduleDpc) (IN ProcessorId dspId,
                                         IN ChannelId   chnlId) ;

/** ============================================================================
 *  @func   FnLinkHandshakeSetup
 *
 *  @desc   Signature of function that setup the handshake process toward
 *          specified DSP on the Link Driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLinkHandshakeSetup) (IN ProcessorId dspId) ;

/** ============================================================================
 *  @func   FnLinkHandshakeStart
 *
 *  @desc   Signature of function that starts the handshake process toward
 *          specified DSP on the Link Driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLinkHandshakeStart) (IN ProcessorId dspId) ;

/** ============================================================================
 *  @func   FnLinkHandshakeComplete
 *
 *  @desc   Signature of function that completes the handshake process toward
 *          specified DSP on the Link Driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLinkHandshakeComplete) (IN ProcessorId dspId) ;

/** ============================================================================
 *  @name   LinkInterface
 *
 *  @desc   Interface functions exported by the Link Driver.
 *
 *  @field  initialize
 *              Function pointer to the Link Driver initialize function.
 *  @field  finalize
 *              Function pointer to the Link Driver finalize function.
 *  @field  openChannel
 *              Function pointer to the Link Driver function to open channel.
 *  @field  closeChannel
 *              Function pointer to the Link Driver function to close channel.
 *  @field  cancelChannel
 *              Function pointer to the Link Driver function to cancel channel.
 *  @field  ioRequest
 *              Function pointer to the Link Driver function to request IO.
 *  @field  scheduleDpc
 *              Function pointer to the Link Driver function to schedule DPC.
 *  @field  handshakeSetup
 *              Function pointer to the Link Driver function to setup handshake.
 *  @field  handshakeStart
 *              Function pointer to the Link Driver function to start handshake.
 *  @field  handshakeComplete
 *              Function pointer to the Link Driver function to complete
 *              handshake.
 *  ============================================================================
 */
typedef struct LinkInterface_tag {
    FnLinkInitialize        initialize        ;
    FnLinkFinalize          finalize          ;
    FnLinkOpenChannel       openChannel       ;
    FnLinkCloseChannel      closeChannel      ;
    FnLinkCancelIO          cancelChannel     ;
    FnLinkIORequest         ioRequest         ;
    FnLinkScheduleDpc       scheduleDpc       ;
    FnLinkHandshakeSetup    handshakeSetup    ;
    FnLinkHandshakeStart    handshakeStart    ;
    FnLinkHandshakeComplete handshakeComplete ;
} LinkInterface ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (LINK_H) */
