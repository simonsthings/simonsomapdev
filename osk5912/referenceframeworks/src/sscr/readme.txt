
TITLE
-----
SSCR (Shared SCRatch) module

USAGE
-----
RF5

DESCRIPTION
-----------
SSCR is a module to manage the overlaying of XDAIS algorithm's on-chip scratch
memory.

FILES
-----
Each SSCR function is placed in a separate file to avoid dead code, unless 
two or more functions are always dependent; in that case, they are placed
together in a single source file.

- sscr*.pjt: project files for building a library for the appropriate
      architecture
- sscr.c: SSCR private global variables
- sscr.h: SSCR header file
- sscr_createbuf.c: SSCR_createBuf() function
- sscr_deletebuf.c: SSCR_deleteBuf() function
- sscr_exit.c: SSCR_exit() function
- sscr_getbuf.c: SSCR_getBuf() function
- sscr_init.c: SSCR_init() function
- sscr_prime.c: SSCR_prime() function
- sscr_setup.c: SSCR_setup() function
- ../include/sscr.h: public header file for the SSCR module
- readme.txt: this file

NOTE
----
Files in the library are compiled with no optimization switches turned on,
and the same is true for the project files. If you plan to use this module 
in a product release, it is advised that you rebuild the library with 
optimization turned on.

The UTL module is used by the SSCR module.  UTL_assert is used to check 
parameters passed into the various functions.  This is to aid in debugging.
For your production code, it is advised that you rebuild the library with 
UTL_assert turned off.

Q&A
---
Q1: What memory is being overlaid?
Q2: How can the scratch memory be overlaid?
Q3: What is a SSCR_bucket?
Q4: Why must SSCR_prime() be called before SSCR_createBuf()?
Q5: Is the SSCR module thread-safe?
Q6: Why would I want to pre-define the buckets in SSCR_setup?

---
Q1: What memory is being overlaid?
A1: XDAIS algorithms request memory from the application. This is done to
    remove memory management from the algorithm.  
    Scratch memory is memory that is freely used by an algorithm without regard
    to its prior contents, i.e. no assumptions about the content can be made 
    by the algorithm and the algorithm is free to leave it in any state.
    Thus, multiple algorithm can share the same scratch memory.  

---
Q2: How can the scratch memory be overlaid?    
A2: Given that an algorithm cannot block during the execution of a runtime 
    execution function, scratch memory can be shared among other algorithms.
    There is one major caveat with this: since BIOS is a preemptive OS (one 
    thread can interrupt another thread), special measures must be taken to 
    prevent corruption of the scratch memory. 
    In BIOS, only threads of a higher priority can preempt a thread, therefore
    a thread of an equal or lower priority cannot preempt a thread. So scratch
    memory can safely be shared among all thread of the same priority.  This is
    called the "by priority" scratch overlaying method. An alternate method of
    scratch overlaying is to have one large scratch buffer that all algorithms 
    use. Locking mechanisms (e.g. semaphores, HWI_disable/restore, etc.) must be
    used to protect against preemption. This is called the "single" scratch 
    overlaying method.
    The "by priority" method has less latency issues, but results in a larger 
    total scratch buffer allocation. The "single" scratch method minimizes 
    memory allocation, but increases latency during the exection of the 
    algorithms.  All applications are different and it is up to the integrator 
    to determine which method or combination is best for them.

---
Q3: What is a SSCR_bucket?
A3: In the "by priority" scratch overlaying method, all algorithms with the 
    same priority will share the same scratch buffer.  This scratch buffer is
    maintained in a SSCR_bucket.  The SSCR_bucket contains the pointer to the
    buffer, size of the buffer and the number of algorithms using the buffer. 

---
Q4: Why must SSCR_prime() be called before SSCR_createBuf()?
A4: The SSCR_prime() does not allocate anything.  It simply determines the
    worst case scratch requirements.  Therefore, SSCR_prime() must be called
    on all algorithms in the system first.  Then SSCR_createBuf() can be called
    to actually allocate the memory.

---
Q5: Is the SSCR module thread-safe?
A5: Yes.  All APIs are thread-safe within the box of the API calling
    sequence specified below.  
                                                  -----------
                                                  |         |
                                                  v         |
    --------------     --------------     ----------------  |
    | SSCR_init  | --> | SSCR_setup | --> | SSCR_prime   |---
    --------------     --------------     ----------------
                                                  |
                                                  |  ----------
                                                  |  |        |
                                                  v  v        |
                                          ------------------   |
                                          | SSCR_createBuf@|----
                                          | SSCR_getBuf    |
                                          | SSCR_deleteBuf@|
                                          ----------------   
                                                  |  
                                                  |  
                                                  v  
                                          ----------------
                                          | SSCR_exit    |
                                          ----------------   
   Additional notes
     @ = must have created a scratch buffer before you can delete it. 

---
Q6: Why would I want to pre-define the buckets in SSCR_setup?
A6: In a dynamic system, the initial configuration might not be the 
    worst-case requirements.  With allowing the integrator to define
    the bucket sizes and supply the buffers, the addition of new algorithms
    during runtime is easier.

