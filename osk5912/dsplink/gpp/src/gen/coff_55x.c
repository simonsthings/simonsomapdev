/** ============================================================================
 *  @file   coff_55x.c
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Defines C55x specific functions of COFF loader.
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
#include <coff_55x.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID         ID_GEN_COFF_55x

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON  GEN_SetReason (status, FID_C_GEN_COFF_55x, __LINE__)

/** ============================================================================
 *  @macro  LARGE_MEM_MODEL_SIZE
 *
 *  @desc   Size for the large memory model
 *  ============================================================================
 */
#define  LARGE_MEM_MODEL_SIZE           4


/** ============================================================================
 *  @func   COFF_IsValidFile_55x
 *
 *  @desc   Checks to indicate if the file data format is valid for 55x
 *          architecture.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_IsValidFile_55x (IN  KFileObject * fileObj,
                      OUT Bool *        isValid)
{
    DSP_STATUS status     = DSP_SOK ;
    Bool       isValidId  = FALSE   ;
    Bool       isValidVer = FALSE   ;
    Int16      swapVal              ;
    Uint16     version              ;

    TRC_2ENTER ("COFF_IsValidFile_55x", fileObj, isValid) ;

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

            if (swapVal != COFF_MAGIC_55x) {
                if (SWAP_WORD (swapVal) != COFF_MAGIC_55x) {
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

    TRC_1LEAVE ("COFF_IsValidFile_55x", status) ;

    return status ;
}


/** ============================================================================
 *  @func   COFF_IsSwapped_55x
 *
 *  @desc   Checks to indicate if the file data is swapped for 55x architecture.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_IsSwapped_55x (IN  KFileObject * fileObj,
                    OUT Bool *        isSwapped)
{
    DSP_STATUS status  = DSP_SOK ;
    Int16      swapVal           ;

    TRC_2ENTER ("COFF_IsSwapped_55x", fileObj, isSwapped) ;

    DBC_Require (fileObj != NULL)   ;
    DBC_Require (isSwapped != NULL) ;

    *isSwapped = FALSE ;
    status = KFILE_Seek (fileObj, SWAP_LOCATION, KFILE_SeekSet) ;
    if (DSP_SUCCEEDED (status)) {
        swapVal = COFF_Read16 (fileObj, FALSE) ;

        if (swapVal != COFF_MAGIC_55x) {
            if (SWAP_WORD (swapVal) != COFF_MAGIC_55x) {
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

    TRC_1LEAVE ("COFF_IsSwapped_55x", status) ;

    return status ;
}


/** ============================================================================
 *  @func   COFF_FillArgsBuffer_55x
 *
 *  @desc   Allocates and fills up a buffer with arguments to be sent to DSP's
 *          "main" function for the 55x architecture.
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
COFF_FillArgsBuffer_55x (IN  Uint32      argc,
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
    Int16 *    dstPtr    = NULL    ;
    Int32 *    argvPtr   = NULL    ;
    Int32 *    envPtr    = NULL    ;
    Int16      envc      = 0       ;
    Uint32     totalReqd           ;
    Uint32     length              ;
    Uint32     i                   ;

    TRC_6ENTER ("COFF_FillArgsBuffer_55x",
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
     *      argc + 1 units for storing the argv pointers.
     *      envc + 1 units for storing the envp pointers.
     *  For the current implementation 'envc' is 0 as the environment
     *  variables are not being passed.
     *  ------------------------------------------------------------------------
     */
    totalReqd = (3 + argc + 1 + envc + 1) * LARGE_MEM_MODEL_SIZE ;
    for (i = 0 ; DSP_SUCCEEDED (status) && (i < argc) ; i++) {

        DBC_Assert (argv [i] != NULL) ;

        status = GEN_Strlen (argv [i], &length) ;
        if (DSP_SUCCEEDED (status)) {
            totalReqd += (length + 1) * wordSize ;
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
            loadAddr /= wordSize ;

            *((Int16 *) bufPtr) = SWAP_WORD (argc) ;
            bufPtr++;

            *bufPtr++ = SWAP_LONG (loadAddr + (3 * wordSize)) ;

            *bufPtr++ = SWAP_LONG (loadAddr + ((3 + argc + 1) * wordSize)) ;

            argvPtr   = bufPtr ;
            bufPtr   += argc + 1 ;

            envPtr    = bufPtr ;
            bufPtr   += envc + 1 ;

            for ( ; argc > 0 ; argc--) {
                *argvPtr++ =
                        SWAP_LONG (  (  ((Int32)(  (Char8 *) bufPtr
                                                 - (Char8 *)(argsBuf)))
                                      / wordSize)
                                    + loadAddr) ;

                srcPtr = *argv++ ;
                dstPtr = (Int16 *) bufPtr ;
                while (*srcPtr != '\0') {
                    *dstPtr = SWAP_WORD((Int16)(*srcPtr)) ;
                    dstPtr++ ;
                    srcPtr++ ;
                }
                *dstPtr++ = (Int16) 0 ;
                bufPtr    = (Int32 *) dstPtr ;
            }
            *argvPtr = 0L ;
            *envPtr  = 0L ;
        }
    }

    TRC_1LEAVE ("COFF_FillArgsBuffer_55x", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
