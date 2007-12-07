TITLE
-----
XDAIS FIR algorithm 

USAGE
-----
RF1, RF3, RF5

DESCRIPTION
-----------
The FIR module is a FIR filter implemented as an XDAIS algorithm.

FILES
-----
- fir*.pjt: project files for building a library for the appropriate
       architecture
- fir_ti.c: implementation of FIR_TI_init,exit() functions
- fir_ti_filter: implementation of the filtering function
- fir_ti_ialg.c: implementation of IALG functions for the algorithm       
- fir_ti_vt.c: definition of the virtual table for the algorithm
- fir_ti_priv.h: declaration of private algorithm object structure
- ../include/fir_ti.h: declaration of FIR filter function names
- ../include/ifir.h: FIR interface definition
- readMe.txt: this file

NOTE
----
Files in the library are compiled with -o2 optimization switch turned on.

Q&A
---
Q1: Where can I find documentation on FIR implementation?

---
Q1: Where can I find documentation on FIR implementation?
A1: SPRU360 (XDAIS API reference), Example Algorithm Implementation.
