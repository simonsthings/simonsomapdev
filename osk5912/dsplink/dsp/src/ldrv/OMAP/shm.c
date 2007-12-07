/** ============================================================================
 *  @file   shm.c
 *
 *  @path   $(DSPLINK)\dsp\src\ldrv\OMAP
 *
 *  @desc   Implementation of Shared Memory Protocol Control Module. It provide
 *          functions that are called by main interface module to access shared
 *          memory.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>
#include <sys.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <shm_driver.h>
#include <shm.h>
#include <failure.h>
#include <dbc.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   File Id of this file.
 *  ============================================================================
 */
#define FILEID  FID_SHM_C

/** ============================================================================
 *  @macro  MAX_SHM_FIELDS
 *
 *  @desc   Total number of shared memory control parameters.
 *  ============================================================================
 */
#define MAX_SHM_FIELDS 14

/** ============================================================================
 *  @macro  SHM_BASE
 *
 *  @desc   Base address (word address) of shared memory.
 *  ============================================================================
 */
const LgUns SHM_BASE = 0x200000 ;

/** ============================================================================
 *  @macro  SHM_LEN
 *
 *  @desc   Length of shared memory (1MB) in words.
 *  ============================================================================
 */
const LgUns SHM_LEN  = 0x80000 ;

/** ============================================================================
 *  @macro  SHM_CONTROL_LEN
 *
 *  @desc   Total length of shared memory control structure in 16 bit words.
 *  ============================================================================
 */
const LgUns SHM_CONTROL_LEN = sizeof(SHM_Control) ;

/** ============================================================================
 *  @macro  SHM_BUFFER_LEN
 *
 *  @desc   Buffer size in words (16 bit) of shared memory buffers.
 *  ============================================================================
 */
#define SHM_BUFFER_LEN      ((SHM_LEN - SHM_CONTROL_LEN) / 2)

/** ============================================================================
 *  @name   SHM_GPP_TOKEN
 *
 *  @desc   Synchronization token that GPP writes and DSP reads from a fixed
 *          location in shared memory.
 *  ============================================================================
 */
#define SHM_GPP_TOKEN   0xBABA

/** ============================================================================
 *  @name   SHM_DSP_TOKEN
 *
 *  @desc   Synchronization token that DSP writes and GPP reads from a fixed
 *          location in shared memory.
 *  ============================================================================
 */
#define SHM_DSP_TOKEN   0xC0C0

/** ============================================================================
 *  @name   outputBuffer, inputBuffer
 *
 *  @desc   Pointer to shared memory output buffer and input buffer.
 *  ============================================================================
 */
volatile Uns *outputBuffer;
volatile Uns *inputBuffer;

/** ============================================================================
 *  @name   shmFieldMap
 *
 *  @desc   Mapping of shared memory parameter with the actual address where
 *          that parameter is located in shared memory.
 *  ============================================================================
 */
volatile Uns *shmFieldMap [MAX_SHM_FIELDS];


/*  ============================================================================
 *  Create named sections for the functions to allow specific memory placement.
 *  ============================================================================
 */
#pragma CODE_SECTION (SHM_init,           ".text:DSPLINK_init")
#pragma CODE_SECTION (SHM_handshake,      ".text:DSPLINK_init")
#pragma CODE_SECTION (SHM_registerGPPISR, ".text:DSPLINK_init")


/** ============================================================================
 *  @func   SHM_init
 *
 *  @desc   Initializes shared memory. It also intializes the hardware
 *          abstraction  module.
 *
 *  @modif  None
 *  ============================================================================
 */
Void SHM_init()
{
    SHM_Control *shmControl = (SHM_Control *)SHM_BASE;

    HAL_init();

    outputBuffer = (volatile Uns *)(SHM_BASE + SHM_CONTROL_LEN);
    inputBuffer  = (volatile Uns *)
                            (SHM_BASE + SHM_CONTROL_LEN + SHM_BUFFER_LEN);

    shmFieldMap [SHM_handshakeGPP]  = &(shmControl->handshakeGPP);
    shmFieldMap [SHM_handshakeDSP]  = &(shmControl->handshakeDSP);
    shmFieldMap [SHM_dspFreeMask]   = &(shmControl->dspFreeMask);
    shmFieldMap [SHM_gppFreeMask]   = &(shmControl->gppFreeMask);
    shmFieldMap [SHM_inputFull]     = &(shmControl->inputFull);
    shmFieldMap [SHM_inputId]       = &(shmControl->inputId);
    shmFieldMap [SHM_inputSize]     = &(shmControl->inputSize);
    shmFieldMap [SHM_outputFull]    = &(shmControl->outputFull);
    shmFieldMap [SHM_outputId]      = &(shmControl->outputId);
    shmFieldMap [SHM_outputSize]    = &(shmControl->outputSize);
    shmFieldMap [SHM_argv]          = &(shmControl->argv);
    shmFieldMap [SHM_resv]          = &(shmControl->resv);
#if defined (_MSGQ_COMPONENT)
    shmFieldMap [SHM_dspFreeMsg]    = &(shmControl->dspFreeMsg);
    shmFieldMap [SHM_gppFreeMsg]    = &(shmControl->gppFreeMsg);
#endif /* if defined (_MSGQ_COMPONENT) */
}


/** ============================================================================
 *  @func   SHM_handshake
 *
 *  @desc   Does handshaking with ARM.
 *
 *  @modif  None
 *  ============================================================================
 */
Void SHM_handshake()
{
    /* DSP reads the value of token that ARM writes as part of synchronization
     */
    while ((SHM_readCtlParam(SHM_handshakeGPP) != SHM_GPP_TOKEN)) ;

    /* DSP writes the value on which ARM is waiting as part of synchronization
     * procedure.
     */
    SHM_writeCtlParam(SHM_handshakeDSP, SHM_DSP_TOKEN) ;
}


/** ============================================================================
 *  @func   SHM_getMaxBufferSize
 *
 *  @desc   Function to get maximum size of shared memory buffer.
 *
 *  @modif  None
 *  ============================================================================
 */
Uns SHM_getMaxBufferSize()
{
    LgUns maxBufferSize = SHM_BUFFER_LEN ;

    /*
     * Buffer size is limited to 0xFFFF MAUs due to restrictions on packet size
     */
    maxBufferSize = maxBufferSize > 0xFFFF ? 0xFFFF : maxBufferSize ;

    return (Uns) maxBufferSize ;
}


/** ============================================================================
 *  @func   SHM_registerGPPISR
 *
 *  @desc   Register ISR for GPP interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
Void SHM_registerGPPISR(Ptr func, Ptr arg)
{
    DBC_require(func != NULL);
    HAL_registerGPPISR(func, arg);
}


#ifdef __cplusplus
}
#endif /* extern "C" */
