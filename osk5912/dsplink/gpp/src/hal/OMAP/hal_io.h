/** ============================================================================
 *  @file   hal_io.h
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Declares necessary functions for IO operations.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (HAL_IO_H)
#define HAL_IO_H


USES (gpptypes.h)
USES (errbase.h)
USES (linkdefs.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @func   HAL_ReadDspData
 *
 *  @desc   Read from DSP internal memory.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    endianism
 *              Endianism of data being read.
 *  @arg    hostBuf
 *              Host storage for DSP data.
 *  @arg    dspAddr
 *              Address to read from.
 *  @arg    numBytes
 *              Number of Bytes to read.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ReadDspData (IN HalObject *  halObject,
                 IN Endianism    endianism,
                 IN Uint8 *      hostBuf,
                 IN Uint32       dspAddr,
                 IN Uint32       numBytes) ;


/** ============================================================================
 *  @func   HAL_WriteDspData
 *
 *  @desc   Write to DSP internal memory.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    endianism
 *              Endianism of data being written.
 *  @arg    hostBuf
 *              Location of host data to write.
 *  @arg    dspAddr
 *              Address to write to.
 *  @arg    numBytes
 *              Number of Bytes to write.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_WriteDspData (IN HalObject * halObject,
                  IN Endianism   endianism,
                  IN Uint8 *     hostBuf,
                  IN Uint32      dspAddr,
                  IN Uint32      numBytes) ;


/** ============================================================================
 *  @func   HAL_WriteExtDspData
 *
 *  @desc   Write to External memory.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    endianism
 *              Endianism of data being written.
 *  @arg    hostBuf
 *              Location of host data to write.
 *  @arg    dspAddr
 *              Address to write to.
 *  @arg    numBytes
 *              Number of Bytes to write.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_WriteExtDspData (IN HalObject * halObject,
                     IN Endianism   endianism,
                     IN Uint8 *     hostBuf,
                     IN Uint32      dspAddr,
                     IN Uint32      numBytes) ;


#if defined (__cplusplus)
}
#endif


#endif  /* !defined (HAL_IO_H) */
