/** ============================================================================
 *  @file   kfile.h
 *
 *  @path   $(DSPLINK)\gpp\src\osal
 *
 *  @desc   Defines interfaces and data structures for the sub-component KFILE.
 *          This subcomponent assumes that a file system is available on
 *          the target platform.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (KFILE_H)
#define KFILE_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   KFILE_Seek
 *
 *  @desc   Enumerates the values used for repositioning the
 *          file position indicator.
 *
 *  @field  KFILE_SeekSet
 *              Seek from beginning of file.
 *  @field  KFILE_SeekCur
 *              Seek from current position.
 *  @field  KFILE_SeekEnd
 *              Seek from end of file.
 *  ============================================================================
 */
typedef enum {
    KFILE_SeekSet = 0x00,
    KFILE_SeekCur = 0x01,
    KFILE_SeekEnd = 0x02
} KFILE_FileSeek ;


/** ============================================================================
 *  @name   FileName
 *
 *  @desc   Definition for identifying files.
 *  ============================================================================
 */
typedef Pstr FileName ;

/** ============================================================================
 *  @name   KFileObject
 *
 *  @desc   Forward declaration. The actual definition is dependent on the
 *          file system available on OS.
 *  ============================================================================
 */
typedef struct KFileObject_tag  KFileObject ;


/** ============================================================================
 *  @func   KFILE_Initialize
 *
 *  @desc   This function allocates and initializes all resources used by this
 *          subcomponent.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    KFILE_Finalize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Initialize () ;


/** ============================================================================
 *  @func   KFILE_Finalize
 *
 *  @desc   Releases resources used by this sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory.
 *
 *  @enter  Subcomponent must be initialized.
 *
 *  @leave  None
 *
 *  @see    KFILE_Initialize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Finalize () ;


/** ============================================================================
 *  @func   KFILE_Open
 *
 *  @desc   Opens a file.
 *
 *  @arg    fileName
 *              Name of the file to be opened.
 *  @arg    mode
 *              Mode for opening the file. This argument is case-sensitive.
 *              Expected modes are: "r" for read, "w" for write and
 *              "a" for append.
 *  @arg    fileHandle
 *              Placeholder to return the file handle.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid arguments.
 *          DSP_EFILE
 *              File not found.
 *          DSP_EMEMORY
 *              Out of memory error.
 *
 *  @enter  Subcomponent must be initialized.
 *          fileName must be valid.
 *          mode must be valid.
 *          fileHandle must be valid.
 *
 *  @leave  fileHandle contains the fileObject on success.
 *
 *  @see    KFILE_Close
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Open (IN CONST FileName       fileName,
            IN CONST Char8 *        mode,
            OUT      KFileObject ** fileHandle) ;


/** ============================================================================
 *  @func   KFILE_Close
 *
 *  @desc   Closes a file handle.
 *
 *  @arg    fileObj
 *             Handle of file to be closed, returned from KFILE_Open.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFILE
 *              File is not open.
 *          DSP_EPOINTER
 *              Invalid file object.
 *
 *  @enter  Subcomponent must be initialized.
 *          fileObj must be a valid handle to a file opened earlier.
 *
 *  @leave  Memory allocated for fileObj is freed.
 *
 *  @see    KFILE_Open
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Close (IN  KFileObject * fileObj) ;


/** ============================================================================
 *  @func   KFILE_Read
 *
 *  @desc   Reads a specified number of items of specified size (in bytes)
 *          to a buffer.
 *
 *  @arg    buffer
 *             Buffer in which the contents of file are read.
 *  @arg    size
 *             Size of each object to read from file.
 *  @arg    count
 *             Number of objects to read.
 *  @arg    fileObj
 *             KFileObject to read from.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid arguments.
 *          DSP_EPOINTER
 *              Invalid file object.
 *          DSP_EFILE
 *              File is not open or error reading file.
 *          DSP_ERANGE
 *              The requested number of bytes is beyond EOF.
 *
 *  @enter  Subcomponent must be initialized.
 *          fileObj must be a valid KFileObject pointer opened earlier.
 *
 *  @leave  None
 *
 *  @see    KFILE_Open
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Read (OUT Char8 *       buffer,
            IN  Uint32        size,
            IN  Uint32        count,
            IN  KFileObject * fileObj) ;


/** ============================================================================
 *  @func   KFILE_Seek
 *
 *  @desc   Repositions the file pointer according to specified arguments.
 *
 *  @arg    fileObj
 *              The fileObject to seek into.
 *  @arg    offset
 *              Offset for positioning the file pointer.
 *  @arg    origin
 *              Origin for calculating absolute position where file pointer
 *              is to be positioned. This can take the following values:
 *                  KFILE_SeekSet
 *                  KFILE_SeekCur
 *                  KFILE_SeekEnd
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid arguments.
 *          DSP_EPOINTER
 *              Invalid file object.
 *          DSP_EFILE
 *              File is not opened.
 *          DSP_ERANGE
 *              Offset and origin combination is beyond file size range.
 *
 *  @enter  Subcomponent must be initialized.
 *          fileObj must be a valid handle to a file opened earlier.
 *
 *  @leave  None
 *
 *  @see    KFILE_Tell
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Seek (IN  KFileObject *   fileObj,
            IN  Int32           offset,
            IN  KFILE_FileSeek  origin) ;


/** ============================================================================
 *  @func   KFILE_Tell
 *
 *  @desc   Returns the current file pointer position for the specified
 *          file handle.
 *
 *  @arg    fileObj
 *              The fileObject pointer.
 *  @arg    pos
 *              Out argument for holding the current file position
 *              indicator value.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid arguments.
 *          DSP_EPOINTER
 *              Invalid file object.
 *          DSP_EFILE
 *              file is not opened.
 *
 *  @enter  Subcomponent must be initialized.
 *          fileObj must be a valid handle to a file opened earlier.
 *
 *  @leave  None
 *
 *  @see    KFILE_Seek
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Tell (IN  KFileObject * fileObj,
            OUT Int32 *       pos) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif   /* !defined (KFILE_H) */
