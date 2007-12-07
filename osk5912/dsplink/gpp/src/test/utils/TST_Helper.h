/** ============================================================================
 *  @file   TST_Helper.h
 *
  *  @path   $(DSPLINK)\gpp\src\test\utils
 *
 *  @desc   Defines the interface of Helper Functions.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_HELPER_H)
#define TST_HELPER_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   ISSPACE
 *
 *  @desc   Tests for the whitespace characters.
 *  ============================================================================
 */
#define ISSPACE(a) (   ((a) == ' ')  || ((a) == '\t')                          \
                    || ((a) == '\n') || ((a) == '\r'))

/** ============================================================================
 *  @name   ISDIGIT
 *
 *  @desc   Tests for the digits.
 *  ============================================================================
 */
#define ISDIGIT(a) (((a) >= '0') && ((a) <= '9'))


/** ============================================================================
 *  @func   TST_StrToStatus
 *
 *  @desc   This function returns an appropriate status when
 *          provided with the status in string format.
 *
 *  @arg    statusString
 *              The status in string format.
 *  @arg    statusFromFile
 *              Placeholder for DSP_STATUS corresponding to the string passed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Operation failed due to invalid arguments.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_StrToStatus (IN  Char8 *      statusString,
                 OUT DSP_STATUS * statusFromFile) ;


/** ============================================================================
 *  @func   TST_ToLower
 *
 *  @desc   This is a function which converts a uppercase string to lowercase.
 *
 *  @arg    source
 *              This argument is the string in the lowercase which is to be
 *              converted into uppercase
 *  @arg    destination
 *              This is the buffer which is provided by the user.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Operation failed due to invalid arguments.
 *
 *  @enter  source should not be NULL.
 *          destination should not be NULL.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_ToLower (IN Char8 * source, OUT Char8 * destination) ;


/** ============================================================================
 *  @func   TST_ToUpper
 *
 *  @desc   This is a function which converts a lowercase string to uppercase.
 *
 *  @arg    source
 *              This argument is the string in the lowercase which is to be
 *              converted into uppercase
 *  @arg    destination
 *              This is the buffer which is provided by the user.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Operation failed due to invalid arguments.
 *
 *  @enter  source should not be null
 *          destination should not be null
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_ToUpper (IN Char8 * source, OUT Char8 * destination) ;


/** ============================================================================
 *  @func   TST_StringToInt
 *
 *  @desc   It converts a string to an integer, supports positive as well as
 *          negative numbers.
 *
 *  @arg    string
 *              The string which is to be converted into a number.
 *
 *  @ret    The integer value of the string.
 *
 *  @enter  string should not be NULL.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Int32
TST_StringToInt (IN Char8 * string) ;

/** ============================================================================
 *  @func   TST_IntToString
 *
 *  @desc   It converts a integer to an string, supports positive as well as
 *          negative numbers.
 *
 *  @arg    num
 *              The number that neesd to be converted to string.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Operation failed due to invalid arguments.
 *
 *  @enter  destination should not be NULL.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_IntToString (IN Int16 num, OUT Char8 * destination) ;


/** ============================================================================
 *  @func   TST_MacroToIntValue
 *
 *  @desc   Compares str with macroName. If they match, assign macroVal to var.
 *          Works on macros with integer values.
 *
 *  @arg    string
 *              The string which is to be converted into a number.
 *
 *  @ret    DSP_SOK
 *              Strings matched and the value was assigned successfully.
 *          DSP_EFAIL
 *              String did not match or there was error during comparison.
 *
 *  @enter  str and macroName should not be NULL
 *          var should not be NULL
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_MacroToIntValue (OUT Uint32 * var,
                     IN  Uint32   macroVal,
                     IN  Char8 *  str,
                     IN  Char8 *  macroName) ;


/** ============================================================================
 *  @func   TST_Strcmp
 *
 *  @desc   Compares 2 ASCII strings.  Works the same way as stdio's strcmp.
 *
 *  @arg    string1
 *              First string for comparison.
 *  @arg    string2
 *              Second string for comparison.
 *  @arg    cmpResult
 *              Result of comparision (zero = equal, non-zero otherwise).
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The buffer to store first string must be valid.
 *          The buffer to store second string must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_Strcmp (IN  CONST Char8 * string1,
            IN  CONST Char8 * string2,
            OUT       Int32 * cmpResult) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TST_HELPER_H) */
