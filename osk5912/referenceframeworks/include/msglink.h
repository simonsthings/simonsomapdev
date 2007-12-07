/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== msglink.h ========
 *  This is the public include file for the msglink module.
 */


#ifndef MSGLINK_
#define MSGLINK_

#ifdef __cplusplus
extern "C" {
#endif

/* Number of buffers for input and output */
#define MSGLINK_NUMBUFS 3

/* Used to denote whether the message link is an input or output link */
typedef enum MSGLINK_Mode {
    MSGLINK_INPUT,
    MSGLINK_OUTPUT
} MSGLINK_Mode;

/* Return Codes */
#define MSGLINK_SUCCESS  0
#define MSGLINK_FAILURE -1

/*
 *  ======== MSGLINK_init ========
 *  
 *  Initialize the module.
 */
Void MSGLINK_init( Void );

/*
 *  ======== MSGLINK_exit ========
 *
 *  Finalize the module.
 */
Void MSGLINK_exit( Void );

/*
 *  ======== MSGLINK_create ========
 *  
 *  Create a new message link.
 */
Int MSGLINK_create(
       MSGLINK_Mode msglinkMode, /* Whether to use input or output channel   */
       MdUns        msgSize,     /* Size of the msgs to send on the channel  */
       Uns          timeout      /* Timeout for recv (input) or send (output)*/
       );

/*
 *  ======== MSGLINK_delete ========
 *  
 *  Delete the message link
 */
Int MSGLINK_delete( 
       MSGLINK_Mode msglinkMode  /* Whether to use input or output channel */
       );

/*
 *  ======== MSGLINK_send ========
 *  
 *  Send a message.
 */
Int MSGLINK_send( Ptr msg ); /* Pointer to message to send */

/*
 *  ======== MSGLINK_recv ========
 *  
 *  Receive a message.
 */
Int MSGLINK_recv( Ptr msg ); /* Pointer to buffer to recv into */

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MSGLINK_ */

