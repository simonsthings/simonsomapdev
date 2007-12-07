/** ============================================================================
 *  @file   TST_FileOperation.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils
 *
 *  @desc   Defines the interface of file operation functions.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_FILEOPERATION_H)
#define TST_FILEOPERATION_H


USES (gpptypes.h)
USES (errbase.h)
USES (tst_framework.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_ReadLine
 *
 *  @desc   It  parses the data file and reads one line.
 *
 *  @arg    argc
 *              No. of arguments in the line parsed.
 *  @arg    argv
 *              The pointer to the list of arguments.
 *  @arg    filePtr
 *              File pointer of the file to be parsed.
 *  @arg    isTestCommented
 *              The pointer to the flag which tells if the test is commented
 *              out.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument given.
 *          DSP_EFILE
 *              File error.
 *
 *  @enter  filePtr should not be NULL.
 *          argc should not be NULL.
 *          argv should not be NULL.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_ReadLine (IN  Void *   filePtr,
              OUT Uint32 * argc,
              OUT Char8    argv [][MAX_BUFSIZE],
              OUT Bool *   isTestCommented) ;


/** ============================================================================
 *  @func   TST_OpenFile
 *
 *  @desc   This function opens a given file and returns a file pointer.
 *
 *  @arg    fileName
 *              The name of the file to be opened.
 *  @arg    filePtr
 *              The address of the file pointer of the file which is being
 *              opened.
 *
 *  @ret    DSP_SOK
 *              Successful
 *          DSP_EINVALIDARG
 *              Invalid arguments.
 *          DSP_EFILE
 *              File not found
 *          DSP_EMEMORY
 *              Memory allocation error
 *
 *  @enter  fileName should not be NULL.
 *          filePtr  should not be NULL.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_OpenFile (IN Char8 * fileName, IN Void ** filePtr) ;


/** ============================================================================
 *  @func   TST_CloseFile
 *
 *  @desc   This function accepts a file pointer and closes that file.
 *
 *  @arg    filePtr
 *              The file pointer of the file which is to be closed.
 *
 *  @ret    DSP_SOK
 *              Successful.
 *          DSP_EFILE
 *              File error.
 *
 *  @enter  filePtr should be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_CloseFile (IN Void ** filePtr) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TST_FILEOPERATION_H) */
