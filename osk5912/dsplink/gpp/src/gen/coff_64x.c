/** ============================================================================
 *  @file   coff_64x.c
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Defines C64x specific functions of COFF loader.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


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
#include <kfile.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>
#include <coff_int.h>
#include <coff.h>
#include <coff_64x.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID         ID_GEN_COFF_64x


/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON  GEN_SetReason (status, FID_C_GEN_COFF_64x, __LINE__)


/** ============================================================================
 *  @macro  ARG_PTR_SIZE
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  ARG_PTR_SIZE 4


/** ============================================================================
 *  @func   COFF_IsValidFile_64x
 *
 *  @desc   Checks to indicate if the file data format is valid for the given
 *          architecture.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_IsValidFile_64x (IN  KFileObject * fileObj,
                      OUT Bool *        isValid)
{
    DSP_STATUS status     = DSP_SOK ;
    Bool       isValidId  = FALSE   ;
    Bool       isValidVer = FALSE   ;
    Int16      swapVal              ;
    Uint16     version              ;

    TRC_2ENTER ("COFF_IsValidFile_64x", fileObj, isValid) ;

    DBC_Require (fileObj != NULL) ;
    DBC_Require (isValid != NULL) ;

    *isValid = FALSE ;
    status = KFILE_Seek (fileObj, 0, KFILE_SeekSet) ;
    if (DSP_SUCCEEDED (status)) {
        version = COFF_Read16 (fileObj, FALSE) ;

        if (version != COFF_VERSION) {
            if (SWAP_WORD (version) != COFF_VERSION) {
                status = DSP_EFILE ;
                SET_FAILURE_REASON ;
            }
            else {
                isValidVer = TRUE ;
            }
        }
        else {
            isValidVer = TRUE ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = KFILE_Seek (fileObj, SWAP_LOCATION, KFILE_SeekSet) ;
        if (DSP_SUCCEEDED (status)) {
            swapVal = COFF_Read16 (fileObj, FALSE) ;

            if (swapVal != COFF_MAGIC_64x) {
                if (SWAP_WORD (swapVal) != COFF_MAGIC_64x) {
                    status = DSP_EFILE ;
                    SET_FAILURE_REASON ;
                }
                else {
                    isValidId = TRUE ;
                }
            }
            else {
                isValidId = TRUE ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        *isValid = (isValidId && isValidVer) ;
    }

    TRC_1LEAVE ("COFF_IsValidFile_64x", status) ;

    return status ;
}


/** ============================================================================
 *  @func   COFF_IsSwapped_64x
 *
 *  @desc   Check to indicate if the file data is swapped.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_IsSwapped_64x (IN  KFileObject * fileObj,
                    OUT Bool *        isSwapped)
{
    DSP_STATUS status  = DSP_SOK ;
    Int16      swapVal           ;

    TRC_2ENTER ("COFF_IsSwapped_64x", fileObj, isSwapped) ;

    DBC_Require (fileObj != NULL)   ;
    DBC_Require (isSwapped != NULL) ;

    *isSwapped = FALSE ;
    status = KFILE_Seek (fileObj, SWAP_LOCATION, KFILE_SeekSet) ;
    if (DSP_SUCCEEDED (status)) {
        swapVal = COFF_Read16 (fileObj, FALSE) ;

        if (swapVal != COFF_MAGIC_64x) {
            if (SWAP_WORD (swapVal) != COFF_MAGIC_64x) {
                status = DSP_EFILE ;
                SET_FAILURE_REASON ;
            }
            else {
                *isSwapped = TRUE ;
            }
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        TRC_1PRINT (TRC_LEVEL3, "Swapped: %d\n", *isSwapped) ;
    }

    TRC_1LEAVE ("COFF_IsSwapped_64x", status) ;

    return status ;
}


/** ============================================================================
 *  @func   COFF_FillArgsBuffer_64x
 *
 *  @desc   Allocates and fills up a buffer with arguments to be sent to DSP's
 *          "main" function for the 64x architecture.
 *
 *          The .args buffer has the form:
 *          argc,
 *      +-- argv,
 *      |   envp
 *      +-->argv[0], -------+
 *          argv[1],        |
 *          ...,            |
 *          argv[n],        |
 *          argv 0 string,<-+
 *          argv 1 string,
 *          ...
 *          argv n string,
 *
 *          The envp pointer needs to be populated for the arguments to be
 *          passed correctly to the DSP executable's 'main'.
 *          However, currently this is not required.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_FillArgsBuffer_64x (IN  Uint32      argc,
                         IN  Char8 **    argv,
                         IN  Uint32      sectSize,
                         IN  Uint32      loadAddr,
                         IN  Uint32      wordSize,
                         IN  Endianism   endian,
                         IN  Void *      argsBuf)
{
    DSP_STATUS status    = DSP_SOK ;
    Int32 *    bufPtr    = NULL    ;
    Char8 *    srcPtr    = NULL    ;
    Int32 *    argvPtr   = NULL    ;
    Int32 *    envPtr    = NULL    ;
    Char8 *    dstPtr    = NULL    ;
    Char8      envc      = 1       ;
    Uint32     totalReqd           ;
    Uint32     length              ;
    Uint32     i                   ;

    TRC_6ENTER ("COFF_FillArgsBuffer_64x",
                argc,
                argv,
                sectSize,
                loadAddr,
                wordSize,
                argsBuf) ;

    DBC_Require (argc > 0) ;
    DBC_Require (argv != NULL) ;
    DBC_Require (argsBuf != NULL) ;

    /*  ------------------------------------------------------------------------
     *  Compute total target buffer size required (in bytes). Memory requirement
     *  is calculated as:
     *      3 units for argc, argv and envp.
     *      argc units for storing the argv pointers.
     *      envc units for storing the envp pointers.
     *  For the current implementation 'envc' is 1 as the environment
     *  variables are not being passed, so it is set as NULL.
     *  ------------------------------------------------------------------------
     */
    totalReqd = (3 + argc + envc) * ARG_PTR_SIZE ;
    for (i = 0 ; DSP_SUCCEEDED (status) && (i < argc) ; i++) {
        DBC_Assert (argv [i] != NULL) ;

        status = GEN_Strlen (argv [i], &length) ;
        if (DSP_SUCCEEDED (status)) {
            totalReqd += (length + 1) ;
            /* Extend to the next wordsize */
            totalReqd = ((totalReqd + (ARG_PTR_SIZE - 1)) / ARG_PTR_SIZE) * ARG_PTR_SIZE ;
        }
    }

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }
    else {
        if (totalReqd > sectSize) {
            status = DSP_ESIZE ;
            SET_FAILURE_REASON ;
        }
        else {
            bufPtr = (Int32 *) argsBuf ;

            /*  ------------------------------------------------------------
             *  Initialize the argument buffer with 0.
             *  ------------------------------------------------------------
             */
            srcPtr = (Char8 *) argsBuf ;
            for (i = 0 ; i < sectSize ; i++) {
                *srcPtr++ = 0 ;
            }

            /*  ------------------------------------------------------------
             *  Fill the buffer according to the required format.
             *  ------------------------------------------------------------
             */
            *bufPtr++ = argc ;
            *bufPtr++ = loadAddr + (3 * ARG_PTR_SIZE) ; /* argv */
            *bufPtr++ = loadAddr + ((3 + argc + 1) * ARG_PTR_SIZE) ; /* envp */

            argvPtr = bufPtr ; /* Pointer to argv[0] */
            bufPtr  += argc ;  /* Space for argv pointers */
            envPtr  = bufPtr ; /* Pointer to first envp */
            bufPtr  += envc ;  /* Space for envp pointers */
            *envPtr = (Int32) 0 ; /* No envp */

            for ( ; argc > 0 ; argc--) {
                *argvPtr++ = (Int32) (  ((Char8 *) bufPtr - (Char8 *) argsBuf)
                                      + loadAddr) ;

                srcPtr = *argv++ ;
                dstPtr = (Char8 *) bufPtr ;
                while (*srcPtr != '\0') {
                    *dstPtr = (Char8)(*srcPtr) ;
                    dstPtr++ ;
                    srcPtr++ ;
                }
                *dstPtr++ = (Char8) '\0' ;
                bufPtr    = (Int32 *) dstPtr ;
            }
        }
    }

    TRC_1LEAVE ("COFF_FillArgsBuffer_64x", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
