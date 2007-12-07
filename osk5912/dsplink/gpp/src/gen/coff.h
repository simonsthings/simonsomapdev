/** ============================================================================
 *  @file   coff.h
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Defines COFF loader interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (COFF_H)
#define COFF_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  SWAP_LOCATION
 *
 *  @desc   Location in COFF file where swap information is kept.
 *  ============================================================================
 */
#define     SWAP_LOCATION                     20

/** ============================================================================
 *  @const  SECT_DSECT
 *
 *  @desc   Identifier for dummy section.
 *  ============================================================================
 */
#define     SECT_DSECT                        0x0001

/** ============================================================================
 *  @const  SECT_NOLOAD
 *
 *  @desc   Identifier for a no_load section.
 *  ============================================================================
 */
#define     SECT_NOLOAD                       0x0002

/** ============================================================================
 *  @const  SECT_BSS
 *
 *  @desc   Identifier for a BSS section.
 *  ============================================================================
 */
#define     SECT_BSS                          0x0080

/** ============================================================================
 *  @const  SECT_COPY
 *
 *  @desc   Identifier for a COPY section.
 *  ============================================================================
 */
#define     SECT_COPY                         0x0010

/** ============================================================================
 *  @const  COFF_VERSION
 *
 *  @desc   Offset in file header where number of sections is present.
 *  ============================================================================
 */
#define     COFF_VERSION                       0x00C2

/** ============================================================================
 *  @const  SIZE_OPT_HDR_LOC
 *
 *  @desc   Location in file header for number of bytes in optional header.
 *  ============================================================================
 */
#define     SIZE_OPT_HDR_LOC     16

/** ============================================================================
 *  @const  COFF_NAME_LEN
 *
 *  @desc   Length of name.
 *  ============================================================================
 */
#define     COFF_NAME_LEN                      8

/** ============================================================================
 *  @const  SYMTAB_OFFSET
 *
 *  @desc   Offset in file header where symbol table details are present.
 *  ============================================================================
 */
#define     SYMTAB_OFFSET                      8

/** ============================================================================
 *  @const  NUM_SECT_OFFSET
 *
 *  @desc   Offset in file header where number of sections is present.
 *  ============================================================================
 */
#define     NUM_SECT_OFFSET                    2

/** ============================================================================
 *  @const  SIZE_COFF_FILE_HEADER/SIZE_COFF_SYMBOL_ENTRY/
 *          SIZE_COFF_SECTION_HEADER
 *
 *  @desc   Size of file header, symbolEntry and sectionHeader structure in
 *          COFF file format.
 *  ============================================================================
 */
#define     SIZE_COFF_FILE_HEADER             22
#define     SIZE_COFF_SYMBOL_ENTRY            18
#define     SIZE_COFF_SECTION_HEADER          48


/** ============================================================================
 *  @name   CoffFileHeader
 *
 *  @desc   File header for a COFF file.
 *
 *  @field  version
 *              Version ID. indicates the version of the COFF file structure.
 *  @field  numSections
 *              Number of section headers
 *  @field  dateTime
 *              Time and date stamp. indicates when the file was created.
 *  @field  fpSymTab
 *              Symbol table's starting location in file.
 *  @field  numSymTabEntries
 *              Number of entries in the symbol table.
 *  @field  numBytesOptHeader
 *              Number of bytes in the optional header. This field is
 *              either 0 or 28. If it is 0, there is no optional file header.
 *  @field  flags
 *              Flags (see the File Header Flags table).
 *  @field  targetId
 *              Target ID. magic number indicates the file can be executed
 *              in a particular system. This field is checked for validating
 *              the support of supplied file.
 *  ============================================================================
 */
typedef struct CoffFileHeader_tag {
    Uint16  version           ;
    Uint16  numSections       ;
    Int32   dateTime          ;
    Int32   fpSymTab          ;
    Int32   numSymTabEntries  ;
    Uint16  numBytesOptHeader ;
    Uint16  flags             ;
    Uint16  targetId          ;
} CoffFileHeader ;


/** ============================================================================
 *  @name   CoffContext
 *
 *  @desc   Structure defining the context of loader. This object is created
 *          on initialization of this sub component and it is required to be
 *          passed as a parameter for any subsequent function call.
 *
 *  @field  fileObj
 *              File object for the DSP base image file.
 *  @field  startAddr
 *              Entry point address for the DSP base image file.
 *  @field  isSwapped
 *              Flag to indicate if the file data is swapped.
 *  ============================================================================
 */
typedef struct CoffContext_tag {
    KFileObject * fileObj    ;
    Uint32        startAddr  ;
    Bool          isSwapped  ;
} CoffContext ;


/** ============================================================================
 *  @name   CoffOptHeader
 *
 *  @desc   Optional header for coff file format.
 *
 *  @field  magic
 *              Optional file header magic number
 *  @field  version
 *              Version stamp.
 *  @field  sizeExeCode
 *              Size (in bytes) of executable code.
 *  @field  sizeInitData
 *              Size (in bytes) of initialized data.
 *  @field  sizeUninitData
 *              Size (in bytes) of uninitialized data.
 *  @field  entry
 *              Entry point.
 *  @field  addrExe
 *              Beginning address of executable code.
 *  @field  addrInitData
 *              Beginning address of initialized data.
 *  ============================================================================
 */
typedef struct CoffOptHeader_tag {
    Int16 magic          ;
    Int16 version        ;
    Int32 sizeExeCode    ;
    Int32 sizeInitData   ;
    Int32 sizeUninitData ;
    Int32 entry          ;
    Int32 addrExe        ;
    Int32 addrInitData   ;
} CoffOptHeader ;

/** ============================================================================
 *  @name   CoffSectionHeader
 *
 *  @desc   Section header for COFF file format.
 *
 *  @field  name
 *              This field contains one of the following:
 *                  1) An 8-character section name, padded with nulls, or
 *                  2) A pointer into the string table if the section name
 *                     is longer than 8 characters.
 *              In the latter case the first four bytes of the field are 0.
 *  @field  physicalAddress
 *              Section's physical address.
 *  @field  virtualAddress
 *              Section's virtual address.
 *  @field  size
 *              Section's size in bytes.
 *  @field  fpRawData
 *              File pointer to raw data.
 *  @field  fpReloc
 *              File pointer to relocation entries.
 *  @field  fpLineNum
 *              File pointer to line-number entries.
 *  @field  numReloc
 *              Number of relocation entries.
 *  @field  numLine
 *              Number of line-number entries.
 *  @field  flags
 *              Flags (see the Section Header Flags table)
 *  @field  reserved
 *              Reserved.
 *  @field  memPageNum
 *              Memory page number.
 *  @field  isLoadSection
 *              Flag to indicate that the section is loadable.
 *  @field  data
 *              Buffer to hold data.
 *  ============================================================================
 */
typedef struct CoffSectionHeader_tag {
    Char8   name [COFF_NAME_LEN] ;
    Int32   physicalAddress      ;
    Int32   virtualAddress       ;
    Int32   size                 ;
    Int32   fpRawData            ;
    Int32   fpReloc              ;
    Int32   fpLineNum            ;
    Uint32  numReloc             ;
    Uint32  numLine              ;
    Uint32  flags                ;
    Uint16  reserved             ;
    Uint16  memPageNum           ;
    Bool    isLoadSection        ;
    Char8 * data                 ;
} CoffSectionHeader ;

/** ============================================================================
 *  @name   CoffSymbolEntry
 *
 *  @desc   Defines the structure for a symbol table entry.
 *
 *  @field  name
 *              This field contains one of the following:
 *                  1) An 8-character symbol name, padded with nulls.
 *                  2) A pointer into the string table if the symbol name is
 *                     longer than 8 characters.
 *              In the later case the first four bytes of the field are 0.
 *  @field  value
 *              Symbol value; storage class dependent.
 *  @field  sectNum
 *              Section number of the symbol.
 *  @field  type
 *              Basic and derived type specification.
 *  @field  storage
 *              Storage class of the symbol.
 *  @field  numAuxEnt
 *              Number of auxiliary entries (always 0 or 1). If this is '1'
 *              then this structure is followed by the Auxilliary entry
 *              structure (which is of the same size as this structure).
 *  ============================================================================
 */
typedef struct CoffSymbolEntry_tag {
    Char8  name [COFF_NAME_LEN] ;
    Int32  value                ;
    Int16  sectNum              ;
    Uint16 type                 ;
    Char8  storage              ;
    Char8  numAuxEnt            ;
} CoffSymbolEntry ;


/*  ============================================================================
 *  @func   COFF_Load
 *
 *  @desc   Loads the Coff format file on the DSP. This is called by
 *          PMGR_PROC_Load through the function pointer table.
 *
 *  @arg    procId
 *              Target DSP identifier where the base image is to be loaded.
 *  @arg    loaderObj
 *              This object is used to receive arguments from PMGR.
 *  @arg    argc
 *              Number of arguments to be passed to the base image upon start.
 *  @arg    argv
 *              Arguments to be passed to DSP main application.
 *  @arg    entryPt
 *              OUT argument for returning entry address for the executable.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid ProcId argument.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_ECORRUPTFILE
 *              File is not valid for this architecture.
 *          DSP_EFILE
 *              Invalid base image.
 *          DSP_EFAIL
 *              General Failure, unable to load image on DSP
 *
 *  @enter  procId must be valid.
 *          baseImage must be a valid file identifier.
 *          entryAddress must be a valid pointer.
 *          If argc is 0 then argv must be NULL pointer.
 *          If argc is non-zero then argv must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Load
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_Load (IN  ProcessorId     procId,
           IN  LoaderObject *  loaderObj,
           IN  Uint32          argc,
           IN  Char8 **        argv,
           OUT Uint32 *        entryPt) ;


/*  ============================================================================
 *  @func   COFF_LoadSection
 *
 *  @desc   Loads a section from the DSP executable onto the DSP. PMGR_PROC_Load
 *          calls this through the function pointer table.
 *
 *  @arg    procId
 *              DSP identifier.
 *  @arg    loaderObj
 *              This object is used to receive arguments from PMGR.
 *  @arg    sectId
 *              Identifier for section to load.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid ProcId argument.
 *          DSP_EFILE
 *              Invalid ImagePath parameter.
 *          DSP_ECORRUPTFILE
 *              File is not valid for this architecture.
 *          DSP_EINVALIDSECT
 *              Invalid section name.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EFAIL
 *              General Failure, unable to load image on DSP
 *
 *  @enter  procId must be valid.
 *          baseImage must be a valid file identifier.
 *          sectId must be a valid section identifier.
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Load
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
COFF_LoadSection (IN  ProcessorId     procId,
                  IN  LoaderObject *  loaderObj,
                  IN  Uint32          sectId) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (COFF_H) */
