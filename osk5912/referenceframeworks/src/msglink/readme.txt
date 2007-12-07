
TITLE
-----
MSGLINK (Messaging on DSP Link) module

USAGE
-----
RF6

DESCRIPTION
-----------
 *  MSGLINK module
 *  
 *  This module implements message passing between the GPP and the DSP via
 *  DSP Link Channels.  The intent of this module is to be temporary. Once
 *  messaging is implemented in DSP Link, RF6 will use it instead of the
 *  MSGLINK module.
 *
 *  Since this module is temporary, it was purposely kept simple and easy
 *  to replace.  The rest of the readme.txt will have the perspective from 
 *  the DSP.
 *
 *  The underlying transport mechanism is DSPLink Channels (CHNL on GPP,
 *  SIO on DSP).  The MSGLINK module was one input and one output message
 *  link. It uses DSP/BIOS Link channels 14 and 15. Since the MSG links are 
 *  based upon DSP/BIOS Link channels, they are uni-directional.
 *
 *  There are also a fixed number of messages in the module.  The 
 *  MSGLINK_NUMBUFS constant specifies how many messages each of the two links
 *  has.  Note: the messages are small, so not a large use of memory.
 *
 *  The MSGLINK module also does a copy of the message.  Messages are generally
 *  small, therefore the copy does not have a significant performance impact.  
 *
 *  As mentioned above, there are two message links in the module. The 
 *  following enum is used to specify which one is being used.
 *
 *      typedef enum MSGLINK_Mode {
 *          MSGLINK_INPUT,
 *          MSGLINK_OUTPUT
 *      } MSGLINK_Mode;
 *
 *
 * There are 6 APIs (note: same APIs are the DSP side).
 *
 * Void MSGLINK_init( Void ): Called at initialization.
 *
 * Void MSGLINK_exit( Void ): Called at shutdown.
 *
 * Int  MSGLINK_create( MSGLINK_Mode msgId, LgUns msgSize, Uns timeout ): 
 *                                           Specify input or 
 *                                           output, size of the messages and
 *                                           the timeout on the  send and 
 *                                           recv. Should only call for each 
 *                                           message link. 
 *
 * Int  MSGLINK_delete( MSGLINK_Mode msgId ): Specify input or output
 *
 * Int  MSGLINK_send( Ptr msg ): Always uses output message link. Simply 
 *                               pass the ptr to the message.
 *
 * Int  MSGLINK_recv( Ptr msg ): Always uses input message link. 
 *                               Simply pass a pointer to an empty message
 *                                 that will get filled in.


FILES
-----
- makefile: makefile for building a library 
- msglink.c: MSGLINK implementation
- ../include/msglink.h: public header file for the MSGLINK module
- readme.txt: this file

NOTE
----
Files in the library are compiled with no optimization switches turned on,
and the same is true for the project files. If you plan to use this module 
in a product release, it is advised that you rebuild the library with 
optimization turned on.

Q&A
---
Q1: Why are there just 2 message links?
Q2: Do the message sizes on the DSP input and GPP output have to match?
Q3: Do the message sizes on the GPP input and DSP output have to match?
Q4: Can the input and output links on the GPP have different size messages?
Q5: Can the input and output links on the DSP have different size messages?
Q6: Is both MSGLINK_send and MSGLINK_recv blocking?
Q7: Is the MSGLINK_send thread-safe? MSGLINK_recv?
Q8: Does MSGLINK handle endian conversions?
Q9: Can a SWI use MSGLINK?
Q10:How do you wait forever on sending and receiving messages?

---
Q1: Why are there just 2 message links?
A1: To make it simple.

---
Q2: Do the message sizes on the DSP input and GPP output have to match?
A2: Yes. The size of the message specified on the creation of the GPP 
    output link must match exactly the size specified in the creation of the
    DSP input link.  

---
Q3: Do the message sizes on the GPP input and DSP output have to match?
A3: Yes. Same logic as Q2. 

---
Q4: Can the input and output links on the GPP have different size messages?
A4: Yes. The GPP -> DSP message size can be different from the DSP -> GPP 
    message size.

---
Q5: Can the input and output links on the DSP have different size messages?
A5: Yes. Same logic as Q4.

---
Q6: Is both MSGLINK_send and MSGLINK_recv blocking?
A6: Both send and recv are blocking if you specify a non-zero timeout on the 
    create.  Underneath they are doing a CHNL_Reclaim(), which blocks when
    the timeout is non-zero.

---
Q7: Is the MSGLINK_send thread-safe? MSGLINK_recv?
A7: The APIs are not thread-safe.  There should only be one thread make a 
    call into MSGLINK_send() at a time.  Similarly for MSGLINK_recv().

---
Q8: Does MSGLINK handle endian conversions?
A8: No!  This is left to the application.

---
Q9: Can a SWI use MSGLINK?
A9: Yes, but only with timeout zero.

---
Q10:How do you wait forever on sending and receiving messages?
A10:Use the DSP/BIOS constant SYS_FOREVER in the MSGLINK_create() call.

