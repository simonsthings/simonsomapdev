/** ============================================================================
 *  @file   loaderdefs.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Defines data types and structures used by the DSP/BIOS(tm) Link
 *          loader.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LOADERDEFS_H)
#define LOADERDEFS_H


USES (gpptypes.h)

#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @name   FnWriteDspMem
 *
 *  @desc   Signature of function that writes to DSP memory.
 *
 *  @arg    dspId
 *              DSP ID of DSP whoose memory is to be written.
 *  @arg    dspAddr
 *              Address to which data needs to be written.
 *  @arg    endianInfo
 *              This specifies endianness of the data.
 *  @arg    numBytes
 *              Number of bytes to be written.
 *  @arg    buffer
 *              Buffer in which to store the data to write.
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EWRONGSTATE
 *              DSP not in the right state to execute this function.
 *
 *  ============================================================================
 */
typedef DSP_STATUS (*FnWriteDspMem) (IN  ProcessorId  dspId,
                                     IN  Uint32       dspAddr,
                                     IN  Endianism    endianInfo,
                                     IN  Uint32       numBytes,
                                     IN  Uint8 *      buffer) ;


/** ============================================================================
 *  @name   LoaderObject
 *
 *  @desc   This object is used to pass arguments to Loader component.
 *
 *  @field  baseImage
 *              DSP executable file name.
 *  @field  dspArch
 *              Architecture of the dsp.
 *  @field  endian
 *              Endianism of the processor.
 *  @field  maduSize
 *              Madu size on the dsp.
 *  @field  fnWriteDspMem
 *              Function pointer to write function for the DSP.
 *
 *  @see    None
 *  ============================================================================
 */
typedef struct LoaderObject_tag {
    Pstr            baseImage     ;
    DspArch         dspArch       ;
    Endianism       endian        ;
    Uint32          maduSize      ;
    FnWriteDspMem   fnWriteDspMem ;
} LoaderObject ;


/** ============================================================================
 *  @name   BinLoaderImageInfo
 *
 *  @desc   Structure defining information about the image to be loaded. A
 *          pointer to this structure is passed during the PROC_Load () function
 *          as the imagePath, when the binary loader is used.
 *
 *  @field  gppAddr
 *              Address of the file to be loaded, in GPP address space.
 *  @field  dspLoadAddr
 *              DSP address where the binary file is to be loaded.
 *  @field  dspRunAddr
 *              DSP address from where the binary file execution is to be
 *              started.
 *  @field  fileSize
 *              Size of the file to be loaded in bytes.
 *
 *  @see    PROC_Load ()
 *  ============================================================================
 */
typedef struct BinLoaderImageInfo_tag {
    Uint32  gppAddr     ;
    Uint32  dspLoadAddr ;
    Uint32  dspRunAddr  ;
    Uint32  fileSize    ;
} BinLoaderImageInfo ;


#if defined (__cplusplus)
}
#endif

#endif /* !define (LOADERDEFS_H) */
