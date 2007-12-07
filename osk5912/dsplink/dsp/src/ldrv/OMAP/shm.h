/** ============================================================================
 *  @file   shm.h
 *
 *  @path   $(DSPLINK)\dsp\src\ldrv\OMAP
 *
 *  @desc   Defines the interface of Link Shared Memory Protocol Interface
 *          Module. Note that interface is defined as macros which are
 *          either implemented there only or they are implemented as functions.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#ifndef SHM_
#define SHM_


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <hal.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @macro  SHM_readCtlParam
 *
 *  @desc   Reads a control parameter from the shared memory control structure.
 *
 *  @modif  None
 *  ============================================================================
 */
#define SHM_readCtlParam(param) \
            HAL_memReadUns((Void *) shmFieldMap [param])

/** ============================================================================
 *  @macro  SHM_writeCtlParam
 *
 *  @desc   Writes shared memory control structure parameter.
 *
 *  @modif  None
 *  ============================================================================
 */
#define SHM_writeCtlParam(param, value)\
            HAL_memWriteUns((Void *) shmFieldMap [param], value)

/** ============================================================================
 *  @macro  SHM_writeOutputBuffer
 *
 *  @desc   Writes shared memory output buffer.
 *
 *  @modif  None
 *  ============================================================================
 */
#define SHM_writeOutputBuffer(buffer, size) \
            HAL_memWrite(buffer, (Void *)outputBuffer, size)

/** ============================================================================
 *  @macro  SHM_readInputBuffer
 *
 *  @desc   Reads input data from the shared memory.
 *
 *  @modif  None
 *  ============================================================================
 */
#define SHM_readInputBuffer(buffer, size) \
            HAL_memRead((Void *)inputBuffer, buffer, size)

/** ============================================================================
 *  @macro  SHM_disableGPPInt
 *
 *  @desc   Disbles GPP interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
#define SHM_disableGPPInt() HAL_disableGPPInt()

/** ============================================================================
 *  @macro  SHM_enableGPPInt
 *
 *  @desc   Enables GPP interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
#define SHM_enableGPPInt(key) \
            HAL_enableGPPInt(key)

/** ============================================================================
 *  @macro  SHM_sendInt
 *
 *  @desc   Send interrupt to GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
#define SHM_sendInt(arg) HAL_sendInt(arg)


/** ============================================================================
 *  @name   SHM_FieldId
 *
 *  @desc   Defines Id values for control fields.
 *
 *  @field  SHM_handshakeGPP
 *              GPP's handshake field.
 *  @field  SHM_handshakeDSP
 *              DSP's handshake field.
 *  @field  SHM_dspFreeMask
 *              k'th bit set means DSP is ready to receive on k'th
 *              channel.
 *  @field  SHM_gppFreeMask
 *              k'th bit set means GPP is ready to receive on k'th
 *              channel.
 *  @field  SHM_inputFull
 *              There is something in the input buffer for DSP to read.
 *  @field  SHM_inputId
 *              Channel number for which input buffer is available.
 *  @field  SHM_inputSize
 *              Size of the buffer.
 *  @field  SHM_outputFull
 *              Output is full and GPP should read it.
 *  @field  SHM_outputId
 *              Channel number for which output is done.
 *  @field  SHM_outputSize
 *              Size of output buffer.
 *  @field  argv
 *              reserved
 *  @field  resv
 *              reserved
 *  @field  dspFreeMsg
 *              If set, indicates that a free message is available on the DSP.
 *  @field  gppFreeMsg
 *              If set, indicates that a free message is available on the GPP.
 *  ============================================================================
 */
typedef enum {
    SHM_handshakeGPP = 0,
    SHM_handshakeDSP,
    SHM_dspFreeMask,
    SHM_gppFreeMask,
    SHM_inputFull,
    SHM_inputId,
    SHM_inputSize,
    SHM_outputFull,
    SHM_outputId,
    SHM_outputSize,
    SHM_argv,
    SHM_resv
#if defined(_MSGQ_COMPONENT)
    ,SHM_dspFreeMsg,
    SHM_gppFreeMsg
#endif /* #if defined(_MSGQ_COMPONENT) */
} SHM_FieldId;


/** ============================================================================
 *  @name   SHM_Control
 *
 *  @desc   Control structure of Shared memory.
 *
 *  @field  handshakeGPP
 *              GPP's handshake field.
 *  @field  handshakeDSP
 *              DSP's handshake field.
 *  @field  dspFreeMask
 *              k'th bit set means DSP is ready to receive on k'th
 *              channel.
 *  @field  gppFreeMask
 *              k'th bit set means GPP is ready to receive on k'th
 *              channel.
 *  @field  inputFull
 *              There is something in the input buffer for DSP to read.
 *  @field  inputId
 *              Channel number for which input buffer is available.
 *  @field  inputSize
 *              Size of the buffer.
 *  @field  outputFull
 *              Output is full and GPP should read it.
 *  @field  outputId
 *              Channel number for which output is done.
 *  @field  outputSize
 *              Size of output buffer.
 *  @field  argv
 *              reserved
 *  @field  resv
 *              reserved
 *  @field  dspFreeMsg
 *              Indicates whether a free message is available on the DSP.
 *              (written by DSP/read by GPP)
 *  @field  gppFreeMsg
 *              Indicates whether a free message is available on the GPP.
 *              (written by GPP/read by DSP)
 *  ============================================================================
 */
typedef struct SHM_Control {
    volatile Uns  handshakeGPP;
    volatile Uns  handshakeDSP;
    volatile Uns  dspFreeMask;
    volatile Uns  gppFreeMask;
    volatile Uns  outputFull;
    volatile Uns  outputId;
    volatile Uns  outputSize;
    volatile Uns  inputFull;
    volatile Uns  inputId;
    volatile Uns  inputSize;
    volatile Uns  argv;
    volatile Uns  resv;
#if defined(_MSGQ_COMPONENT)
    volatile Uns  dspFreeMsg;
    volatile Uns  gppFreeMsg;
#endif /* #if defined(_MSGQ_COMPONENT) */
} SHM_Control;


/** ============================================================================
 *  @func   shmFieldMap
 *
 *  @desc   Shared memory field address mapping array.
 *
 *  @modif  None
 *  ============================================================================
 */
extern volatile Uns * shmFieldMap [];

/** ============================================================================
 *  @name   outputBuffer, inputBuffer
 *
 *  @desc   Pointer to shared memory output buffer and input buffer.
 *  ============================================================================
 */
extern volatile Uns *outputBuffer;
extern volatile Uns *inputBuffer;


/** ============================================================================
 *  @func   SHM_init
 *
 *  @desc   Initializes shared memory. It also intializes the hardware
 *          abstraction  module.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Void SHM_init();


/** ============================================================================
 *  @func   SHM_handshake
 *
 *  @desc   Does handshaking with ARM.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Void SHM_handshake();


/** ============================================================================
 *  @func   SHM_registerGPPISR
 *
 *  @desc   Register ISR for GPP interrupt.
 *
 *  @arg    func
 *              Function to register.
 *          arg
 *              Argument to function.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SHM_init
 *  ============================================================================
 */
Void SHM_registerGPPISR(Ptr func, Ptr arg);


/** ============================================================================
 *  @func   SHM_getMaxBufferSize
 *
 *  @desc   Function to get maximum size of shared memory buffer.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SHM_init
 *  ============================================================================
 */
Uns SHM_getMaxBufferSize();


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* SHM_ */

