TITLE
-----
UTL (Utility module for debugging)

USAGE
-----
RF1, RF3, RF5

DESCRIPTION
-----------
UTL module is a set of macros and conditionally linked-in functions
that aid real-time debugging and analysis efforts.

FILES
-----
- utl*.pjt: project files for building a library for the appropriate
       architecture
- ../include/utl.h: UTL module header file
- utl_algMem.c: implementation of algorithm memory usage reporting function
- utl_defaultLog.c: function for setting default LOG objects
- utl_error.c: function for halting the target and reporting an error
- utl_heapUsage.c: function for reporting heap usage
- utl_sts*.c: implementation of UTL_sts* statistical time functions
- readme.txt: this file

Q&A
---
Q1: How do I use this module for debugging?

---
Q1: How do I use this module for debugging?
A1: All the API information can be found in the eXpressDSP Software Reference
    Frameworks API doc. Usage of the APIs is desribed in the particular
    RF # Application Notes.
    Also refer to ../include/utl.h header file: it is a documentation
    file as well as the source code. 
    
