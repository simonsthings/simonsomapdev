/** ============================================================================
 *  @file   gen_utils.c
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Implementation of platform independent common library
 *          functions interface.
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
#include <errbase.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Headers                */
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and subcomponent identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_GEN_GEN

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_GEN_GEN, __LINE__)


/** ============================================================================
 *  @name   ErrReason
 *
 *  @desc   Global for error storing reason.
 *  ============================================================================
 */
ErrorInfo     ErrReason ;


/** ============================================================================
 *  @func   GEN_Initialize
 *
 *  @desc   Initializes the GEN module's private state.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Initialize ()
{
    DSP_STATUS status = DSP_SOK ;

    TRC_0ENTER ("GEN_Initialize") ;

    TRC_1LEAVE ("GEN_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_Finalize
 *
 *  @desc   Discontinues usage of the GEN module.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Finalize ()
{
    DSP_STATUS status = DSP_SOK ;

    TRC_0ENTER ("GEN_Finalize") ;

    TRC_1LEAVE ("GEN_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_SetReason
 *
 *  @desc   Sets failure reason.
 *          In debug build, all failures are printed to show the complete call
 *          stack.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
GEN_SetReason (DSP_STATUS status, Int32 FileId, Int32 Line)
{
#if defined(DDSP_DEBUG)
    TRC_3PRINT (TRC_LEVEL7,
                "Failure [0x%x] in [0x%x] at line %d\n",
                status, FileId, Line) ;
#endif  /* #if defined(DDSP_DEBUG) */

    if (ErrReason.IsSet == FALSE) {
        ErrReason.IsSet   = TRUE   ;
        ErrReason.ErrCode = status ;
        ErrReason.FileId  = FileId ;
        ErrReason.LineNum = Line   ;
    }
}


/** ============================================================================
 *  @func   GEN_NumToAscii
 *
 *  @desc   Converts a 1 or 2 digit number to a 2 digit string.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_NumToAscii (IN Uint32 number, OUT Char8 * strNumber)
{
    DSP_STATUS  status = DSP_SOK ;
    Char8       tens   = '0'     ;

    TRC_2ENTER ("GEN_NumToAscii", number, strNumber) ;

    DBC_Require (number <= 99) ;
    DBC_Require (strNumber != NULL) ;

    if ((number > 99) || (strNumber == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;

    }
    else {
        tens = (Char8) (number / 10) ;
        number = number % 10 ;
        if (tens) {
            strNumber [0] = (Char8) (tens + '0') ;
            strNumber [1] = (Char8) (number + '0') ;
            strNumber [2] = '\0' ;
        }
        else {
            strNumber [0] = (Char8) (number + '0') ;
            strNumber [1] = '\0' ;
        }
    }

    DBC_Ensure (strNumber != NULL) ;

    TRC_1LEAVE ("GEN_NumToAscii", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_Strcmp
 *
 *  @desc   Compares 2 ASCII strings.  Works the same way as stdio's strcmp.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Strcmp (IN  CONST Char8 * string1,
            IN  CONST Char8 * string2,
            OUT       Int32 * cmpResult)
{
    DSP_STATUS     status      = DSP_SOK ;
    Int32          charCompare = 0       ;

    TRC_3ENTER ("GEN_Strcmp", string1, string2, cmpResult) ;

    DBC_Require (string1 != NULL) ;
    DBC_Require (string2 != NULL) ;
    DBC_Require (cmpResult != NULL) ;

    if ((string1 == NULL) || (string2 == NULL) || (cmpResult == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        /* iterate for comparing the strings - character by character
         * Note: The comparison takes care of termination of both strings
         *  - by explicitly checking for string termination of string2
         *  - the check for string1 termination is implicit, since when
         *    string1 terminates with a NULL ('\0') the charCompare value
         *    will be negative (if string2 also doesn't terminate then).
         */
        while (  ((charCompare = *(Uint8 *)string1 - *(Uint8 *)string2) == 0)
               && *string2) {
            ++string1, ++string2;
        }

        if (charCompare < 0) {
            charCompare = -1 ;
        }
        else if (charCompare > 0) {
            charCompare = 1 ;
        }

        *cmpResult = charCompare ;
    }

    TRC_1LEAVE ("GEN_Strcmp", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_Strcpyn
 *
 *  @desc   Safe strcpy function.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Strcpyn (OUT Char8 * destination,
             IN  Char8 * source,
             IN  Int32   maxNum)
{
    DSP_STATUS     status = DSP_SOK     ;
    Char8 *        dest   = destination ;
    CONST Char8 *  src    = source      ;

    TRC_3ENTER ("GEN_Strcpyn", destination, source, maxNum) ;

    DBC_Require (destination != NULL) ;
    DBC_Require (source != NULL) ;
    DBC_Require (maxNum >= 0) ;

    if ((destination == NULL) || (source == NULL) || (maxNum < 0)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {

        while (maxNum != 0 && *src) {
            *dest++ = *src++ ;
            maxNum-- ;
        }

        if (maxNum) {
            /* pad out with zeroes */
            while (--maxNum) {
                *dest++ = '\0' ;
            }
        }
        else {
            *dest = '\0' ;
        }
    }

    TRC_1LEAVE ("GEN_Strcpyn", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_Strlen
 *
 *  @desc   Determines the length of a null terminated ASCI string.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Strlen (IN CONST Char8 * strSrc, OUT Uint32 * length)
{
    DSP_STATUS     status = DSP_SOK ;
    CONST Char8 *  str    = strSrc  ;

    TRC_2ENTER ("GEN_Strlen", strSrc, length) ;

    DBC_Require (strSrc != NULL) ;
    DBC_Require (length != NULL) ;

    if ((strSrc == NULL) || (length == NULL)) {
        status = DSP_EINVALIDARG ;
    }
    else {

        while (*str++) ;

        *length = (Uint32) (str - strSrc - 1) ;
    }

    TRC_1LEAVE ("GEN_Strlen", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_WcharToAnsi
 *
 *  @desc   Converts a wide char string to an ansi string.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_WcharToAnsi (OUT Char8 *   destination,
                 IN  Char16 *  source,
                 IN  Int32     numChars)
{
    DSP_STATUS  status = DSP_SOK     ;
    Char8 *     temp   = destination ;

    TRC_3ENTER ("GEN_WcharToAnsi", destination, source, numChars) ;

    DBC_Require (destination != NULL) ;
    DBC_Require (source != NULL) ;
    DBC_Require (numChars >= 0) ;


    if ((source == NULL) || (destination == NULL) || (numChars == 0)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        while ((*source != '\0') && ((numChars--) > 0)) {
            *temp++= (Char8) *source++ ;
        }
        *temp = '\0' ;
    }

    TRC_1LEAVE ("GEN_WcharToAnsi", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_Wstrlen
 *
 *  @desc   Determines the length of a null terminated wide character string.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Wstrlen (IN Char16 * strSrc, OUT Uint32 * length)
{
    DSP_STATUS      status = DSP_SOK ;
    CONST Char16 *  str    = strSrc  ;

    TRC_2ENTER ("GEN_Wstrlen", strSrc, length) ;

    DBC_Require (strSrc != NULL) ;
    DBC_Require (length != NULL );

    if ((strSrc == NULL) || (length == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        while (*str++) ;
        *length = str - strSrc - 1 ;
    }

    TRC_1LEAVE ("GEN_Wstrlen", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_Strcatn
 *
 *  @desc   Safe strcat function.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Strcatn (OUT Char8 * destination,
             IN  Char8 * source,
             IN  Int32   maxNum)
{
    DSP_STATUS     status = DSP_SOK     ;
    Char8 *        dest   = destination ;
    CONST Char8 *  src    = source      ;

    TRC_3ENTER ("GEN_Strcatn", destination, source, maxNum) ;

    DBC_Require (destination != NULL) ;
    DBC_Require (source != NULL) ;
    DBC_Require (maxNum >= 0) ;

    if ((destination == NULL) || (source == NULL) || (maxNum < 0)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        while (maxNum != 0 && *dest) {
            dest++ ;
            maxNum-- ;
        }
        while (maxNum != 0 && *src) {
            *dest++ = *src++ ;
            maxNum-- ;
        }

        if (maxNum) {
            /* pad out with zeroes */
            while (--maxNum) {
                *dest++ = '\0' ;
            }
        }
        else {
            *dest = '\0' ;
        }
    }

    TRC_1LEAVE ("GEN_Strcatn", status) ;

    return status ;
}

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
