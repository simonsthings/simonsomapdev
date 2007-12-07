/** ============================================================================
 *  @file   coff.c
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Implements the DSP COFF loader.
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
#include <intobject.h>
#include <errbase.h>
#include <safe.h>

#include <cfgdefs.h>
#include <dspdefs.h>

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                   */
#include <profile.h>

/*  ----------------------------------- OSAL Headers                */
#include <cfg.h>
#include <kfile.h>
#include <mem.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>
#include <coff_int.h>
#include <coff.h>
#include <coff_55x.h>
#include <coff_64x.h>

/*  ----------------------------------- Link Driver                 */
#include <ldrv_proc.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID         ID_GEN_COFF

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define  SET_FAILURE_REASON   GEN_SetReason (status, FID_C_GEN_COFF, __LINE__)

/** ============================================================================
 *  @macro  HasNullChar
 *
 *  @desc   Macro to return TRUE if any byte in an Int32 is NULL.
 *  ============================================================================
 */
#define HasNullChar(val) (   (((val) & 0xff000000) == 0) \
                          || (((val) & 0x00ff0000) == 0) \
                          || (((val) & 0x0000ff00) == 0) \
                          || (((val) & 0x000000ff) == 0))

/** ============================================================================
 *  @macro  IS_LOADABLE_SECTION
 *
 *  @desc   Macro to find if the section is lodable.
 *  ============================================================================
 */
#define IS_LOADABLE_SECTION(x) (!(   ((x.flags & (  SECT_NOLOAD       \
                                                  | SECT_DSECT        \
                                                  | SECT_BSS          \
                                                  | SECT_COPY)) > 0)  \
                                  || (x.size == 0)                    \
                                  || (x.fpRawData == 0)))

/** ============================================================================
 *  @name   COFF_Interface
 *
 *  @desc   Interface table for COFF loader.
 *  ============================================================================
 */
LoaderInterface COFF_Interface = {
    &COFF_Load,
    &COFF_LoadSection
} ;


#if defined (DDSP_DEBUG)
/*  ============================================================================
 *  @func   COFF_Debug
 *
 *  @desc   Prints the debug information of COFF sub=module.
 *
 *  @arg    obj
 *              The context object obtained through COFF_Initialize.
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
NORMAL_API
Void
COFF_Debug (IN CoffContext *   obj) ;
#endif /* defined (DDSP_DEBUG) */


/** ----------------------------------------------------------------------------
 *  @func   COFF_Initialize
 *
 *  @desc   Initializes the CoffContext object for loading a base image file or
 *          a section. This function is required to be called before any other
 *          function is called from this sub component.
 *
 *  @arg    procId
 *              Processor Identifier for the target DSP.
 *  @arg    file
 *              Identifier for the file.
 *  @arg    dspArch
 *              Architecture of the dsp.
 *  @arg    obj
 *              OUT argument that contains the object required to be passed
 *              in any subsequent call from this sub-component.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFILE
 *              File not found.
 *          DSP_EMEMORY
 *              Out of memory error.
 *
 *  @enter  procId must be valid.
 *          file must not be NULL.
 *          obj must not be NULL.
 *
 *  @leave  None
 *
 *  @see    COFF_Finalize
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_Initialize (IN  ProcessorId     procId,
                 IN  Pstr            file,
                 IN  DspArch         dspArch,
                 OUT CoffContext *   obj) ;


/** ----------------------------------------------------------------------------
 *  @func   COFF_Finalize
 *
 *  @desc   Deallocates the object(s) created by COFF_Initialize function and
 *          releases the context.
 *
 *  @arg    objCtx
 *              The context object obtained through COFF_Initialize.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFILE
 *              File is not open.
 *          DSP_EPOINTER
 *              Invalid file object.
 *
 *  @enter  objCtx must be valid.
 *
 *  @leave  None
 *
 *  @see    COFF_Initialize
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_Finalize (IN  CoffContext * objCtx) ;


/** ----------------------------------------------------------------------------
 *  @func   COFF_SeekToSectionHeader
 *
 *  @desc   Repositions the file position indicator to the section header.
 *
 *  @arg    fileObj
 *              Handle to the COFF file.
 *  @arg    sectIndex
 *              Section  Index.
 *  @arg    swap
 *              Flag to indicate that headers in this file are swapped.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_RANGE
 *              Seek error in file.
 *
 *  @enter  fileObj must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_SeekToSectionHeader (IN KFileObject * fileObj,
                          IN Uint32        sectIndex,
                          IN Bool          swap) ;


/** ----------------------------------------------------------------------------
 *  @func   COFF_IsSwapped
 *
 *  @desc   Checks if the fields of headers are stored as byte
 *          swapped values.
 *
 *  @arg    fileObj
 *              Handle to the COFF file.
 *  @arg    dspArch
 *              Architecture of the dsp.
 *  @arg    isSwapped
 *              OUT argument to contain if the COFF headers in file are swapped.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_RANGE
 *              Seek error in file.
 *
 *  @enter  fileObj must be a valid pointer.
 *          isSwapped must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_IsSwapped (IN  KFileObject * fileObj,
                IN  DspArch       dspArch,
                OUT Bool *        isSwapped) ;


/** ----------------------------------------------------------------------------
 *  @func   COFF_IsValidFile
 *
 *  @desc   Checks to indicate if the file data format is valid for the given
 *          architecture.
 *
 *  @arg    fileObj
 *              Handle to the COFF file.
 *  @arg    dspArch
 *              Architecture of the dsp.
 *  @arg    isValid
 *              OUT argument to contain if the file data format is valid
 *              for the given architecture.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_RANGE
 *              Seek error in file.
 *
 *  @enter  fileObj must be a valid pointer.
 *          isValid must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_IsValidFile (IN  KFileObject * fileObj,
                  IN  DspArch       dspArch,
                  OUT Bool *        isValid) ;



/** ----------------------------------------------------------------------------
 *  @func   COFF_FillArgsBuffer
 *
 *  @desc   Fills up the specified buffer with arguments to be sent to DSP's
 *          "main" function for the given architecture.
 *
 *  @arg    dspArch
 *              Architecture of the dsp.
 *  @arg    argc
 *              Number of arguments to be passed.
 *  @arg    argv
 *              Argument strings to be passed.
 *  @arg    size
 *              Size of the '.args' section obtained from the COFF file.
 *  @arg    loadAddr
 *              Load address for the '.args' section.
 *  @arg    wordSize
 *              Wordsize on the target DSP.
 *  @arg    endian
 *              Endianism to be used while writing data.
 *  @arg    argsBuf
 *              Buffer to be filled with formatted argc and argv.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_ESIZE
 *              Insufficient space in .args buffer to hold all the arguments.
 *          DSP_EMEMORY
 *              Out of memory error.
 *
 *  @enter  argc must be greater than 0.
 *          argv must be valid pointer.
 *          argsBuf must be a valid pointer.
 *          sizeBuf must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_FillArgsBuffer (IN  DspArch   dspArch,
                     IN  Uint32    argc,
                     IN  Char8 **  argv,
                     IN  Uint32    sectSize,
                     IN  Uint32    loadAddr,
                     IN  Uint32    wordSize,
                     IN  Endianism endian,
                     IN  Void *    argsBuf) ;


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetOptHeaderSize
 *
 *  @desc   Get the size of optional header in file. This function is used at
 *          many places to quickly seek to the desired field in file.
 *
 *  @arg    fileObj
 *              Handle to the COFF file.
 *  @arg    swap
 *              specifies whether the bytes need to be swapped.
 *  @arg    size
 *              OUT argument to contain the optional header size.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_RANGE
 *              Seek error in file.
 *
 *  @enter  fileObj must be a valid pointer.
 *          size must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetOptHeaderSize (IN  KFileObject * fileObj,
                       IN  Bool          swap,
                       OUT Int32 *       size) ;


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetSymTabDetails
 *
 *  @desc   Gets the details associated to the symbol table -
 *          i.e. number of symbols in the file and the offset of symbol table
 *          in file.
 *
 *  @arg    fileObj
 *              Handle to the COFF file.
 *  @arg    swap
 *              Specifies whether the bytes need to be swapped.
 *  @arg    offsetSymTab
 *              OUT argument to contain the offset of symbol table.
 *  @arg    numSymbols
 *              OUT argument to contain the number of symbols.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_RANGE
 *              Seek error in file.
 *
 *  @enter  fileObj must be a valid pointer.
 *          offsetSymTab must be a valid pointer.
 *          numSymbols must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetSymTabDetails (IN  KFileObject * fileObj,
                       IN  Bool          swap,
                       OUT Uint32 *      offsetSymTab,
                       OUT Uint32 *      numSymbols) ;


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetNumSections
 *
 *  @desc   Get the total number of sections in file.
 *
 *  @arg    fileObj
 *              Handle to the COFF file.
 *  @arg    swap
 *              specifies whether the bytes need to be swapped.
 *  @arg    numSections
 *              OUT argument to contain the number of sections.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_RANGE
 *              Seek error in file.
 *
 *  @enter  fileObj must be valid pointer.
 *          numSections must be valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetNumSections (IN  KFileObject * fileObj,
                     IN  Bool          swap,
                     OUT Uint32 *      numSections) ;

/** ----------------------------------------------------------------------------
 *  @func   COFF_GetFileHeader
 *
 *  @desc   Gets the File Header information.
 *          The caller should allocate memory for file header.
 *
 *  @arg    obj
 *              The context object obtained through COFF_Initialize.
 *  @arg    fileHeader
 *              OUT argument for containing file header information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *          DSP_ERANGE
 *              File seek operation failed.
 *          DSP_EFILE
 *              File format not supported.
 *
 *  @enter  obj must be a valid pointer.
 *          fileHeader must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetFileHeader (IN  CoffContext  *    obj,
                    OUT CoffFileHeader *  fileHeader) ;

/** ----------------------------------------------------------------------------
 *  @func   COFF_GetOptionalHeader
 *
 *  @desc   Gets the COFF file's optional header.
 *          The caller should allocate memory for optional header.
 *
 *  @arg    obj
 *              The context object obtained through COFF_Initialize.
 *  @arg    optHeader
 *              OUT argument for containing optional header information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *          DSP_ERANGE
 *              File seek operation failed.
 *          DSP_EFILE
 *              No optional header in file.
 *
 *  @enter  obj must be valid.
 *          optHeader must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetOptionalHeader (IN  CoffContext   *  obj,
                        OUT CoffOptHeader *  optHeader) ;


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetSectionHeader
 *
 *  @desc   Gets the header information for a section.
 *          The caller should allocate memory for section header.
 *
 *  @arg    sectIndex
 *              Section index.
 *  @arg    obj
 *              The context object obtained through COFF_Initialize.
 *  @arg    sectHeader
 *              OUT argument containing section header.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *          DSP_ERANGE
 *              File seek operation failed.
 *          DSP_EFILE
 *              File format not supported.
 *
 *  @enter  obj must be valid
 *          sectHeader must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetSectionHeader (IN  Uint32               sectId,
                       IN  CoffContext  *       obj,
                       OUT CoffSectionHeader *  sectHeader) ;


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetSectionData
 *
 *  @desc   Gets the data associated with a section.
 *          Memory for buffer should be allocated prior to invoking this
 *          function.
 *
 *  @arg    sectIndex
 *              Section index.
 *  @arg    obj
 *              The context object obtained through COFF_Initialize.
 *  @arg    data
 *              OUT argument containing data buffer associated with the section.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *          DSP_ERANGE
 *              File seek operation failed.
 *          DSP_EFILE
 *              File format not supported.
 *
 *  @enter  obj must be a valid pointer.
 *          data must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetSectionData (IN  Uint32         sectId,
                     IN  CoffContext  * obj,
                     OUT Char8 *        data) ;


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetString
 *
 *  @desc   Gets the string from string table if required. This function
 *          checks if the 'str' argument is a valid string, if not, it looks
 *          up the string in string-table.
 *          Memory for string is allocated by this function.
 *
 *  @arg    str
 *              Contains the string or the string offset.
 *  @arg    obj
 *              The context object obtained through COFF_Initialize.
 *  @arg    outStr
 *              OUT argument containing the string.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *          DSP_EFILE
 *              File format not supported.
 *          DSP_ERANGE
 *              File seek operation failed.
 *          DSP_EMEMORY
 *              Out of memory.
 *
 *  @enter  obj must be a valid pointer.
 *          str must be a valid pointer.
 *          outStr must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetString (IN  Char8 *        str,
                IN  CoffContext *  obj,
                OUT Char8 **       outStr) ;


/** ============================================================================
 *  @func   COFF_GetSymbolTable
 *
 *  @desc   Get the primary SymbolEntry for all the symbols in the coff file.
 *          The caller should allocate memory for the symbol table.
 *
 *  @arg    obj
 *              The context object obtained through COFF_Initialize.
 *  @arg    symTable
 *              OUT argument for holding the symbol table.
 *  @arg    numSymbols
 *              OUT argument for holding the actual number of distinct symbols
 *              present in the file.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *          DSP_EFILE
 *              File format not supported.
 *          DSP_ERANGE
 *              File seek operation failed.
 *          DSP_EMEMORY
 *              Out of memory.
 *
 *  @enter  obj must be a valid pointer.
 *          symTable must be a valid pointer.
 *          numSymbols must be a valid poitner.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_GetSymbolTable (IN  CoffContext  *     obj,
                     OUT CoffSymbolEntry ** symTable,
                     OUT Uint32           * numSymbols) ;


/** ============================================================================
 *  @func   COFF_Load
 *
 *  @desc   Loads the Coff format file on the DSP. This is called by
 *          PMGR_PROC_Load through the function pointer table.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_Load (IN  ProcessorId     procId,
           IN  LoaderObject *  loaderObj,
           IN  Uint32          argc,
           IN  Char8 **        argv,
           OUT Uint32 *        entryPt)
{
    DSP_STATUS        status         = DSP_SOK ;
    DSP_STATUS        tempStatus     = DSP_SOK ;
    Int32             cmpResult      = -1      ;
    Bool              loadedArgs     = FALSE   ;
    Char8      *      sectName       = NULL    ;
    CoffContext       obj                      ;
    CoffFileHeader    fileHeader               ;
    CoffOptHeader     optHeader                ;
    CoffSectionHeader sectHeader               ;
    Uint32            i                        ;

    TRC_5ENTER ("COFF_Load",
                procId,
                loaderObj,
                argc,
                argv,
                entryPt) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (loaderObj != NULL) ;
    DBC_Require (entryPt != NULL) ;
    DBC_Require (   ((argc == 0) && (argv == NULL))
                 || ((argc != 0) && (argv != NULL))) ;

    DBC_Assert (loaderObj->baseImage != NULL) ;

    status = COFF_Initialize (procId,
                              loaderObj->baseImage,
                              loaderObj->dspArch,
                              &obj) ;

    if (DSP_SUCCEEDED (status)) {
        status = COFF_GetFileHeader (&obj, &fileHeader) ;

        if (DSP_SUCCEEDED (status)) {
            status = COFF_GetOptionalHeader (&obj, &optHeader) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }

        for (i = 0 ;   ( DSP_SUCCEEDED (status))
                    && (i < fileHeader.numSections) ; i++) {

            /*  ----------------------------------------------------------------
             *  A DSP executable image can contains some sections that
             *  are not-loadable.
             *  So check if the section is a loadable section and contains
             *  data to be written.
             *  ----------------------------------------------------------------
             */
            status = COFF_GetSectionHeader (i, &obj, &sectHeader) ;
            if (DSP_SUCCEEDED (status)) {
                if ((argc > 0) && (loadedArgs == FALSE)) {
                    status = COFF_GetString (sectHeader.name, &obj, &sectName) ;
                }
                if (DSP_SUCCEEDED (status)) {
                    if (IS_LOADABLE_SECTION (sectHeader)) {
                        sectHeader.isLoadSection = TRUE ;
                    }
                    else {
                        sectHeader.isLoadSection = FALSE ;
                    }
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }

            if (   DSP_SUCCEEDED (status)
                && (sectHeader.isLoadSection)) {

                status = MEM_Alloc ((Void **) &(sectHeader.data),
                                    sectHeader.size,
                                    MEM_DEFAULT) ;

                if (DSP_SUCCEEDED (status)) {
                    if ((argc > 0) && (loadedArgs == FALSE)) {
                        status = GEN_Strcmp (sectName, ".args", &cmpResult) ;

                        if ((DSP_SUCCEEDED (status)) && (cmpResult == 0)) {
                            loadedArgs = TRUE ;
                            /* If the section name is .args then load
                             * the user arguments */
                            status = COFF_FillArgsBuffer
                                        (loaderObj->dspArch,
                                         argc,
                                         argv,
                                         sectHeader.size,
                                         sectHeader.virtualAddress,
                                         loaderObj->wordSize,
                                         loaderObj->endian,
                                         sectHeader.data) ;
                            if (DSP_FAILED (status)) {
                                SET_FAILURE_REASON ;
                            }
                        }
                        else if ((DSP_SUCCEEDED (status)) && (cmpResult != 0)) {
                            status = COFF_GetSectionData (i,
                                                          &obj,
                                                          sectHeader.data) ;
                            if (DSP_FAILED (status)) {
                                SET_FAILURE_REASON ;
                            }
                        }
                        else {
                            SET_FAILURE_REASON ;
                        }
                    }
                    else {
                        status = COFF_GetSectionData (i,
                                                      &obj,
                                                      sectHeader.data) ;

                        if (DSP_FAILED (status)) {
                            SET_FAILURE_REASON ;
                        }
                    }

                    if (DSP_SUCCEEDED (status)) {
                        status = LDRV_PROC_Write (procId,
                                                  sectHeader.virtualAddress,
                                                  Endianism_Default,
                                                  sectHeader.size,
                                                  (Uint8 *) sectHeader.data) ;

                        if (DSP_FAILED (status)) {
                            SET_FAILURE_REASON ;
                        }
                    }

                    tempStatus = FREE_PTR (sectHeader.data) ;
                    if (DSP_FAILED (tempStatus) && DSP_SUCCEEDED (status)) {
                        status = tempStatus ;
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }

            if (sectName != NULL) {
                tempStatus = FREE_PTR (sectName) ;
                sectName = NULL ;
                if (DSP_FAILED (tempStatus) && DSP_SUCCEEDED (status)) {
                    status = tempStatus ;
                    SET_FAILURE_REASON ;
                }
            }
        }

        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        else {
            *entryPt = optHeader.entry ;

            tempStatus = COFF_Finalize (&obj) ;
            if (DSP_FAILED (tempStatus) && DSP_SUCCEEDED (status)) {
                status = tempStatus ;
                SET_FAILURE_REASON ;
            }
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("COFF_Load", status) ;

    return status ;
}


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   COFF_Debug
 *
 *  @desc   Prints the debug information of COFF sub=module.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
COFF_Debug (IN CoffContext *   obj)
{
    TRC_0ENTER ("COFF_Debug") ;

    DBC_Require (obj != NULL) ;

    if (obj != NULL) {
        TRC_1PRINT (TRC_LEVEL1, "   fileObj  :    [0x%x]\n", obj->fileObj) ;
        TRC_1PRINT (TRC_LEVEL1, "   startAddr:    [0x%x]\n", obj->startAddr) ;
        TRC_1PRINT (TRC_LEVEL1, "   isSwapped:    [0x%x]\n", obj->isSwapped) ;
    }

    TRC_0LEAVE ("COFF_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


/** ============================================================================
 *  @func   COFF_LoadSection
 *
 *  @desc   Loads a section from the DSP executable onto the DSP. This is called
 *          by PMGR_PROC_LoadSection through the function pointer table.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_LoadSection (IN  ProcessorId     procId,
                  IN  LoaderObject *  loaderObj,
                  IN  Uint32          sectId)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("COFF_LoadSection", procId, loaderObj, sectId) ;

    status = DSP_ENOTIMPL ;
    SET_FAILURE_REASON ;

    TRC_1LEAVE ("COFF_LoadSection", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_Initialize
 *
 *  @desc   Initializes the CoffContext object for loading a base image file or
 *          a section. This function is required to be called before any other
 *          function is called from this sub component.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_Initialize (IN  ProcessorId     procId,
                 IN  Pstr            file,
                 IN  DspArch         dspArch,
                 OUT CoffContext *   obj)
{
    DSP_STATUS      status      = DSP_SOK ;
    KFileObject *   fileObj     = NULL    ;
    CONST Char8 *   mode        = "r"     ;
    Bool            isValidArch = FALSE   ;
    Bool            isSwapped   = FALSE   ;

    TRC_4ENTER ("COFF_Initialize", procId, file, dspArch, obj) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (file != NULL) ;
    DBC_Require (obj != NULL) ;

    status = KFILE_Open (file, mode, &fileObj) ;
    if (DSP_SUCCEEDED (status)) {
        obj->fileObj   = fileObj ;
        obj->startAddr = 0       ;
        obj->isSwapped = FALSE   ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = COFF_IsValidFile (fileObj, dspArch, &isValidArch) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        else if (isValidArch == FALSE) {
            status = DSP_ECORRUPTFILE ;
            SET_FAILURE_REASON ;
        }
        else {
            status = COFF_IsSwapped (fileObj, dspArch, &isSwapped) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
            else if (isSwapped == TRUE) {
                obj->isSwapped = TRUE ;
            }
        }
        if (DSP_FAILED (status)) {
            KFILE_Close (fileObj) ;
        }
    }

    TRC_1LEAVE ("COFF_Initialize", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_Finalize
 *
 *  @desc   Deallocates the object(s) created by COFF_Initialize function and
 *          releases the context.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_Finalize (IN  CoffContext * objCtx)
{
    DSP_STATUS status = DSP_SOK ;

    DBC_Require (objCtx != NULL) ;

    TRC_1ENTER ("COFF_Finalize", objCtx) ;

    if ((objCtx != NULL) && (objCtx->fileObj != NULL)) {
        status = KFILE_Close (objCtx->fileObj) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        objCtx->fileObj    = NULL ;
        objCtx->startAddr  = 0     ;
        objCtx->isSwapped  = FALSE ;
    }

    TRC_1LEAVE ("COFF_Finalize", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetFileHeader
 *
 *  @desc   Gets the file header from the coff file.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetFileHeader (IN  CoffContext *     obj,
                    OUT CoffFileHeader *  fileHeader)
{
    DSP_STATUS    status  = DSP_SOK ;
    Bool          swap    = FALSE   ;
    KFileObject * fileObj = NULL    ;

    TRC_2ENTER ("COFF_GetFileHeader", obj, fileHeader) ;

    DBC_Require (obj != NULL) ;
    DBC_Require (fileHeader != NULL) ;

    if ((obj == NULL) || (fileHeader == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        swap = obj->isSwapped ;
        fileObj = obj->fileObj ;
        /* Read the file header from the start of the file.*/
        status = KFILE_Seek (fileObj, 0, KFILE_SeekSet) ;
        if (DSP_SUCCEEDED (status)) {
            fileHeader->version           = COFF_Read16 (fileObj, swap) ;
            fileHeader->numSections       = COFF_Read16 (fileObj, swap) ;
            fileHeader->dateTime          = COFF_Read32 (fileObj, swap) ;
            fileHeader->fpSymTab          = COFF_Read32 (fileObj, swap) ;
            fileHeader->numSymTabEntries  = COFF_Read32 (fileObj, swap) ;
            fileHeader->numBytesOptHeader = COFF_Read16 (fileObj, swap) ;
            fileHeader->flags             = COFF_Read16 (fileObj, swap) ;
            fileHeader->targetId          = COFF_Read16 (fileObj, swap) ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("COFF_GetFileHeader", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetOptionalHeader
 *
 *  @desc   Gets the optional header information from the COFF file.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetOptionalHeader (IN  CoffContext  *   obj,
                        OUT CoffOptHeader *  optHeader)
{
    DSP_STATUS    status            = DSP_SOK ;
    Bool          swap              = FALSE   ;
    Int32         numBytesOptHeader = 0       ;
    KFileObject * fileObj           = NULL    ;

    TRC_2ENTER ("COFF_GetOptionalHeader", obj, optHeader) ;

    DBC_Require (obj != NULL) ;
    DBC_Require (optHeader != NULL) ;

    if ((obj == NULL) || (optHeader == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        swap    = obj->isSwapped ;
        fileObj = obj->fileObj   ;
        status = COFF_GetOptHeaderSize (fileObj, swap, &numBytesOptHeader) ;

        if (DSP_SUCCEEDED (status)) {
            if (numBytesOptHeader > 0) {
                /* Seek to optional header offset */
                status = KFILE_Seek (fileObj,
                                     SIZE_COFF_FILE_HEADER,
                                     KFILE_SeekSet) ;

                if (DSP_SUCCEEDED (status)) {
                    optHeader->magic          = COFF_Read16 (fileObj, swap) ;
                    optHeader->version        = COFF_Read16 (fileObj, swap) ;
                    optHeader->sizeExeCode    = COFF_Read32 (fileObj, swap) ;
                    optHeader->sizeInitData   = COFF_Read32 (fileObj, swap) ;
                    optHeader->sizeUninitData = COFF_Read32 (fileObj, swap) ;
                    optHeader->entry          = COFF_Read32 (fileObj, swap) ;
                    optHeader->addrExe        = COFF_Read32 (fileObj, swap) ;
                    optHeader->addrInitData   = COFF_Read32 (fileObj, swap) ;
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }
            else {
                status = DSP_EFILE ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("COFF_GetOptionalHeader", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetSectionHeader
 *
 *  @desc   Gets the header information for the specified section
 *          from the given coff file.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetSectionHeader (IN  Uint32               sectId,
                       IN  CoffContext  *       obj,
                       OUT CoffSectionHeader *  sectHeader)
{
    DSP_STATUS    status    = DSP_SOK ;
    Bool          swap      = FALSE   ;
    KFileObject * fileObj   = NULL    ;
    Int32         i                   ;

    TRC_3ENTER ("COFF_GetSectionHeader", sectId, obj, sectHeader) ;

    DBC_Require (obj != NULL) ;
    DBC_Require (sectHeader != NULL) ;

    if ((obj == NULL) || (sectHeader == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        swap    = obj->isSwapped ;
        fileObj = obj->fileObj   ;
        status = COFF_SeekToSectionHeader (fileObj, sectId, swap) ;
        if (DSP_SUCCEEDED (status)) {
            for (i = 0 ; i < COFF_NAME_LEN ; i++) {
                sectHeader->name [i] = COFF_Read8 (fileObj) ;
            }
            sectHeader->physicalAddress = COFF_Read32 (fileObj, swap) ;
            sectHeader->virtualAddress  = COFF_Read32 (fileObj, swap) ;
            sectHeader->size            = COFF_Read32 (fileObj, swap) ;
            sectHeader->fpRawData       = COFF_Read32 (fileObj, swap) ;
            sectHeader->fpReloc         = COFF_Read32 (fileObj, swap) ;
            sectHeader->fpLineNum       = COFF_Read32 (fileObj, swap) ;
            sectHeader->numReloc        = COFF_Read32 (fileObj, swap) ;
            sectHeader->numLine         = COFF_Read32 (fileObj, swap) ;
            sectHeader->flags           = COFF_Read32 (fileObj, swap) ;
            sectHeader->reserved        = COFF_Read16 (fileObj, swap) ;
            sectHeader->memPageNum      = COFF_Read16 (fileObj, swap) ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("COFF_GetSectionHeader", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetSectionData
 *
 *  @desc   Gets the section data of the specified section from the COFF file.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetSectionData (IN  Uint32         sectId,
                     IN  CoffContext  * obj,
                     OUT Char8 *        data)
{
    DSP_STATUS    status         = DSP_SOK ;
    Int32         sizeOffset     = 0       ;
    Bool          swap           = FALSE   ;
    KFileObject * fileObj        = NULL    ;
    Uint32        sectSize                 ;
    Uint32        sectDataOffset           ;

    TRC_3ENTER ("COFF_GetSectionData", sectId, obj, data) ;

    DBC_Require (obj != NULL) ;
    DBC_Require (data != NULL) ;

    if ((obj == NULL) || (data == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        swap    = obj->isSwapped ;
        fileObj = obj->fileObj   ;
        status = COFF_SeekToSectionHeader (fileObj, sectId, swap) ;
        if (DSP_SUCCEEDED (status)) {
            /*  ------------------------------------------------------------
             *  sizeOffset is calculated by taking the length of
             *  section name, the section's physical address
             *  and its virtual address.
             *  ------------------------------------------------------------
             */
            sizeOffset =  (sizeof (Char8) * COFF_NAME_LEN)
                         + sizeof (Int32)
                         + sizeof (Int32) ;
            status = KFILE_Seek (fileObj, sizeOffset, KFILE_SeekCur) ;

            if (DSP_SUCCEEDED (status)) {
                sectSize       = COFF_Read32 (fileObj, swap) ;
                sectDataOffset = COFF_Read32 (fileObj, swap) ;

                status = KFILE_Seek (fileObj,
                                     sectDataOffset,
                                     KFILE_SeekSet) ;

                if (DSP_SUCCEEDED (status)) {
                    status = KFILE_Read (data,
                                         sizeof (Uint8),
                                         sectSize,
                                         fileObj) ;

                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("COFF_GetSectionData", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetString
 *
 *  @desc   Gets the null terminated string from string table if required.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetString (IN  Char8 *        str,
                IN  CoffContext  * obj,
                OUT Char8 **       outStr)
{
    DSP_STATUS    status   = DSP_SOK ;
    Uint32        len      = 0       ;
    Uint32        val      = 0       ;
    Bool          swap     = FALSE   ;
    KFileObject * fileObj  = NULL    ;
    Uint32        i                  ;
    Int32         strTblOffset       ;
    Uint32        offsetSymTab       ;
    Uint32        numSymbols         ;
    Uint32        strOffset          ;

    TRC_3ENTER ("COFF_GetString", str, obj, outStr) ;

    DBC_Require (obj != NULL) ;
    DBC_Require (str != NULL) ;
    DBC_Require (outStr != NULL) ;

    if ((obj == NULL) || (str == NULL) || (outStr == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        *outStr = NULL ;

        /*  --------------------------------------------------------------------
         *  If the first four bytes of the 'string' are NULL - it indicates that
         *  string is present in the symbol table.
         *  --------------------------------------------------------------------
         */
        if (* ((Uint32 *) (&str [0])) == 0) {
            swap    = obj->isSwapped ;
            fileObj = obj->fileObj   ;
            status = COFF_GetSymTabDetails (fileObj,
                                            swap,
                                            &offsetSymTab,
                                            &numSymbols) ;

            if (DSP_SUCCEEDED (status)) {
                strTblOffset =   offsetSymTab
                               + (numSymbols * SIZE_COFF_SYMBOL_ENTRY) ;

                if (swap == TRUE) {
                    strOffset =  SWAP_LONG (*(Uint32 *) (&str [4])) ;
                }
                else {
                    strOffset =  *(Uint32 *) (&str [4]) ;
                }

                strTblOffset += strOffset;

                status = KFILE_Seek (fileObj, strTblOffset, KFILE_SeekSet) ;
                /*  ------------------------------------------------------------
                 *  Get the length of string. Read 4 bytes at a time from
                 *  the file and check if the null terminator for the string
                 *  is found.
                 *  ------------------------------------------------------------
                 */
                if (DSP_SUCCEEDED (status)) {
                    len = sizeof (Int32) ;
                    val = COFF_Read32 (fileObj, swap) ;
                    while (HasNullChar (val) == FALSE) {
                        len += sizeof (Int32) ;
                        val  = COFF_Read32 (fileObj, swap) ;
                    }
                    /*  --------------------------------------------------------
                     *  Now allocate buffer and copy string. Note that the
                     *  allocated buffer may be a little bigger than the
                     *  required amount.
                     *  Note that there is no need to check the status of Seek
                     *  operation since we have already seeked to the file
                     *  location once.
                     *  --------------------------------------------------------
                     */
                    status = MEM_Alloc ((Void **) outStr, len, MEM_DEFAULT) ;
                    if (DSP_SUCCEEDED (status)) {
                        KFILE_Seek (fileObj, strTblOffset, KFILE_SeekSet) ;
                        status = KFILE_Read (*outStr,
                                             READ_REC_SIZE,
                                             len,
                                             fileObj) ;
                        if (DSP_FAILED (status)) {
                            FREE_PTR (*outStr) ;
                            SET_FAILURE_REASON ;
                        }
                    }
                    else {
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = MEM_Alloc ((Void **) outStr,
                                COFF_NAME_LEN + 1,
                                MEM_DEFAULT) ;
            if (DSP_SUCCEEDED (status)) {
                for (i = 0 ; i < COFF_NAME_LEN ; i++) {
                    (*outStr) [i] = str [i] ;
                }
                (*outStr) [COFF_NAME_LEN] = '\0' ;
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
    }

    TRC_1LEAVE ("COFF_GetString", status) ;

    return status ;
}


/** ============================================================================
 *  @func   COFF_GetSymbolTable
 *
 *  @desc   Gets the Symbol table from the COFF file.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_GetSymbolTable (IN  CoffContext  *     obj,
                     OUT CoffSymbolEntry ** symTable,
                     OUT Uint32           * numSymbols)
{
    DSP_STATUS    status           = DSP_SOK ;
    Bool          swap             = FALSE   ;
    KFileObject * fileObj          = NULL    ;
    Uint32        offsetSymTab               ;
    Uint32        stringTableStart           ;
    Int32         i                          ;
    Int32         j                          ;

    TRC_3ENTER ("COFF_GetSymbolTable", obj, symTable, numSymbols) ;

    DBC_Require (obj != NULL) ;
    DBC_Require (symTable != NULL) ;
    DBC_Require (numSymbols != NULL) ;

    if ((obj == NULL) || (symTable == NULL) || (numSymbols == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        swap    = obj->isSwapped ;
        fileObj = obj->fileObj   ;
        status = COFF_GetSymTabDetails (fileObj,
                                        swap,
                                        &offsetSymTab,
                                        numSymbols) ;

        if (DSP_SUCCEEDED (status)) {
            status = KFILE_Seek (fileObj,
                                 offsetSymTab,
                                 KFILE_SeekSet) ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            stringTableStart = ((*numSymbols) * SIZE_COFF_SYMBOL_ENTRY)
                              + offsetSymTab ;

            i = 0 ;
            while (   (offsetSymTab < stringTableStart)
                   && DSP_SUCCEEDED (status)) {

                for (j = 0 ; j < COFF_NAME_LEN ; j++) {
                    (*symTable) [i].name [j] = COFF_Read8 (fileObj) ;
                }

                (*symTable) [i].value     = COFF_Read32 (fileObj, swap) ;
                (*symTable) [i].sectNum   = COFF_Read16 (fileObj, swap) ;
                (*symTable) [i].type      = COFF_Read16 (fileObj, swap) ;
                (*symTable) [i].storage   = COFF_Read8 (fileObj)        ;
                (*symTable) [i].numAuxEnt = COFF_Read8 (fileObj)        ;

                offsetSymTab += SIZE_COFF_SYMBOL_ENTRY ;
                if ((*symTable) [i].numAuxEnt == 1) {
                    status = KFILE_Seek (fileObj,
                                         SIZE_COFF_SYMBOL_ENTRY,
                                         KFILE_SeekCur) ;

                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                    offsetSymTab += SIZE_COFF_SYMBOL_ENTRY ;
                }
                i++ ;
            }
            if (DSP_SUCCEEDED (status)) {
                *numSymbols = i ;
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("COFF_GetSymbolTable", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_SeekToSectionHeader
 *
 *  @desc   Positions the file pointer to the start of section header.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_SeekToSectionHeader (IN KFileObject * fileObj,
                          IN Uint32        sectIndex,
                          IN Bool          swap)
{
    DSP_STATUS status        = DSP_SOK ;
    Int32      optHdrSize              ;
    Int32      sectHdrOffset           ;
    Uint32     numSections             ;

    TRC_3ENTER ("COFF_SeekToSectionHeader", fileObj, sectIndex, swap) ;

    DBC_Require (fileObj != NULL) ;

    status = COFF_GetOptHeaderSize (fileObj, swap, &optHdrSize) ;
    if (DSP_SUCCEEDED (status)) {
        status = COFF_GetNumSections (fileObj, swap, &numSections) ;

        DBC_Assert (sectIndex < numSections) ;

        if (sectIndex < numSections) {
            /* Get the offset where the section begins */
            sectHdrOffset =   SIZE_COFF_FILE_HEADER
                            + optHdrSize
                            + (sectIndex * SIZE_COFF_SECTION_HEADER) ;

            status = KFILE_Seek (fileObj, sectHdrOffset, KFILE_SeekSet) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = DSP_ERANGE ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("COFF_SeekToSectionHeader", status) ;

    return status ;

}


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetOptHeaderSize
 *
 *  @desc   Gets the size (in bytes) of optional header.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetOptHeaderSize (IN KFileObject * fileObj, IN Bool swap, OUT Int32 * size)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("COFF_GetOptHeaderSize", fileObj, swap, size) ;

    DBC_Require (fileObj != NULL) ;
    DBC_Require (size != NULL) ;

    status = KFILE_Seek (fileObj,
                         SIZE_OPT_HDR_LOC,
                         KFILE_SeekSet) ;
    if (DSP_SUCCEEDED (status)) {
        *size = COFF_Read16 (fileObj, swap) ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        TRC_1PRINT (TRC_LEVEL3, "Optional Header Size: %d\n", *size) ;
    }

    TRC_1LEAVE ("COFF_GetOptHeaderSize", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetSymTabDetails
 *
 *  @desc   Gets the information about symbol table in file.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetSymTabDetails (IN  KFileObject * fileObj,
                       IN  Bool          swap,
                       OUT Uint32 *      offsetSymTab,
                       OUT Uint32 *      numSymbols)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_4ENTER ("COFF_GetSymTabDetails",
                fileObj,
                swap,
                offsetSymTab,
                numSymbols) ;

    DBC_Require (fileObj != NULL) ;
    DBC_Require (offsetSymTab != NULL) ;
    DBC_Require (numSymbols != NULL) ;

    status = KFILE_Seek (fileObj, SYMTAB_OFFSET, KFILE_SeekSet) ;
    if (DSP_SUCCEEDED (status)) {
        *offsetSymTab = COFF_Read32 (fileObj, swap) ;
        *numSymbols   = COFF_Read32 (fileObj, swap) ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        TRC_1PRINT (TRC_LEVEL3, "Number of Symbols: %d\n", *numSymbols) ;
    }

    TRC_1LEAVE ("COFF_GetSymTabDetails", status) ;

    return status;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_GetNumSections
 *
 *  @desc   Gets the number of section in the COFF file.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_GetNumSections (IN  KFileObject * fileObj,
                     IN  Bool          swap,
                     OUT Uint32 *      numSections)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("COFF_GetNumSections", fileObj, swap, numSections) ;

    DBC_Require (fileObj != NULL) ;
    DBC_Require (numSections != NULL) ;

    status = KFILE_Seek (fileObj, NUM_SECT_OFFSET, KFILE_SeekSet) ;
    if (DSP_SUCCEEDED (status)) {
        *numSections = COFF_Read16 (fileObj, swap) ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        TRC_1PRINT (TRC_LEVEL3, "Number of Sections: %d\n", *numSections) ;
    }

    TRC_1LEAVE ("COFF_GetNumSections", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_IsValidFile
 *
 *  @desc   Checks to indicate if the file data format is valid for the given
 *          architecture.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_IsValidFile (IN  KFileObject * fileObj,
                  IN  DspArch       dspArch,
                  OUT Bool *        isValid)
{
    DSP_STATUS status  = DSP_SOK ;

    TRC_3ENTER ("COFF_IsValidFile", fileObj, dspArch, isValid) ;

    DBC_Require (fileObj != NULL) ;
    DBC_Require (isValid != NULL) ;

    *isValid = FALSE ;
    switch (dspArch) {
    case DspArch_C55x:
        status = COFF_IsValidFile_55x (fileObj, isValid) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        break ;

    case DspArch_C64x:
        status = COFF_IsValidFile_64x (fileObj, isValid) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        break ;

    default:
        TRC_0PRINT (TRC_LEVEL7, "Invalid architecture specified.\n") ;
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
        break ;
    }

    TRC_1LEAVE ("COFF_IsValidFile", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_IsSwapped
 *
 *  @desc   Check to indicate if the file data is swapped.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_IsSwapped (IN  KFileObject * fileObj,
                IN  DspArch       dspArch,
                OUT Bool *        isSwapped)
{
    DSP_STATUS status  = DSP_SOK ;

    TRC_3ENTER ("COFF_IsSwapped", fileObj, dspArch, isSwapped) ;

    DBC_Require (fileObj != NULL)   ;
    DBC_Require (isSwapped != NULL) ;

    *isSwapped = FALSE ;
    switch (dspArch) {
    case DspArch_C55x:
        status = COFF_IsSwapped_55x (fileObj, isSwapped) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        break ;

    case DspArch_C64x:
        status = COFF_IsSwapped_64x (fileObj, isSwapped) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        break ;

    default:
        TRC_0PRINT (TRC_LEVEL7, "Invalid architecture specified.\n") ;
        status = DSP_ECORRUPTFILE ;
        SET_FAILURE_REASON ;
        break ;
    }

    TRC_1LEAVE ("COFF_IsSwapped", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   COFF_FillArgsBuffer
 *
 *  @desc   Fills up the specified buffer with arguments to be sent to DSP's
 *          "main" function for the given architecture.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
COFF_FillArgsBuffer (IN  DspArch   dspArch,
                     IN  Uint32    argc,
                     IN  Char8 **  argv,
                     IN  Uint32    sectSize,
                     IN  Uint32    loadAddr,
                     IN  Uint32    wordSize,
                     IN  Endianism endian,
                     IN  Void *    argsBuf)
{
    DSP_STATUS status  = DSP_SOK ;

    TRC_6ENTER ("COFF_FillArgsBuffer", argc, argv, sectSize, loadAddr, wordSize,
                                       endian) ;

    DBC_Require (argc > 0) ;
    DBC_Require (argv != NULL) ;
    DBC_Require (argsBuf != NULL) ;

    switch (dspArch) {
    case DspArch_C55x:
        status = COFF_FillArgsBuffer_55x (argc,
                                          argv,
                                          sectSize,
                                          loadAddr,
                                          wordSize,
                                          endian,
                                          argsBuf) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        break ;

    case DspArch_C64x:
        status = COFF_FillArgsBuffer_64x (argc,
                                          argv,
                                          sectSize,
                                          loadAddr,
                                          wordSize,
                                          endian,
                                          argsBuf) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        break ;

    default:
        TRC_0PRINT (TRC_LEVEL7, "Invalid architecture specified.\n") ;
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
        break ;
    }

    TRC_1LEAVE ("COFF_FillArgsBuffer", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
