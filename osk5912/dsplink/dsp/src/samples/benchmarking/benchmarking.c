/** ============================================================================
 *  @file   benchmarking.c
 *
 *  @path   $(PROJROOT)\dsp\src\samples\benchmarking
 *
 *  @desc   Implemetation of Benchmarking application.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  This software is copyright to Texas Instruments Incorporated.
 *  Its usage is governed by the terms and conditions stipulated in
 *  the license agreement under which this software has been supplied.
 *
 *  The recipient of this software implicitly accepts the terms of
 *  the license.
 *  ============================================================================
 */


#include <std.h>
#include <sio.h>
#include <tsk.h>
#include <clk.h>
#include <sem.h>
#include <log.h>
#include <benchmarking.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   bufSize
 *
 *  @desc   Size of the buffer to be used.
 *  ============================================================================
 */
static
Uns
bufSize ;


/** ============================================================================
 *  @name   numTransfers
 *
 *  @desc   Number of times the data transfer is done.
 *  ============================================================================
 */
static
Uns
numTransfers ;


/*  ============================================================================
 *  @name   CCS_BREAKPOINT
 *
 *  @desc   If USE_CCS_BREAKPOINT is defined the breakpoint instruction
 *          specific to CCS is used.
 *  ============================================================================
 */
#if (defined(USE_CCS_BREAKPOINT) && defined( _55_ ))

#define CCS_BREAKPOINT  \
    asm(" .if (.MNEMONIC)\n" \
        " ESTOP_1\n"     \
        " .else\n"           \
        " ESTOP_1()\n"   \
        " .endif\n"          \
        " NOP")

#endif /* (defined(USE_CCS_BREAKPOINT) && defined( _55_ )) */


/** ============================================================================
 *  @name   trace
 *
 *  @desc   This is the trace object used for writing debug strings
 *          and error messages.
 *  ============================================================================
 */
extern
LOG_Obj
trace ;

/** ============================================================================
 *  @name   input
 *
 *  @desc   This is the input stream which is opened for receiving data
 *          from GPP.
 *  ============================================================================
 */
static
SIO_Handle
input ;

/** ============================================================================
 *  @name   buf
 *
 *  @desc   Pointer to the buffer used for receiving data from GPP.
 *  ============================================================================
 */
static
Uns *
buf = NULL ;

/** ============================================================================
 *  @name   STScounter
 *
 *  @desc   Static Statistics object defined in the configuration file.
 *  ============================================================================
 */
extern
STS_Obj
STScounter ;

/** ============================================================================
 *  @name   frameRateSem
 *
 *  @desc   This is the handle to the semaphore which is used to maintain the
 *          frame rate.
 *  ============================================================================
 */
static
SEM_Handle
frameRateSem ;

/** ============================================================================
 *  @name   BenchmarkObject
 *
 *  @desc   Object used for benchmarking
 *  ============================================================================
 */
typedef struct BenchmarkObject {
    STS_Obj *   sts ;        /* Pointer to STS object defined in TCF file */
    LgUns       lastTime ;   /* Last timestamp on the object              */
    LgUns       count ;      /* Number of times the object was updated    */
} BenchmarkObject ;

/** ============================================================================
 *  @name   benchmarkObj
 *
 *  @desc   Object used for benchmarking
 *  ============================================================================
 */
static
BenchmarkObject
benchmarkObj ;


/** ============================================================================
 *  @func   UpdateBenchmarkObject
 *
 *  @desc   This function measures time difference between two calls and also
 *          returns the same.
 *
 *  @arg    obj
 *              Benchmarking object for measuring time difference between
 *              two calls.
 *
 *  @ret    Delta time between each invocation of this function.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
static
LgUns
UpdateBenchmarkObject (BenchmarkObject * obj) ;


/** ============================================================================
 *  @func   Benchmarking_Create
 *
 *  @desc   This function allocates the resources needed by the Benchmarking
 *          test.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    Benchmarking_Execute, Benchmarking_Delete
 *  ============================================================================
 */
static
Uns
Benchmarking_Create () ;


/** ============================================================================
 *  @func   Benchmarking_Execute
 *
 *  @desc   This function does the actual data transfer with the GPP.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    Benchmarking_Delete, Benchmarking_Create
 *  ============================================================================
 */
static
Uns
Benchmarking_Execute () ;


/** ============================================================================
 *  @func   Benchmarking_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the Benchmarking_Create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    Benchmarking_Execute, Benchmarking_Create
 *  ============================================================================
 */
static
Void
Benchmarking_Delete () ;


/** ============================================================================
 *  @func   Benchmarking
 *
 *  @desc   This function receives buffers from the gpp side.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
Benchmarking ()
{
    Uns       status = SYS_OK ;

    status = Benchmarking_Create () ;

    if (status == SYS_OK) {
        status = Benchmarking_Execute () ;
    }

    Benchmarking_Delete () ;
}


/** ============================================================================
 *  @func   Benchmarking_Create
 *
 *  @desc   This function allocates the resources needed by the Benchmarking
 *          test.
 *
 *  @modif  input, output
 *  ============================================================================
 */
static
Uns
Benchmarking_Create ()
{
    Uns       status = SYS_OK ;
    SIO_Attrs attrs           ;
    SEM_Attrs semAttrs        ;

    attrs         = SIO_ATTRS        ;
    attrs.nbufs   = 1                ;
    attrs.segid   = 0                ;
    attrs.align   = 0                ;
    attrs.flush   = TRUE             ;
    attrs.model   = SIO_ISSUERECLAIM ;
    attrs.timeout = SYS_FOREVER      ;

    benchmarkObj.sts      = &STScounter ;
    benchmarkObj.lastTime = 0 ;
    benchmarkObj.count    = 0 ;

    input = SIO_create (INPUTDATACHANNEL, SIO_INPUT, bufSize, &attrs) ;
    if (input == NULL) {
       status = SYS_EALLOC ;
       LOG_printf (&trace, "Could not create input stream") ;
    }

    if (status == SYS_OK) {
        buf = MEM_calloc (0, bufSize, 0) ;
    }

    if (buf == NULL) {
       status = SYS_EALLOC ;
       LOG_printf (&trace, "Could not allocate buffer for data transfer") ;
    }

    if (status == SYS_OK) {
        semAttrs = SEM_ATTRS ;
        frameRateSem = SEM_create (0, &semAttrs);
    }
    return status ;
}


/** ============================================================================
 *  @func   Benchmarking_Execute
 *
 *  @desc   This function does the actual data transfer with the GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
static
Uns
Benchmarking_Execute ()
{
    Uns         status       = SYS_OK ;
    Uns         i            = 0      ;
    Bool        stsSemPend   = TRUE   ;
    Int         nbytes       = -1     ;
    Arg         arg                   ;
    Int       * reclaimedBuf          ;

    for (i = 1 ; (i <= numTransfers) && (status == SYS_OK) ; i++) {
        stsSemPend = SEM_pend (frameRateSem, SYS_FOREVER) ;

        if (stsSemPend == TRUE) {
            UpdateBenchmarkObject (&benchmarkObj) ;

            status = SIO_issue (input, buf, bufSize, 0) ;

            if (status == SYS_OK) {
                nbytes = SIO_reclaim (input, (Ptr *)&reclaimedBuf, &arg) ;

                if (nbytes < 0) {
                    status = -nbytes ;
                    LOG_printf (&trace, "DSP BIOS Link: "
                                "Buffer reclaim failed %d\n",
                                status) ;
                }
            }
            else {
                LOG_printf (&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                            status) ;
            }
        }
        else {
            LOG_printf (&trace, "DSP BIOS Link: SEM_pend failed %d\n",
                        stsSemPend) ;
        }
    }

    return status ;
}


/** ============================================================================
 *  @func   Benchmarking_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the Benchmarking_Create function.
 *
 *  @modif  input
 *  ============================================================================
 */
static
Void
Benchmarking_Delete ()
{
    SIO_delete (input) ;
    input = NULL ;

    if (buf != NULL) {
        MEM_free (0, buf, bufSize) ;
        buf = NULL ;
    }

    SEM_delete (frameRateSem) ;
}


/** ============================================================================
 *  @func   UpdateBenchmarkObject
 *
 *  @desc   This function measures time difference between two calls and also
 *          returns the same.
 *
 *  @modif  None
 *  ============================================================================
 */
static
LgUns
UpdateBenchmarkObject (BenchmarkObject * benchObj)
{
    LgUns hiResTime ;
    LgUns delta     ;

    hiResTime = CLK_gethtime () ;
    if (benchObj->count == 0) {
        /*
         * first time this one is ever used
         */
        benchObj->lastTime = hiResTime ;
        STS_set (benchObj->sts, hiResTime) ;
        delta = 0 ;
    } else {
        delta = hiResTime - benchObj->lastTime ;
        STS_delta (benchObj->sts, hiResTime) ;
        benchObj->lastTime = hiResTime ;
    }
    benchObj->count++ ;

    return delta ;
}


/** ============================================================================
 *  @func   PrdSemPost
 *
 *  @desc   This function posts a semaphore on which the Benchmarking task
 *          is waiting.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
PrdSemPost ()
{
    SEM_post (frameRateSem) ;
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the Benchmarking task.
 *
 *  @modif  bufSize, numTransfers, inTask
 *  ============================================================================
 */
Void main (Int argc, Char * argv [])
{
    TSK_Handle inTask ;

#if (defined (USE_CCS_BREAKPOINT) && defined ( _55_ ))
    CCS_BREAKPOINT ;
#else
    {
        Int i = 1, j = 2 ;

        /* A forever while loop; It is not simple while (1); because compiler
         * does some unwanted optimization in that case.
         */
        while (i < j)
        {
            j++ ;
            if (j == 100)
            {
                j = 2 ;
            }
        }
    }
#endif

    bufSize = (atoi (argv[0]) / 2) ;

    numTransfers = atoi (argv[1]) ;

    inTask = TSK_create ((Fxn)Benchmarking, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
