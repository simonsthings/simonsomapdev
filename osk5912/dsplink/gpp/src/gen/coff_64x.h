/** ============================================================================
 *  @file   coff_64x.h
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Defines interface for C64x specific functions of COFF loader.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (COFF_64X_H)
#define COFF_64X_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  COFF_MAGIC_64x
 *
 *  @desc   Magic number to identify 64x COFF file format.
 *  ============================================================================
 */
#define     COFF_MAGIC_64x                        0x0099

/** ============================================================================
 *  @func   COFF_IsSwapped_64x
 *
 *  @desc   Checks if the fields of headers are stored as byte
 *          swapped values in 64x file format.
 *
 *  @arg    fileObj
 *              Handle to the COFF file.
 *  @arg    isSwapped
 *              OUT argument to contain if the COFF headers in file are swapped.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_RANGE
 *              Seek error in file.
 *
 *  @enter  fileObj must be a valid pointer.
 *          isSwapped must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_IsSwapped_64x (IN  KFileObject * fileObj, OUT Bool * isSwapped) ;


/** ============================================================================
 *  @func   COFF_IsValidFile_64x
 *
 *  @desc   Checks to indicate if the file data format is valid for 64x.
 *          architecture.
 *
 *  @arg    fileObj
 *              Handle to the COFF file.
 *  @arg    isValid
 *              OUT argument to contain if the file data format is valid
 *              for 64x.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_RANGE
 *              Seek error in file.
 *
 *  @enter  fileObj must be a valid pointer.
 *          isValid must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_IsValidFile_64x (IN  KFileObject * fileObj, OUT Bool * isValid) ;


/** ============================================================================
 *  @func   COFF_FillArgsBuffer_64x
 *
 *  @desc   Fills up the specified buffer with arguments to be sent to DSP's
 *          "main" function for the 64x architecture.
 *
 *  @arg    argc
 *              Number of arguments to be passed.
 *  @arg    argv
 *              Argument strings to be passed.
 *  @arg    size
 *              Size of the '.args' section obtained from the COFF file.
 *  @arg    loadAddr
 *              Load address for the '.args' section.
 *  @arg    wordSize
 *              Wordsize on the target DSP.
 *  @arg    endian
 *              Endianism to be used while writing data.
 *  @arg    argsBuf
 *              Buffer to be filled with formatted argc and argv.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_ESIZE
 *              Insufficient space in .args buffer to hold all the arguments.
 *          DSP_EMEMORY
 *              Out of memory error.
 *
 *  @enter  argc must be greater than 0.
 *          argv must be valid pointer.
 *          argsBuf must be a valid pointer.
 *          sizeBuf must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_FillArgsBuffer_64x (IN  Uint32    argc,
                         IN  Char8 **  argv,
                         IN  Uint32    sectSize,
                         IN  Uint32    loadAddr,
                         IN  Uint32    wordSize,
                         IN  Endianism endian,
                         IN  Void *    argsBuf) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (COFF_64X_H) */
