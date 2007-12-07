/** ============================================================================
 *  @file   TST_Helper.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils
 *
 *  @desc   Implemetation of Helper Functions.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <errbase.h>

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


#define TST_PrnDebug(arg)
/** ============================================================================
 *  @func   TST_StrToStatus
 *
 *  @desc   This function returns an appropriate status when
 *          provided with the status in string format.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_StrToStatus (IN Char8 * statusString,
                 OUT DSP_STATUS * statusFromFile)
{
    DSP_STATUS status        = DSP_SOK       ;
    Bool       check         = FALSE         ;
    Int32      cmpResult                     ;
    Char8      statusUppercase [MAX_BUFSIZE] ;

    TST_ToUpper (statusString, statusUppercase) ;

    TST_Strcmp ("DSP_SOK", statusUppercase, &cmpResult) ;
    if (cmpResult == 0) {
        *statusFromFile = DSP_SOK ;
        check = TRUE ;
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EINVALIDARG", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EINVALIDARG ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ERESOURCE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ERESOURCE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_SALREADYATTACHED", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_SALREADYATTACHED ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_SENUMCOMPLETE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_SENUMCOMPLETE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EACCESSDENIED", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EACCESSDENIED ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EALREADYCONNECTED", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EALREADYCONNECTED ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EATTACHED", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EATTACHED ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ECHANGEDURINGENUM", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ECHANGEDURINGENUM ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ECORRUPTFILE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ECORRUPTFILE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EDELETE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EDELETE ;
            check = TRUE ;
        }
    }
    if (check == FALSE) {
        TST_Strcmp ("DSP_EDIRECTION", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EDIRECTION ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ESTREAMFULL", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ESTREAMFULL ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EFAIL", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EFAIL ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EFILE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EFILE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EHANDLE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EHANDLE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EMEMORY", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EMEMORY ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ENOTIMPL", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ENOTIMPL ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EPENDING", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EPENDING ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EPOINTER", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EPOINTER ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ERANGE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ERANGE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ESIZE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ESIZE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ESTREAM", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ESTREAM ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ETASK", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ETASK ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ETIMEOUT", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ETIMEOUT ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ETRUNCATED", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ETRUNCATED ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EVALUE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EVALUE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EWRONGSTATE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EWRONGSTATE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ERESTART", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ERESTART ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EFREE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EFREE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EIOFREE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EIOFREE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EMULINST", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EMULINST ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ENOTFOUND", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ENOTFOUND ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EOUTOFIO", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EOUTOFIO ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ETRANSLATE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ETRANSLATE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_EFWRITE", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_EFWRITE ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("DSP_ENOSECT", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = DSP_ENOSECT ;
            check = TRUE ;
        }
    }

     if (check == FALSE) {
        TST_Strcmp ("CHNL_E_NOIOC", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = CHNL_E_NOIOC ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        TST_Strcmp ("CHNL_E_EOS", statusUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            *statusFromFile = CHNL_E_EOS ;
            check = TRUE ;
        }
    }

    if (check == FALSE) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("This error code not implemented. Status = [0x%x]",
                      status) ;
    }

    return status ;
}


/** ============================================================================
 *  @func   TST_ToLower
 *
 *  @desc   This is a function which converts a upppercase string to lowercase.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_ToLower (IN Char8 * source, OUT Char8 * destination)
{
    DSP_STATUS status = DSP_SOK ;
    Uint32     index            ;

    TST_PrnDebug ("Entered TST_ToLower ()") ;

    DBC_Require (source != NULL) ;
    DBC_Require (destination != NULL) ;

    if ((source == NULL) || (destination == NULL)) {
        status = DSP_EINVALIDARG ;
    }

    for (index = 0 ; source [index] != '\0' ; index++) {
        if ((source [index] >= 'A') && (source [index] <= 'Z')) {
            destination [index] = (Char8) (source [index] + 'a' - 'A') ;
        }
        else {
            destination [index] = source [index] ;
        }
    }
    destination [index] = '\0' ;

    TST_PrnDebug ("Leaving TST_ToLower ()") ;

    return status ;
}


/** ============================================================================
 *  @func   TST_ToUpper
 *
 *  @desc   This is a function which converts a lowercase string to uppercase.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_ToUpper (IN Char8 * source, OUT Char8 * destination)
{
    DSP_STATUS status = DSP_SOK ;
    Uint32     index            ;

    TST_PrnDebug ("Entered TST_ToUpper ()") ;

    DBC_Require (source != NULL) ;
    DBC_Require (destination != NULL) ;

    if ((source == NULL) || (destination == NULL)) {
        status = DSP_EINVALIDARG ;
    }

    for (index=0 ; source [index] != '\0' ;index++) {
        if ((source [index] >= 'a') && (source [index] <= 'z')) {
            destination [index] = (Char8 ) (source [index] + 'A' - 'a') ;
        }
        else {
            destination [index] = source [index] ;
        }
    }
    destination [index] = '\0' ;

    TST_PrnDebug ("Leaving TST_ToUpper () ") ;

    return status ;
}


/** ============================================================================
 *  @func   TST_StringToInt
 *
 *  @desc   It converts a string to an integer, supports positive as well as
 *          negative numbers.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Int32
TST_StringToInt (IN Char8 * string)
{
    DSP_STATUS status = DSP_SOK ;
    Int32      index            ;
    Int32      sum    = 0       ;
    Int32      sign             ;
    Int32      retVal           ;

    TST_PrnDebug ("Entered StringToInt ()") ;

    DBC_Require (string != NULL) ;

    for (index = 0 ; ISSPACE (string [index]) ; index++) ;

    sign = (string [index] == '-') ? -1 : 1 ;

    if ((string [index] == '-') || (string [index] == '+')) {
        index++ ;
    }

    if (!ISDIGIT (string [index])) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Wrong argument to TST_StringToInt. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        for (sum = 0 ; ISDIGIT (string [index]) ; index++) {
            sum = 10 * sum + (string [index] - '0') ;
        }
    }

    retVal = sign * sum ;

    TST_PrnDebug ("Leaving StringToInt ()") ;

    return retVal ;
}


/** ============================================================================
 *  @func   TST_IntToString
 *
 *  @desc   It converts a integer to an string, supports positive as well as
 *          negative numbers.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_IntToString (IN Int16 num, OUT Char8 * destination)
{
    /*  ------------------------------------------------------------------------
     *  There can be a max of 7 char str for signed int including terminator
     *  so we use a temp string of 7 char. The index is to the rightmost space
     *  ie. 5. We divide the number by 10, and successively store the remainder
     *  in the rightmost space while decrementing the counter. Later we add the
     *  sign if required.
     *  ------------------------------------------------------------------------
     */
    DSP_STATUS status    = DSP_SOK                        ;
    Int16      sign      = 0                              ;
    Char8      remainder = 0                              ;
    char       temp [7]  = {' ',' ',' ',' ',' ',' ','\0'} ;
    Uint16     index     = 5                              ;

    TST_PrnDebug ("Entered IntToString ()") ;
    DBC_Require (destination != NULL) ;

    if (destination == NULL) {
        status = DSP_EINVALIDARG ;
    }

    sign = (num < 0) ? -1 : 1 ;
    num = num * sign ;

    do {
        remainder = num % 10 ;
        temp [index] = '0' + remainder ;
        num = num / 10 ;
        index-- ;
    } while (num > 0) ;

    if (sign == -1) {
        temp [index] = '-' ;
    }
    else {
        index++ ;
    }

    TST_ToUpper (&(temp [index]), destination) ;

    TST_PrnDebug ("Leaving IntToString ()") ;

    return status ;
}


/** ============================================================================
 *  @func   TST_MacroToIntValue
 *
 *  @desc   Compares str with macroName. If they match, assign macroVal to var.
 *          Works on macros with integer values.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_MacroToIntValue (OUT Uint32 * var,
                     IN  Uint32   macroVal,
                     IN  Char8 *  str,
                     IN  Char8 *  macroName)
{
    DSP_STATUS status = DSP_SOK ;
    Int32      cmp ;

    DBC_Require (str != NULL) ;
    DBC_Require (macroName != NULL) ;
    DBC_Require (var != NULL) ;

    status = TST_Strcmp (str, macroName, &cmp) ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("TST_MacroToIntValue. Status = [0x%x]", status) ;
    }
    else if (cmp == 0) {
        *var = macroVal ;
    }
    else {
        *var = 0 ;
        status = DSP_ENOTFOUND ; /* Not the specified macro value */
    }
    return status ;
}


/** ============================================================================
 *  @func   TST_Strcmp
 *
 *  @desc   Compares 2 ASCII strings.  Works the same way as stdio's strcmp.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_Strcmp (IN  CONST Char8 * string1,
            IN  CONST Char8 * string2,
            OUT       Int32 * cmpResult)
{
    DSP_STATUS     status      = DSP_SOK ;
    Int32          charCompare = 0       ;

    TST_PrnDebug ( "Entered TST_Strcmp ()\n") ;

    DBC_Require (string1 != NULL) ;
    DBC_Require (string2 != NULL) ;
    DBC_Require (cmpResult != NULL) ;

    if ((string1 == NULL) || (string2 == NULL) || (cmpResult == NULL)) {
        status = DSP_EINVALIDARG ;
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

    TST_PrnDebug ("Leaving TST_Strcmp ()\n") ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
