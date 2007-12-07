TITLE
-----
ALGRF (ALGorithm instantiation for Reference Frameworks) module

USAGE
-----
RF3, RF5

DESCRIPTION
-----------
ALGRF is a module used for creation, deletion, activation, and deactivation
of XDAIS algorithms. It has the similar purpose as the ALG module included in
the standard XDAIS installation, except that the module is optimized for
DSP/BIOS usage.

FILES
-----
Each ALGRF function is placed in a separate file to avoid dead code, unless 
two or more functions are always dependent; in that case, they are placed
together in a single source file.

- algrf*.pjt: project files for building a library for the appropriate
      architecture
- algrf_activate.c: ALGRF_activate() function
- algrf_control.c:  ALGRF_control() function
- algrf_cre.c: ALGRF_create(), ALGRF_free() functions
- algrf_creScratchSupport.c: ALGRF_createScratchSupport() function,
  ALGRF_memFreeScratchSupport() function
- algrf_deactivate.c: ALGRF_deactivate() function
- algrf_delete.c: ALGRF_delete() function
- algrf_delScratchSupport.c ALGRF_deleteScratchSupport() function
- algrf_exit.c: ALGRF_exit() function
- algrf_init.c: ALGRF_init() function
- algrf_setup.c: ALGRF_setup() function
- ../include/algrf.h: public header file for the ALGRF module
- readme.txt: this file

NOTE
----
Files in the library are compiled with no optimization switches turned on,
and the same is true for the project files. If you plan to use this module 
in a product release, it is advised that you rebuild the library with 
optimization turned on.

Q&A
---
Q1: How is ALGRF different from ALG?
Q2: How do I set up ALGRF?
Q3: How do I instantiate an XDAIS algorithm using ALGRF?
Q4: What other ALGRF functions are available?
Q5: Where can I find more information?

---
Q1: How is ALGRF different from ALG?
A1: The ALGRF library module is a set of XDAIS algorithm instantiation 
    procedures for Reference Frameworks. It is functionally similar to the ALG 
    module included in the XDAIS installation directory; the module has been 
    named "ALGRF" instead of "ALG" to avoid name conflicts, and the 
    corresponding ALG_ function name prefixes have been changed to ALGRF_.

    The ALGRF module differs from the ALG module in its implementation of
    memory management, in that it uses the MEM module from DSP/BIOS instead of 
    the standard malloc(), as the former provides greater control and 
    flexibility in managing memory heaps. Functions from the MEM module 
    (MEM_alloc(), MEM_free() et al.) always operate over a specific DSP/BIOS 
    heap segment, as defined in the configuration tool, so the "heap segment 
    identifier" information has to be communicated to the ALGRF module in its 
    initialization phase. This information is used by the ALGRF module to 
    determine exactly which DSP/BIOS MEM heap should be used for a specific 
    algorithm's memory needs.

    Using the ALGRF module consists of a setup call (once in the application
    lifetime), algorithm instantiation calls (once per algorithm instance),
    and optionally deleting/moving/controlling algorithms etc. Setup must
    be performed before any other ALGRF function is called.

---
Q2: How do I set up ALGRF?
A2: Heap segment identifier in DSP/BIOS is an integer variable, defined 
    in files automatically generated from the configuration database, 
    and it uniquely identifies the heap segment within a memory section.
    For instance, if the segment IDATA has an unnamed heap of a certain size, 
    then this heap can be referred to by IDATA, where IDATA must be declared 
    by the user as 
    	extern Int IDATA;
    If the memory segment's heap has a name assigned by the user, an integer 
    variable with that name identifies the heap. If the heap is named, that
    name should be used instead.

    Heap segment identifiers are passed to the ALGRF module via 
    the ALGRF_setup() procedure call:

    Void ALGRF_setup( Int internalHeap, Int externalHeap );

    Parameters internalHeap, externalHeap are heap segment
    identifiers for internal heap, external heap.
    Internal and/or external heaps are used by XDAIS algorithms, as indicated
    in their specification.

    A typical ALGRF setup, in a system that has a heap named INTERNALHEAP in 
    internal memory, and a heap named EXTERNALHEAP in external memory, would be

    	extern Int INTERNALHEAP;
    	extern Int EXTERNALHEAP;
    	...
    	ALGRF_setup( INTERNALHEAP, EXTERNALHEAP );
	
    In the example, dynamic heap INTERNALHEAP is used for internal memory for 
    the algorithms, and EXTERNALHEAP is used for external memory for the 
    algorithms.
		
    An example of an ALGRF setup in a system where only internal memory exists, 
    but where the application uses XDAIS algorithms that may ask for external
    memory, would be
	
	    extern Int INTERNALHEAP;
	    ...
	    ALGRF_setup( INTERNALHEAP, INTERNALHEAP );

    The above setting will result in the algorithm instance always having
    its memory blocks allocated in internal memory, regardless of whether
    it asks for external memory or not.

---
Q3: How do I instantiate an XDAIS algorithm using ALGRF?
A3: Function ALGRF_create() is used for creating a new instance of an XDAIS
    algorithm where scratch memory, if any, is *not* reused among algorithms:

    ALGRF_Handle ALGRF_create( IALG_Fxns *fxns, IALG_Handle parent, 
			     IALG_Params *params );

    Parameters fxns, parent (usually NULL), and params are the standard
    instance creation parameters as used in the ALG_create function, the
    difference being that ALGRF_create() returns ALGRF_Handle as the type
    of the handle of the created instance; the value is the handle of the
    instance or NULL if the creation failed.

    Type ALGRF_Handle is a generic XDAIS algorithm handle type, equivalent
    to the ALG_Handle type in the standard ALG module. 

    Typically one might use wrappers for creating instances, as in

    static inline FIR_Handle FIR_create( IFIR_Fxns *fxns, FIR_Params *prms )
    {
        return ( (FIR_Handle)
            ALGRF_create((IALG_Fxns *)fxns, NULL, (IALG_Params *)prms) );
    }

    where the generic handle is cast to the algorithm specific handle.

    For applications in which two or more algorithms can reuse the same
    scratch buffer, the user must calculate the max combined scratch size 
    needed by the 'worst-case' algorithm, allocate the buffer
    of that size, and provide the buffer address and size as parameters to 
    ALGRF_createScratchSupport() function. 

    (combined scratch size refers to a situation in which an algorithm instance
    needs more than one scratch buffer, so if an algorithm instance needs one 
    scratch buffer of size s1 and one of size s2, the combined scratch size 
    needs for that instance is s1 + s2; Alignment is also a factor in that 
    equation and is covered below. "largest" refers to the maximum of all
    scratch size needs across all the algorithms sharing the buffer). The
    scratch buffer is allocated by the user and passed as the argument to 
    ALGRF_createScratchSupport(). 

    The user must ensure that standard conditions for reusing scratch buffers
    are met (non-preemption among the algorithm instances using the same
    scratch buffer etc.)

    Alignment must also be catered for with internal, scratch requests. e.g.

    memTab[3].size = 50;
    memTab[3].alignment = 2;

    memTab[5].size = 100;
    memTab[5].alignment = 32;

    scratchBuf scratchSize to be passed to ALGRF_createScratchSupport() ==
    50 + 2 + 100 + 32 = 184. 

    This keeps it simple.

    Signature is :- 

    ALGRF_Handle ALGRF_createScratchSupport( IALG_Fxns *fxns,
        IALG_Handle parent, IALG_Params *params, Void *scratchBuf,
        Int scratchSize );

    Example usage :-

    	firHandleA = (IFIR_Handle)ALGRF_createScratchSupport(
		    (IALG_Fxns *)&FIR_TI_IFIR, NULL, ((IALG_Params *)&firParams), 
		    scratchBuffer, sizeof( scratchBuffer ) );

    This will perform :-
    * MEM dynamic allocation of IALG_PERSIST with External/Internal space 
    * MEM dynamic allocation of IALG_SCRATCH with External space 
    * Simple, pointer mapping for IALG_SCRATCH Internal bufs to handle alignment
      (ie no dynamic memory is allocated for IALG_SCRATCH, Internal case - 
      instead it is passed in by the application to this function with sufficient 
      size)

    If scratchSize passed in is not large enough NULL is returned, indicating
    failure.

---
Q4: What other ALGRF functions are available?
A4: Function ALGRF_control() invokes the generic control function for the
    instance: 

    Int ALGRF_control( ALGRF_Handle alg, IALG_Cmd cmd, IALG_Status *statusPtr );

    The remaining ALGRF functions are:

    - Bool ALGRF_delete( ALGRF_Handle instalg );
      destroys the instance and frees its memory
  
    For instances using scratch memory the functions are:

    - Bool ALGRF_deleteScratchSupport(ALGRF_Handle instalg);

    - Void ALGRF_activate(ALGRF_Handle alg);
      initializes scratch memory buffers before processing
  
    - extern Void ALGRF_deactivate(ALGRF_Handle alg);
      saves all persistent data to non-scratch memory

    There are other internal functions in ALGRF. Some are prefixed by ALGRF_.
    This is simply because more than 1 file references the function hence
    it is in global namespace and must be named to avoid conflicts. It is not
    intended to be called by the user directly however. Local functions are 
    static.
 
---
Q5: Where can I find more information?
A5: All the API information can be found in the eXpressDSP Software Reference
    Frameworks API doc. 
