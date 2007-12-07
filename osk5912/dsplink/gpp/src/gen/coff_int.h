/** ============================================================================
 *  @file   coff_int.h
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Defines interface for generic functions and macros of COFF loader.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (COFF_INT_H)
#define COFF_INT_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  SWAP_WORD
 *
 *  @desc   Macro to swap a 16 bit word.
 *  ============================================================================
 */
#define SWAP_WORD(x)   ((((x) << 8) & 0xFF00) | (((x) >> 8) & 0x00FF))

/** ============================================================================
 *  @macro  SWAP_LONG
 *
 *  @desc   Macro to swap a 32 bit dword.
 *  ============================================================================
 */
#define SWAP_LONG(x)   ((((x) << 24) & 0xFF000000)    \
                     |  (((x) <<  8) & 0x00FF0000L)   \
                     |  (((x) >>  8) & 0x0000FF00L)   \
                     |  (((x) >> 24) & 0x000000FFL))

/** ============================================================================
 *  @macro  READ_REC_SIZE
 *
 *  @desc   Record size to be specified when reading file thru' KFILE_Read
 *  ============================================================================
 */
#define READ_REC_SIZE   1


/** ============================================================================
 *  @func   COFF_Read8
 *
 *  @desc   Reads an Int8 from file.
 *
 *  @arg    fileObj
 *              File to read from.
 *
 *  @ret    The read value.
 *
 *  @enter  fileObj must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Int8
COFF_Read8 (IN KFileObject * fileObj) ;


/** ============================================================================
 *  @func   COFF_Read16
 *
 *  @desc   Reads an Int16 from file.
 *
 *  @arg    fileObj
 *              File to read from.
 *  @arg    swap
 *              specifies whether the bytes need to be swapped.
 *
 *  @ret    The read value.
 *
 *  @enter  fileObj must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Int16
COFF_Read16 (IN KFileObject * fileObj, IN Bool swap) ;


/** ============================================================================
 *  @func   COFF_Read32
 *
 *  @desc   Reads an Int32 from file.
 *
 *  @arg    fileObj
 *              File to read from.
 *  @arg    swap
 *              specifies whether the bytes need to be swapped.
 *
 *  @ret    The read value.
 *
 *  @enter  fileObj must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Int32
COFF_Read32 (IN KFileObject * fileObj, IN Bool swap) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (COFF_INT_H) */
