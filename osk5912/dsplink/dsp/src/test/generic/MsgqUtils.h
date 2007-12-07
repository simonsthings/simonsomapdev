/** ============================================================================
 *  @file   MsgqUtils.h
 *
 *  @path   $(PROJROOT)\dsp\src\test\generic
 *
 *  @desc   Defines some utility functions for messaging testsuite.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  This software is copyright to Texas Instruments Incorporated.
 *  Its usage is governed by the terms and conditions stipulated in
 *  the license agreement under which this software has been supplied.
 *
 *  The recipient of this software implicitly accepts the terms of
 *  the license.
 *  ============================================================================
 */


#if !defined (MSGQUTILS_H)
#define MSGQUTILS_H


/*  ----------------------------------- DSP/BIOS Headers            */
#include <sem.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   SYS_EFAIL
 *
 *  @desc   Generic failure error for bios.
 *  ============================================================================
 */
#define SYS_EFAIL      (SYS_EUSER + 1)

/** ============================================================================
 *  @name   TST_MQABUF_ID/TST_MQTLOCAL_ID/TST_MQTDSPLINK_ID
 *
 *  @desc   Ids to be used.
 *  ============================================================================
 */
#define TST_MQABUF_ID      0
#define TST_MQTLOCAL_ID    0
#define TST_MQTDSPLINK_ID  1

/** ============================================================================
 *  @name   MAX_MSGQ_NAME_LEN
 *
 *  @desc   Maximum length of the MSGQ names.
 *  ============================================================================
 */
#define MAX_MSGQ_NAME_LEN  32

/** ============================================================================
 *  @name   TST_MSGQ_DATA_SIZE
 *
 *  @desc   Maximum length of data in the test msg.
 *  ============================================================================
 */
#define TST_MSGQ_DATA_SIZE   32

/** ============================================================================
 *  @name   TST_REPORT_SIZE
 *
 *  @desc   Size of the report buffer to be sent to GPP
 *  ============================================================================
 */
#define TST_REPORT_SIZE     ALIGN (2 / DSP_MAUSIZE)


/** ============================================================================
 *  @name   TestMsg
 *
 *  @desc   The buffer structure to be used for messaging.
 *
 *  @field  elem
 *              The header required by MSGQ implementation.
 *  @field  info
 *              A 16 bit info field used differently by different tests.
 *  @field  buffer
 *              buffer required for storing the messages esp. for
 *              data verification.
 *  ============================================================================
 */
typedef struct TestMsg_tag {
    MSGQ_MsgHeader  elem                        ;
    Uint16          info                        ;
    Uint16          buffer [TST_MSGQ_DATA_SIZE] ;
} TestMsg ;


/** ============================================================================
 *  @func   SetupMessaging
 *
 *  @desc   This function does the basic setup for using messaging on the DSP
 *          side. The setup involves opening the allocator, transport etc.
 *
 *  @arg    numMsgPerPool
 *              Number of messages per pool of the mqabuf allocator.
 *  @arg    maxMsgSize
 *              Maximum message size to be supported.
 *  @arg    numLocalMsgqs
 *              Maximum number of message queues to be created.
 *  @arg    numRemoteMsgqs
 *              Maximum number of message queues supported by the
 *              remote processor.
 *
 *  @ret    SYS_OK
 *              Operation Successfully completed.
 *          SYS_EFAIL
 *              The operation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    CleanupMessaging
 *  ============================================================================
 */
Uns
SetupMessaging (Uns numMsgPerPool,
                Uns maxMsgSize,
                Uns numLocalMsgqs,
                Uns numRemoteMsgqs) ;


/** ============================================================================
 *  @func   CleanupMessaging
 *
 *  @desc   This function does the cleanup of the initializations done during
 *          setup.
 *
 *  @arg    numLocalMsgqs
 *              Number of local message queues configured during setup.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SetupMessaging
 *  ============================================================================
 */
Void
CleanupMessaging (Uns numLocalMsgqs) ;


/** ============================================================================
 *  @func   GetDsplinkMsgqName
 *
 *  @desc   This function converts id into a string and concatinates it with
 *          the supplied prefix to form the Msgq name.
 *
 *  @arg    name
 *              Array for returning the name.
 *  @arg    prefix
 *              Prefix to be used for the name.
 *  @arg    msgq_id
 *              Number to be appended to prefix to form the name.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Void
GetDsplinkMsgqName(Char * name, Char * prefix, Uns msgq_id) ;


/** ============================================================================
 *  @func   BinarySemPend
 *
 *  @desc   Simulates binary semaphore in DSP/BIOS when there is only one
 *          reader
 *
 *  @arg    semHandle
 *              Handle to the semaphore.
 *  @arg    timeout
 *              Timeout for pending on the semaphore.
 *
 *  @ret    TRUE
 *              Semaphore successfully obtained.
 *          FALSE
 *              Failed to obtain semaphore.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Bool
BinarySemPend (SEM_Handle semHandle, Uns timeout) ;


/** ============================================================================
 *  @func   atoi
 *
 *  @desc   Converts string to integer value.
 *
 *  @arg    str
 *              Pointer to string.
 *
 *  @ret    integer value
 *              Converted integer value.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
extern
int
atoi (const char * str) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (MSGQUTILS_H) */
