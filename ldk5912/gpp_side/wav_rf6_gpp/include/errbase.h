/** ============================================================================
 *  @file   errbase.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Central repository for error and status code bases and ranges for
 *          DSP/BIOS LINK and any Algorithm Framework built on top of LINK.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (ERRBASE_H)
#define ERRBASE_H


USES (gpptypes.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/* Define the return code type */
typedef Int32  DSP_STATUS ;

/* Success & Failure macros*/
#define DSP_SUCCEEDED(status)   (    ((Int32) (status) >= DSP_SBASE) \
                                  && ((Int32) (status) <= DSP_SFINALIZED))

#define DSP_FAILED(status)      (!DSP_SUCCEEDED (status))


/* Base of generic errors and component errors */
#define DSP_SBASE               (DSP_STATUS)0x00008000
#define DSP_EBASE               (DSP_STATUS)0x80008000
#define DSP_COMP_EBASE          (DSP_STATUS)0x80040200
#define DSP_COMP_ELAST          (DSP_STATUS)0x80047fff


/*  ============================================================================
 *  SUCCESS Codes
 *  ============================================================================
 */

/* Generic success code */
#define DSP_SOK                     (DSP_SBASE + 0x0)


/* GPP is already attached to this DSP processor */
#define DSP_SALREADYATTACHED        (DSP_SBASE + 0x1)

/* This is the last object available for enumeration. */
#define DSP_SENUMCOMPLETE           (DSP_SBASE + 0x2)

/* Object has been finalized successfully. */
#define DSP_SFINALIZED              (DSP_SBASE + 0x3)


/*  ============================================================================
 *  FAILURE Codes
 *  ============================================================================
 */

/* The caller does not have access privileges to call this function */
#define DSP_EACCESSDENIED           (DSP_EBASE + 0x0)

/* The Specified Connection already exists */
#define DSP_EALREADYCONNECTED       (DSP_EBASE + 0x1)

/* The GPP must be detached from the DSP before this function is called */
#define DSP_EATTACHED               (DSP_EBASE + 0x2)

/* During enumeration a change in the number or properties of the objects
 * has occurred.
 */
#define DSP_ECHANGEDURINGENUM       (DSP_EBASE + 0x3)

/* An error occurred while parsing the DSP executable file */
#define DSP_ECORRUPTFILE            (DSP_EBASE + 0x4)

/* A failure occurred during a delete operation */
#define DSP_EDELETE                 (DSP_EBASE + 0x5)

/* The specified direction is invalid */
#define DSP_EDIRECTION              (DSP_EBASE + 0x6)

/* A stream has been issued the maximum number of buffers allowed in the
 * stream at once.  buffers must be reclaimed from the stream before any
 * more can be issued.
 */
#define DSP_ESTREAMFULL             (DSP_EBASE + 0x7)

/* A general failure occurred */
#define DSP_EFAIL                   (DSP_EBASE + 0x8)

/* The specified executable file could not be found. */
#define DSP_EFILE                   (DSP_EBASE + 0x9)

/* The specified handle is invalid. */
#define DSP_EHANDLE                 (DSP_EBASE + 0xA)

/* An invalid argument was specified. */
#define DSP_EINVALIDARG             (DSP_EBASE + 0xB)

/* A memory allocation failure occurred. */
#define DSP_EMEMORY                 (DSP_EBASE + 0xC)

/* Reserved error code */
#define DSP_ERESERVED_1             (DSP_EBASE + 0xD)

/* Reserved error code */
#define DSP_ERESERVED_2             (DSP_EBASE + 0xE)

/* Reserved error code */
#define DSP_ERESERVED_3             (DSP_EBASE + 0xF)

/* The indicated operation is not supported. */
#define DSP_ENOTIMPL                (DSP_EBASE + 0x10)

/* I/O is currently pending. */
#define DSP_EPENDING                (DSP_EBASE + 0x11)

/* An invalid pointer was specified. */
#define DSP_EPOINTER                (DSP_EBASE + 0x12)

/* A parameter is specified outside its valid range. */
#define DSP_ERANGE                  (DSP_EBASE + 0x13)

/* An invalid size parameter was specified. */
#define DSP_ESIZE                   (DSP_EBASE + 0x14)

/* A stream creation failure occurred on the DSP. */
#define DSP_ESTREAM                 (DSP_EBASE + 0x15)

/* A task creation failure occurred on the DSP. */
#define DSP_ETASK                   (DSP_EBASE + 0x16)

/* A timeout occurred before the requested operation could complete. */
#define DSP_ETIMEOUT                (DSP_EBASE + 0x17)

/* A data truncation occurred, e.g., when requesting a descriptive error
 * string, not enough space was allocated for the complete error message.
 */
#define DSP_ETRUNCATED              (DSP_EBASE + 0x18)

/* A parameter is invalid. */
#define DSP_EVALUE                  (DSP_EBASE + 0x1A)

/* The state of the specified object is incorrect for the requested
 * operation.
 */
#define DSP_EWRONGSTATE             (DSP_EBASE + 0x1B)

/* Reserved error code */
#define DSP_ERESERVED_4             (DSP_EBASE + 0x1C)

/* Reserved error code */
#define DSP_ERESERVED_5             (DSP_EBASE + 0x1D)

/* Reserved error code */
#define DSP_ERESERVED_6             (DSP_EBASE + 0x1E)

/* Reserved error code */
#define DSP_ERESERVED_7             (DSP_EBASE + 0x1F)

/* Reserved error code */
#define DSP_ERESERVED_8             (DSP_EBASE + 0x20)

/* Reserved error code */
#define DSP_ERESERVED_9             (DSP_EBASE + 0x21)

/* Reserved error code */
#define DSP_ERESERVED_10            (DSP_EBASE + 0x22)

/* A requested resource is not available. */
#define DSP_ERESOURCE               (DSP_EBASE + 0x28)

/* A critical error has occurred, and the DSP is being re-started. */
#define DSP_ERESTART                (DSP_EBASE + 0x29)

/* A DSP memory free operation failed. */
#define DSP_EFREE                   (DSP_EBASE + 0x2A)

/* A DSP I/O free operation failed. */
#define DSP_EIOFREE                 (DSP_EBASE + 0x2B)

/* Multiple instances are not allowed. */
#define DSP_EMULINST                (DSP_EBASE + 0x2C)

/* A specified entity was not found. */
#define DSP_ENOTFOUND               (DSP_EBASE + 0x2D)

/* A DSP I/O resource is not available. */
#define DSP_EOUTOFIO                (DSP_EBASE + 0x2E)

/* Address Translation between ARM and DSP has failed */
#define DSP_ETRANSLATE              (DSP_EBASE + 0x2f)

/* File or section load write function failed to write to DSP */
#define DSP_EFWRITE                 (DSP_EBASE + 0x31)

/* Unable to find a named section in DSP executable */
#define DSP_ENOSECT                 (DSP_EBASE + 0x32)

/* Reserved error code */
#define DSP_ERESERVED_11            (DSP_EBASE + 0x37)

/* Reserved error code */
#define DSP_ERESERVED_12            (DSP_EBASE + 0x40)
#define DSP_ERESERVED_13            (DSP_EBASE + 0x41)
#define DSP_ERESERVED_14            (DSP_EBASE + 0x42)
#define DSP_ERESERVED_15            (DSP_EBASE + 0x43)
#define DSP_ERESERVED_16            (DSP_EBASE + 0x44)
#define DSP_ERESERVED_17            (DSP_EBASE + 0x45)
#define DSP_ERESERVED_18            (DSP_EBASE + 0x46)
#define DSP_ERESERVED_19            (DSP_EBASE + 0x47)
#define DSP_ERESERVED_20            (DSP_EBASE + 0x48)
#define DSP_ERESERVED_21            (DSP_EBASE + 0x49)
#define DSP_ERESERVED_22            (DSP_EBASE + 0x4a)
#define DSP_ERESERVED_23            (DSP_EBASE + 0x4b)
#define DSP_ERESERVED_24            (DSP_EBASE + 0x4c)
#define DSP_ERESERVED_25            (DSP_EBASE + 0x4d)
#define DSP_ERESERVED_26            (DSP_EBASE + 0x4e)
#define DSP_ERESERVED_27            (DSP_EBASE + 0x4f)

/* The connection requested by the client already exists */
#define DSP_EALREADYEXISTS          (DSP_EBASE + 0x50)

/* Timeout parameter was "NO_WAIT", yet the operation was
 * not completed.
 */
#define DSP_ENOTCOMPLETE            (DSP_EBASE + 0x51)

/* The DSP configuration does not match the GPP configuration. */
#define DSP_ECONFIG                 (DSP_EBASE + 0x52)

/* Feature is not supported */
#define DSP_ENOTSUPPORTED           (DSP_EBASE + 0x53)

/* Reserved error code */
#define DSP_ERESERVED_BASE_1        (DSP_COMP_EBASE + 0x000)
#define DSP_ERESERVED_BASE_2        (DSP_COMP_EBASE + 0x100)
#define DSP_ERESERVED_BASE_3        (DSP_COMP_EBASE + 0x200)
#define DSP_ERESERVED_BASE_4        (DSP_COMP_EBASE + 0x300)
#define DSP_ERESERVED_BASE_5        (DSP_COMP_EBASE + 0x400)


/* FAILURE Codes : CHNL */
#define CHNL_EBASE                  (DSP_COMP_EBASE + 0x500)

/* Attempt to create too many channels. */
#define CHNL_E_MAXCHANNELS          (CHNL_EBASE + 0x00)

/* Reserved error code */
#define CHNL_E_RESERVED_1           (CHNL_EBASE + 0x01)

/* No free channels are available. */
#define CHNL_E_OUTOFSTREAMS         (CHNL_EBASE + 0x02)

/* Channel ID is out of range. */
#define CHNL_E_BADCHANID            (CHNL_EBASE + 0x03)

/* Channel is already in use. */
#define CHNL_E_CHANBUSY             (CHNL_EBASE + 0x04)

/* Invalid channel mode argument. */
#define CHNL_E_BADMODE              (CHNL_EBASE + 0x05)

/* Timeout parameter was "NO_WAIT", yet no I/O completions
 * were queued.
 */
#define CHNL_E_NOIOC                (CHNL_EBASE + 0x06)

/* I/O has been cancelled on this channel. */
#define CHNL_E_CANCELLED            (CHNL_EBASE + 0x07)

/* End of stream was already requested on this output channel. */
#define CHNL_E_EOS                  (CHNL_EBASE + 0x09)

/* Unable to create the channel event object. */
#define CHNL_E_CREATEEVENT          (CHNL_EBASE + 0x0A)

/* Reserved error code */
#define CHNL_E_RESERVED_2           (CHNL_EBASE + 0x0B)

/* Reserved error code */
#define CHNL_E_RESERVED_3           (CHNL_EBASE + 0x0C)

/* DSP word size of zero configured for this device. */
#define CHNL_E_INVALIDWORDSIZE      (CHNL_EBASE + 0x0D)

/* Reserved error code */
#define CHNL_E_RESERVED_4           (CHNL_EBASE + 0x0E)

/* Reserved error code */
#define CHNL_E_RESERVED_5           (CHNL_EBASE + 0x0F)

/* Reserved error code */
#define CHNL_E_RESERVED_6           (CHNL_EBASE + 0x10)

/* Unable to plug channel ISR for configured IRQ. */
#define CHNL_E_ISR                  (CHNL_EBASE + 0x11)

/* No free I/O request packets are available. */
#define CHNL_E_NOIORPS              (CHNL_EBASE + 0x12)

/* Buffer size is larger than the size of physical channel. */
#define CHNL_E_BUFSIZE              (CHNL_EBASE + 0x13)

/* User cannot mark end of stream on an input channel. */
#define CHNL_E_NOEOS                (CHNL_EBASE + 0x14)

/* Wait for flush operation on an output channel timed out. */
#define CHNL_E_WAITTIMEOUT          (CHNL_EBASE + 0x15)

/* Reserved error code */
#define CHNL_E_RESERVED_7           (CHNL_EBASE + 0x16)

/* Reserved error code */
#define CHNL_E_RESERVED_8           (CHNL_EBASE + 0x17)

/* Unable to prepare buffer specified */
#define CHNL_E_RESERVED_9           (CHNL_EBASE + 0x18)

/* Unable to Unprepare buffer specified */
#define CHNL_E_RESERVED_10          (CHNL_EBASE + 0x19)

/* The operation failed because it was started from a wrong state */
#define CHNL_E_WRONGSTATE           (CHNL_EBASE + 0x1A)


/* FAILURE Codes : SYNC */
#define SYNC_EBASE                  (DSP_COMP_EBASE + 0x600)

/* Wait on a kernel event failed. */
#define SYNC_E_FAIL                 (SYNC_EBASE + 0x00)

/* Timeout expired while waiting for event to be signalled. */
#define SYNC_E_TIMEOUT              (SYNC_EBASE + 0x01)


/* Reserved error code */
#define DSP_ERESERVED_BASE_6        (DSP_COMP_EBASE + 0x700)
#define DSP_ERESERVED_BASE_7        (DSP_COMP_EBASE + 0x800)


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (ERRBASE_H) */
