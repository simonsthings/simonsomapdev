
TITLE
-----
ICC (Inter-Cell Communication) module

USAGE
-----
RF5

DESCRIPTION
-----------
The ICC module is a mechanism to move input and output data from a cell.


FILES
-----
Each ICC function is placed in a separate file to avoid dead code, unless 
two or more functions are always dependent; in that case, they are placed
together in a single source file.

- icc*.pjt: project files for building a library for the appropriate
      architecture
- icc.c: global variables
- icc_linearCreate.c: ICC_linearCreate() function
- icc_linearDelete.c: ICC_linearDelete() function
- ../include/icc.h: public header file for the icc module
- readme.txt: this file

NOTE
----
Files in the library are compiled with no optimization switches turned on,
and the same is true for the project files. If you plan to use this module 
in a product release, it is advised that you rebuild the library with 
optimization turned on.

Q&A
---
Q1: Are ICC's require in RF5?
Q2: Can the type of ICCs be expanded?

---
Q1: Are ICC's require in RF5?
A1: Yes.  ICC's are the mechansim to passed input and output buffers into a 
    cell. The other option was to have CHAN_execute() pass in these input and
    output buffers. That approach has several limitations which are not present
    with ICCs.

---
Q2: Can the type of ICCs be expanded?
A2: Yes.  There are place-holders to allow additional types of ICC objects.
    Currently there is only linear buffer supported (i.e. raw buffer).


