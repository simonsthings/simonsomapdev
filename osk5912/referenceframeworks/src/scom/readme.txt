
TITLE
-----
SCOM (Synchronized COMmunication) module

USAGE
-----
RF5

DESCRIPTION
-----------
 *  Synchronized COMmunication module
 *  
 *  This module implements message passing between tasks. It lets the user
 *  create any number of named, synchronized queues, and put messages
 *  to such queues and receive messages from them. Messages are buffers
 *  of arbitrary sizes.  The message structure's first field must be 
 *  QUE_Elem.  For example:
 *  struct MyMsg {
 *      QUE_Elem  elem;
 *      Ptr       buffer;
 *  }
 *
 *  Usage example:
 *
 *  0. Tasks A and B agree that A sends messages of some type MyMsg to B,
 *     via an SCOM queue named "queueForB".
 *
 *  1. Some code (typically B's initialization part) creates the SCOM queue
 *     with a unique name:
 *     SCOM_Handle queueB = SCOM_create( "queueForB" );
 *     
 *     .---.                  .---.
 *     | A |        ========> | B |       
 *     `---'                  `---'
 *     
 *   
 *  2. A gets the reference to the SCOM queue by supplying its name:
 *     SCOM_Handle queueB = SCOM_open( "queueForB" );
 *     B can do the same if it didn't remember it from the call to 
 *     SCOM_create()
 *  
 *     .---.                  .---.
 *     | A |        ========> | B |       
 *     `---'         :   :    `---'
 *       :...........:   :......:
 *   
 *  3. Someone, typically sender task A, allocates a buffer of type MyMsg:
 *     MyMsg * msg = MEM_alloc(someMemSegId, sizeof( MyMsg ), 0 );
 *     The msg could also be static.
 *     
 *        .---->[  ]
 *        |
 *     .--+.                 .---.
 *     |A *|       ========> | B |       
 *     `---'         :   :   `---'
 *       :...........:   :......:
 *   
 *  4. A writes some information, p, into the message buffer:
 *  
 *        .---->[p ]
 *        |
 *     .--+.                 .---.
 *     |A *|       ========> | B |       
 *     `---'         :   :   `---'
 *       :...........:   :......:
 *   
 *  5. A sends the message to B by doing SCOM_putMsg( queueB, msg );
 *     There can be an unlimited number of messages waiting on an SCOM queue.
 *  
 *              [p ]<--.
 *                     |
 *     .---.           |     .---.
 *     | A |       ===(*)==> | B |       
 *     `---'         :   :   `---'
 *       :...........:   :......:
 *  
 *  
 *  6. B receives the message by saying, 
 *     MyMsg * msg = SCOM_getMsg( queueB, SYS_FOREVER ); SYS_FOREVER
 *     is a timeout value, indicating how long to wait until there's a message
 *     available on the SCOM queue. 0 as a timeout means "don't wait."
 *  
 *              [p ]<---------.
 *                            |
 *     .---.                 .+--.
 *     | A |       ========> |* B|       
 *     `---'         :   :   `---'
 *       :...........:   :......:
 *   
 *  7. If B wants to write something into the message buffer and send it back
 *     to A (i.e. if they want a two-way communication), we need an additional 
 *     queue for A, symetrical to the one for B. 

FILES
-----
- scom*.pjt: project files for building a library for the appropriate
      architecture
- scom.c: SCOM implementation
- ../include/scom.h: public header file for the SCOM module
- readme.txt: this file

NOTE
----
Files in the library are compiled with no optimization switches turned on,
and the same is true for the project files. If you plan to use this module 
in a product release, it is advised that you rebuild the library with 
optimization turned on.

Q&A
---
Q1: What modules are required to use the SCOM module?
Q2: Why must the first field of the msg structure be a QUE_Elem?

---
Q1: What non-BIOS modules are required to use the SCOM module?
A1: UTL module is needed if SCOM is built with non-zero UTL_DBGLEVEL.

---
Q2: Why must the first field of the msg structure be a QUE_Elem?
A2: Internally SCOM uses the QUE module.  The QUE_Elem is needed in a 
    well-known location (e.g. first field) to place the msg onto the queue.

