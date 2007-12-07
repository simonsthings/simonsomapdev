
TITLE
-----
CHAN (CHANnel) module

USAGE
-----
RF5

DESCRIPTION
-----------
CHAN is a module to manage the serial execution of XDAIS algorithms.

FILES
-----
Each CHAN function is placed in a separate file to avoid dead code, unless 
two or more functions are always dependent; in that case, they are placed
together in a single source file.

- chan*.pjt: project files for building a library for the appropriate
      architecture
- chan.c: CHAN private global variables
- chan_attrs.c: CHAN_getAttrs() and CHAN_setAttrs() function
- chan_close.c: CHAN_close() function
- chan_create.c: CHAN_create() function
- chan_delete.c: CHAN_delete() function
- chan_execute.c: CHAN_execute() function
- chan_exit.c: CHAN_exit() function
- chan_init.c: CHAN_init() function
- chan_regcell.c: CHAN_regCell() function
- chan_open.c: CHAN_open() function
- chan_setup.c: CHAN_setup() function
- chan_unregcell.c: CHAN_unregCell() function
- ../include/chan.h: public header file for the CHAN module
- readme.txt: this file

NOTE
----
Files in the library are compiled with no optimization switches turned on,
and the same is true for the project files. If you plan to use this module 
in a product release, it is advised that you rebuild the library with 
optimization turned on.

Q&A
---
Q1: What modules are required to use the CHAN module?
Q2: Is the CHAN module thread-safe?

---
Q1: What modules are required to use the CHAN module?
A1: The CHAN module is built on top of the ALGRF, ICC and SSCR modules. It
    also uses the ICELL interface. 

---
Q2: Is the CHAN module thread-safe?
A2: Yes.  All APIs are thread-safe within the box of the API calling
    sequence specified below.
                                                  -----------
                                                  |         |
                                                  v         |
    --------------     --------------     ----------------  |
    | CHAN_init  | --> | CHAN_setup | --> | CHAN_regCell |---
    --------------     --------------     ----------------
                                                  |
                                                  |  ----------
                                                  |  |        |
                                                  v  v        |
                                          -----------------   |
                                          | CHAN_create@  |----
                                          | CHAN_open#    |
                                          | CHAN_getAttrs |
                                          | CHAN_setAttrs |
                                          | CHAN_execute  |
                                          | CHAN_close#   |
                                          | CHAN_delete@  |
                                          ----------------   
                                                  |  
                                                  |   -------   
                                                  |   |     |
                                                  v   v     |
                       --------------     ----------------  |
                       | CHAN_exit  | <-- |CHAN_unregCell|---
                       --------------     ----------------
   Additional notes
     # = must have opened a channel before you can close it. 
     @ = must have created a channel before you can delete it. Create/delete
         are not required if channel statically defined.

