TITLE
-----
ALGMIN (ALGorithm instantiation for MINimal, static applications) module

USAGE
-----
RF1

DESCRIPTION
-----------
ALGMIN is a module used for instantiation, activation, and deactivation
of "static" XDAIS algorithms, i.e. those whose memory requrements are
determined at compile time and all the buffers statically allocated.

FILES
-----
Each ALGMIN function is placed in a separate file to avoid dead code, unless 
two or more functions are always dependent; in that case, they are placed
together in a single source file.

- algmin*.pjt: project files for building a library for the appropriate
      architecture
- algmin_new.c: ALGMIN_new() function
- algmin_activate.c: ALGMIN_activate() function
- algmin_exit.c: ALGMIN_exit() function
- algmin_init.c: ALGMIN_init() function
- ../include/algmin.h: public header file for the ALGMIN module
- readMe.txt: this file

NOTE
----
Files in the library are compiled with no optimization switches turned on,
and the same is true for the project files. If you plan to use this module 
in a product release, it is advised that you rebuild the library with 
optimization turned on.

Q&A
---
Q1: How is ALGMIN different from ALG or ALGRF?
Q2: How do I use ALGMIN?

---
Q1: How is ALGMIN different from ALG or ALGRF?
A1: The ALGMIN library module is a set of XDAIS algorithm instantiation 
    procedures for compact, static applications. 

    It is *NOT* functionally equivalent to the ALG or ALGRF modules.

    ALGMIN's key function is the ALGMIN_new function. All other functions are 
    simply for module completeness.

    ALGMIN_new() instantiates a 'static' XDAIS algorithm. What does this mean?
    It means that instead of 'creating' an algorithm via dynamic heap based 
    allocation of the algorithm's requested buffers, it simply initializes it 
    to work with pre-configured buffers. These buffers are statically
    preconfigured a-priori to be of the alignment, size, etc specified by the 
    algorithm.

    By avoiding heap-based allocation and buffer creation we save a great deal
    of codespace for small, compact, static applications.

---
Q2: How do I use ALGMIN?
A2: Example usage of ALGMIN_new (the key function)

    (a) In the main application we might do :-

    Char *firChanBufs[] = { firChanBufId00, firChanBufId01, firChanBufId02Scr };

    /* Statically create a FIR instance ie no heaps etc */
    firHandle0 = FIR_new(&FIR_IFIR, FIR_chanParamPtrs[0], 
   		    firChanBufs, (sizeof(firChanBufs) / sizeof(firChanBufs[0])));
   
    The declaration of firChanBufs could be stack-based since its lifetime
    need only be until completion of FIR_new()

    The contents of firChanBufs are *all* of the statically configured 
    buffers (the number of which will match the number of memTab's).

    The last parameter uses the sizeof operator to guarentee that the number
    of elements passed is indeed the number of elements in firChanBufs.

    (b) in firapp.h we static inline FIR_new to map to ALGMIN_new

    static inline FIR_Handle FIR_new(const IFIR_Fxns *fxns, 
    	const FIR_Params *params, Char *firChanBufs[], 
    	SmUns firNumChanBufs)
    {
        return ( (FIR_Handle)ALGMIN_new((IALG_Fxns *)fxns, 
    	    (IALG_Params *)params, firChanBufs, firNumChanBufs) );
    }

    This procedure can be repeated for N algorithms.

    Note that for each new algorithm, no additional codespace will be
    consumed since all new algorithms will map to use ALGMIN_new.
