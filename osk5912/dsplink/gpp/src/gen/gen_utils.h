/** ============================================================================
 *  @file   gen_utils.h
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Platform independent common library function interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (GEN_H)
#define GEN_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   ErrorInfo
 *
 *  @desc   Structure for storing error reason.
 *
 *  @field  IsSet
 *              Flag to indicate error is set.
 *  @field  ErrCode
 *              Error Code.
 *  @field  OsMajor
 *              OS  Version Major version number.
 *  @field  OsMinor
 *              OS  Version Minor version number.
 *  @field  OsBuild
 *              OS  Version Build number.
 *  @field  PddMajor
 *              PDD Version Major version number.
 *  @field  PddMinor
 *              PDD Version Minor version number.
 *  @field  PddBuild
 *              PDD Version Build number.
 *  @field  FileId
 *              ID of the file where failure occured.
 *  @field  LineNum
 *              Line number where failure occured.
 *  ============================================================================
 */
typedef struct ErrorInfo_tag {
    Bool       IsSet    ;

    DSP_STATUS ErrCode  ;

    Int32      OsMajor  ;
    Int32      OsMinor  ;
    Int32      OsBuild  ;

    Int32      PddMajor ;
    Int32      PddMinor ;
    Int32      PddBuild ;

    Int32      FileId   ;
    Int32      LineNum  ;
} ErrorInfo ;


/** ============================================================================
 *  @func   GEN_Initialize
 *
 *  @desc   Initializes the GEN module's private state.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  None
 *
 *  @leave  Subcomponent is initialized.
 *
 *  @see    GEN_Finalize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Initialize () ;


/** ============================================================================
 *  @func   GEN_Finalize
 *
 *  @desc   Discontinues usage of the GEN module.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  Subcomponent must be initialized.
 *
 *  @leave  None
 *
 *  @see    GEN_Initialize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Finalize () ;


/** ============================================================================
 *  @func   GEN_SetReason
 *
 *  @desc   This function logs failure if no previous failure has been logged.
 *
 *  @arg    status
 *              Error status to be logged.
 *  @arg    FileId
 *              File identifier.
 *  @arg    Line
 *              Line number where error occurred.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
GEN_SetReason (DSP_STATUS status, Int32 FileId, Int32 Line) ;


/** ============================================================================
 *  @func   GEN_NumToAscii
 *
 *  @desc   Converts a 1 or 2 digit number to a 2 digit string.
 *
 *  @arg    number
 *              Number to convert.
 *  @arg    strNumber
 *              Buffer to store converted string.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The number to convert must be between 0 and 99, both numbers
 *          included.
 *          The buffer to store output string must be valid.
 *
 *  @leave  The buffer to store output string is valid.
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_NumToAscii (IN Uint32 number, OUT Char8 * strNumber) ;


/** ============================================================================
 *  @func   GEN_Strcmp
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
GEN_Strcmp (IN  CONST Char8 * string1,
            IN  CONST Char8 * string2,
            OUT       Int32 * cmpResult) ;


/** ============================================================================
 *  @func   GEN_Strcpyn
 *
 *  @desc   Safe strcpy function.
 *
 *  @arg    destination
 *              destination buffer.
 *  @arg    source
 *              Source buffer.
 *  @arg    maxNum
 *              Number of characters to copy.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The destination buffer must be valid.
 *          The source buffer must be valid.
 *          The number of characters to copy must be more than zero.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Strcpyn (OUT Char8 * destination,
             IN  Char8 * source,
             IN  Int32   maxNum) ;


/** ============================================================================
 *  @func   GEN_Strlen
 *
 *  @desc   Determines the length of a null terminated ASCI string.
 *
 *  @arg    str
 *              Pointer to string.
 *  @arg    length
 *              Out parameter to hold the length of string.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The pointer to the string buffer must be valid.
 *          The pointer to the length field must be valid.
 *
 *  @leave  The pointer to the length field is valid.
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Strlen (IN CONST Char8 * str, OUT Uint32 * length) ;


/** ============================================================================
 *  @func   GEN_WcharToAnsi
 *
 *  @desc   Converts a wide char string to an ansi string.
 *
 *  @arg    dest
 *              Destination buffer.
 *  @arg    source
 *              Source buffer.
 *  @arg    numChars
 *              Number of characters (Wide chars) to be converted/copied.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The destination buffer must be valid.
 *          The source buffer must be valid.
 *          The number of characters to be converted/copied must be greater
 *          than 0.
 *
 *  @leave  The destination buffer is valid.
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_WcharToAnsi (OUT Char8  * dest,
                 IN  Char16 * source,
                 IN  Int32    numChars) ;


/** ============================================================================
 *  @func   GEN_Wstrlen
 *
 *  @desc   Determines the length of a null terminated wide character string.
 *
 *  @arg    str
 *              pointer to string.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The pointer to the string buffer must be valid.
 *          The pointer to length of buffer must be valid.
 *
 *  @leave  The pointer to length of buffer is valid.
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Wstrlen (IN Char16 * str, IN Uint32 * length) ;


/** ============================================================================
 *  @func   GEN_Strcatn
 *
 *  @desc   Safe strcat function.
 *
 *  @arg    destination
 *              destination buffer.
 *  @arg    source
 *              Source buffer.
 *  @arg    maxNum
 *              Maximum length of the destination buffer after concatenation.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  The destination buffer must be valid.
 *          The source buffer must be valid.
 *          The number of characters to copy must be more than zero.
 *
 *  @leave  None.
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
GEN_Strcatn (OUT Char8 * destination,
             IN  Char8 * source,
             IN  Int32   maxNum) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (GEN_H) */

