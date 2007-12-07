/** ============================================================================
 *  @file   kfile.c
 *
 *  @path   $(DSPLINK)\gpp\src\osal\Linux
 *
 *  @desc   Implementation of sub-component KFILE.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers         */
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>

/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Headers                */
#include <mem.h>
#include <kfile.h>
#include <print.h>

/*  ----------------------------------- Generic Function            */
#include <gen_utils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Identifier for sub-component.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_OSAL_KFILE


/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_OSAL_KFILE, __LINE__)

/** ============================================================================
 *  @macro  SEEK_SET
 *
 *  @desc   Seek from beginning.
 *  ============================================================================
 */
#define SEEK_SET        0

/** ============================================================================
 *  @macro  SEEK_CUR
 *
 *  @desc   Seek from current location.
 *  ============================================================================
 */
#define SEEK_CUR        1

/** ============================================================================
 *  @macro  SEEK_END
 *
 *  @desc   Seek from end of file.
 *  ============================================================================
 */
#define SEEK_END        2


/** ============================================================================
 *  @name   KFileObject_tag
 *
 *  @desc   This is a forward declared type in kfile.h.
 *
 *  @field  signature
 *              Signature of the KFILE object.
 *  @field  fileDesc
 *              File descriptor.
 *  @field  fileName
 *              File name.
 *  @field  isOpen
 *              Flag to indicate that file is open.
 *  @field  size
 *              Size of the file.
 *  @field  curPos
 *              Current file position indicator.
 *
 *  @see    None
 *  ============================================================================
 */
struct KFileObject_tag {
    Uint32        signature ;
    struct file * fp        ;
    Pstr          fileName  ;
    Bool          isOpen    ;
    Uint32        size      ;
    Uint32        curPos    ;
} ;


/** ============================================================================
 *  @name   KFILE_IsInitialized
 *
 *  @desc   Flag to keep track of initialization of this subcomponent.
 *  ============================================================================
 */
STATIC Bool KFILE_IsInitialized = FALSE ;


/** ============================================================================
 *  @func   KFILE_Initialize
 *
 *  @desc   Initializes KFILE suc-component by allocating all resources.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Initialize ()
{
    DSP_STATUS status = DSP_SOK ;

    TRC_0ENTER ("KFILE_Initialize") ;

    DBC_Require (KFILE_IsInitialized == FALSE) ;

    KFILE_IsInitialized = TRUE ;

    TRC_1LEAVE ("KFILE_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   KFILE_Finalize
 *
 *  @desc   Releases resources used by this sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Finalize ()
{
    DSP_STATUS status = DSP_SOK ;

    TRC_0ENTER ("KFILE_Finalize") ;

    DBC_Require (KFILE_IsInitialized == TRUE) ;

    KFILE_IsInitialized = FALSE ;

    TRC_1LEAVE ("KFILE_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   KFILE_Open
 *
 *  @desc   Opens a file specified by name of file.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Open (IN CONST FileName       fileName,
            IN CONST Char8 *        mode,
            OUT      KFileObject ** fileHandle)
{
    DSP_STATUS    status = DSP_SOK ;
    Uint32        length = 0       ;
    struct file * fp     = NULL    ;
    mm_segment_t  fs               ;

    TRC_3ENTER ("KFILE_Open ()", fileName, mode, fileHandle) ;

    DBC_Require (KFILE_IsInitialized == TRUE) ;
    DBC_Require (fileName != NULL) ;
    DBC_Require (mode != NULL) ;
    DBC_Require (fileHandle != NULL) ;

    if (   (fileName == NULL)
        || (fileHandle == NULL)
        || (mode == NULL)
        || (mode [0] != 'r')) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        *fileHandle = NULL ;

        status = GEN_Strlen (fileName, &length) ;
    }

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }
    else if (length == 0) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        status = MEM_Alloc ((Void **) fileHandle,
                            sizeof (KFileObject),
                            MEM_DEFAULT) ;
    }

    if (DSP_SUCCEEDED (status)) {
        fs = get_fs () ;
        set_fs (get_ds ()) ;

        /*  --------------------------------------------------------------------
         *  Open the file using filp_open call. Initialize the file object
         *  after validating the object returned by kernel.
         *  --------------------------------------------------------------------
         */
        fp = filp_open (fileName, O_RDONLY, 0) ;
        if (   (IS_ERR (fp))
            || (fp == NULL)
            || (fp->f_op == NULL)
            || (fp->f_op->read == NULL)
            || (fp->f_op->llseek == NULL)) {
            status = DSP_EFILE ;
            SET_FAILURE_REASON ;
        }
        else {
            (*fileHandle)->signature = SIGN_KFILE ;
            (*fileHandle)->fp        = fp       ;
            (*fileHandle)->fileName  = fileName   ;
            (*fileHandle)->isOpen    = TRUE       ;
            (*fileHandle)->curPos    = 0          ;

            (*fileHandle)->size      = fp->f_op->llseek (fp, 0, SEEK_END) ;
            fp->f_op->llseek (fp, 0, SEEK_SET) ;
        }

        set_fs (fs) ;

        /*  --------------------------------------------------------------------
         *  If the function call failed then free the object allocated before.
         *  --------------------------------------------------------------------
         */
        if (DSP_FAILED (status)) {
            FREE_PTR (*fileHandle) ;
            *fileHandle = NULL ;
        }
    }
    else {
        status = DSP_EFILE ;
        SET_FAILURE_REASON ;
    }

    DBC_Ensure (   (   DSP_SUCCEEDED (status)
                    && IS_OBJECT_VALID (*fileHandle, SIGN_KFILE))
                || DSP_FAILED (status)) ;

    DBC_Ensure (   DSP_SUCCEEDED (status)
                || (   DSP_FAILED (status)
                    && (fileHandle != NULL)
                    && (*fileHandle == NULL))) ;

    TRC_1LEAVE ("KFILE_Open", status) ;

    return status ;
}


/** ============================================================================
 *  @func   KFILE_Close
 *
 *  @desc   Closes a file handle.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Close (IN KFileObject * fileObj)
{
    DSP_STATUS   status = DSP_SOK ;
    mm_segment_t fs               ;

    TRC_1ENTER ("KFILE_Close", fileObj) ;

    DBC_Require (KFILE_IsInitialized == TRUE) ;
    DBC_Require (fileObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (fileObj, SIGN_KFILE)) ;
    DBC_Require ((fileObj != NULL) && (fileObj->isOpen == TRUE)) ;

    if (IS_OBJECT_VALID (fileObj, SIGN_KFILE) == FALSE) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else {
        if (fileObj->isOpen == FALSE) {
            status = DSP_EFILE ;
            SET_FAILURE_REASON ;
        }
        else {
            fs = get_fs () ;
            set_fs (get_ds ()) ;
            filp_close (fileObj->fp, NULL) ;
            set_fs (fs) ;
            fileObj->signature = SIGN_NULL ;
            FREE_PTR (fileObj) ;
        }
    }

    DBC_Ensure (   (DSP_SUCCEEDED (status) && (fileObj == NULL))
                || (DSP_FAILED (status))) ;

    TRC_1LEAVE ("KFILE_Close", status) ;

    return status ;
}


/** ============================================================================
 *  @func   KFILE_Read
 *
 *  @desc   Reads a specified number of items of specified size
 *          bytes from file to a buffer.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Read (IN OUT  Char8 *       buffer,
            IN      Uint32        size,
            IN      Uint32        count,
            IN      KFileObject * fileObj)
{
    DSP_STATUS      status    = DSP_SOK ;
    Uint32          bytesRead = 0       ;
    mm_segment_t    fs                  ;

    TRC_4ENTER ("KFILE_Read", buffer, size, count, fileObj) ;

    DBC_Require (KFILE_IsInitialized == TRUE) ;
    DBC_Require (fileObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (fileObj, SIGN_KFILE)) ;
    DBC_Require ((fileObj != NULL) && (fileObj->isOpen == TRUE)) ;
    DBC_Require (buffer != NULL) ;

    if (IS_OBJECT_VALID (fileObj, SIGN_KFILE) == FALSE) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else if (buffer == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (fileObj->isOpen == FALSE) {
        status = DSP_EFILE ;
        SET_FAILURE_REASON ;
    }
    else if ((fileObj->curPos + (size * count)) > fileObj->size) {
        status = DSP_ERANGE ;
        SET_FAILURE_REASON  ;
    }
    else if ((size != 0) && (count != 0)) {
        fs = get_fs () ;
        set_fs (get_ds ()) ;

        bytesRead = fileObj->fp->f_op->read (fileObj->fp,
                                             buffer,
                                             size * count,
                                             &(fileObj->fp->f_pos)) ;
        set_fs (fs) ;

        if (bytesRead >= 0) {
            fileObj->curPos += bytesRead ;
            DBC_Assert ((bytesRead / size) == (Uint32)count);
        }
        else {
            status = DSP_EFILE;
            TRC_1PRINT (TRC_LEVEL1,
                        "File Read failed with status [0x%x]\n",
                        status) ;
        }
    }

    TRC_1LEAVE ("KFILE_Read", status) ;

    return status ;
}


/** ============================================================================
 *  @func   KFILE_Seek
 *
 *  @desc   Repositions the file pointer according to specified arguments.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Seek (IN  KFileObject *   fileObj,
            IN  Int32           offset,
            IN  KFILE_FileSeek  origin)
{
    DSP_STATUS    status = DSP_SOK ;
    struct file * fp     = NULL    ;
    mm_segment_t  fs               ;

    TRC_3ENTER ("KFILE_Seek", fileObj, offset, origin) ;

    DBC_Require (KFILE_IsInitialized == TRUE) ;
    DBC_Require (fileObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (fileObj, SIGN_KFILE)) ;
    DBC_Require ((fileObj != NULL) && (fileObj->isOpen == TRUE)) ;
    DBC_Require (   (origin == KFILE_SeekSet)
                 || (origin == KFILE_SeekCur)
                 || (origin == KFILE_SeekEnd)) ;

    if (IS_OBJECT_VALID (fileObj, SIGN_KFILE) == FALSE) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else if (fileObj->isOpen == FALSE) {
        status = DSP_EFILE ;
        SET_FAILURE_REASON ;
    }
    else {
        fs = get_fs () ;
        set_fs (get_ds()) ;

        fp = fileObj->fp ;

        switch (origin)
        {
        case KFILE_SeekSet:
            if (   (offset < 0)
                || (offset > fileObj->size)) {
                status = DSP_ERANGE ;
                SET_FAILURE_REASON ;
            }
            else {
                fileObj->curPos = fp->f_op->llseek (fp, offset, SEEK_SET) ;
            }
            break ;

        case KFILE_SeekCur:
            if (   ((fileObj->curPos + offset) > fileObj->size)
                || ((fileObj->curPos + offset) < 0)) {
                status = DSP_ERANGE ;
                SET_FAILURE_REASON ;
            }
            else {
                fileObj->curPos = fp->f_op->llseek (fp, offset, SEEK_CUR) ;
            }
            break ;

        case KFILE_SeekEnd:
            /*  ----------------------------------------------------------------
             *  A negative offset indicates offset from the end of file.
             *  Check that the specified offset is not beyond
             *  the bounds of the file.
             *  ----------------------------------------------------------------
             */
            if (   (-offset < 0)
                || (-offset > fileObj->size)) {
                status = DSP_ERANGE ;
                SET_FAILURE_REASON ;
            }
            else {
                fileObj->curPos = fp->f_op->llseek (fp, offset, SEEK_END) ;
            }
            break ;

        default:
            TRC_0PRINT (TRC_LEVEL7, "Invalid origin specified\n") ;
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
            break ;
        }
        set_fs (fs) ;
    }

    TRC_1LEAVE ("KFILE_Seek", status) ;

    return status ;
}


/** ============================================================================
 *  @func   KFILE_Tell
 *
 *  @desc   Returns the current file pointer position for the specified
 *          file handle.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
KFILE_Tell (IN  KFileObject * fileObj,
            OUT Int32 *       pos)
{
    DSP_STATUS   status = DSP_SOK ;

    TRC_2ENTER ("KFILE_Tell", fileObj, pos) ;

    DBC_Require (KFILE_IsInitialized == TRUE) ;
    DBC_Require (fileObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (fileObj, SIGN_KFILE)) ;
    DBC_Require ((fileObj != NULL) && (fileObj->isOpen == TRUE)) ;
    DBC_Require (pos != NULL) ;

    if (IS_OBJECT_VALID (fileObj, SIGN_KFILE) == FALSE) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else if (pos == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (fileObj->isOpen == FALSE) {
        status = DSP_EFILE ;
        SET_FAILURE_REASON ;
    }
    else {
        *pos = fileObj->curPos ;
        DBC_Assert (*pos == fileObj->fp->f_pos) ;
    }

    DBC_Ensure (   (DSP_SUCCEEDED (status) && (pos != NULL) && (*pos >= 0))
                || (DSP_FAILED (status))) ;

    TRC_1LEAVE ("KFILE_Tell", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
