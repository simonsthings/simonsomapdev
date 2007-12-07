TITLE
-----
XDAIS VOL algorithm 

USAGE
-----
RF3, RF5

DESCRIPTION
-----------
The VOL module is a volume changing XDAIS algorithm. It takes an input
buffer, and copies all the samples amplified by speicifed percentage gain 
to the output buffer.

FILES
-----
- vol*.pjt: project files for building a library for the appropriate
       architecture
- vol_ti.c: implementation of VOL_TI_init,exit() functions
- vol_ti_ivol: implementation of the volume changing function
- vol_ti_ialg.c: implementation of IALG functions for the algorithm       
- vol_ti_vt.c: definition of the virtual table for the algorithm
- vol_ti_priv.h: declaration of private algorithm object structure
- ../include/vol_ti.h: declaration of VOL filter function names
- ../include/ivol.h: VOL interface definition
- readMe.txt: this file

NOTE
----
Files in the library are compiled with -o2 optimization switch turned on.

Q&A
---
Q1: Is there any value in using this algorithm in my application?
A1: No. But the local function scale() in vol_ti_ivol.c shows how
    16-bit word samples volume can be changed using a percentage 
    ("100" meaning no change in volume) on any platform.
