/** ============================================================================
 *  @file   APL_WavePlay.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\application
 *
 *  @desc   Implemetation of Waveplay Application test case.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers           */
#include <stdio.h>
#include <unistd.h>
#include <linux/soundcard.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <errbase.h>
#include <dsplink.h>

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- Processor Manager             */
#include <proc.h>
#include <chnl.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Mem.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <APL_WavePlay.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ----------------------------------------------------------------------------
 *  @name   InitializeSound
 *
 *  @desc   The path of the audio codec driver on the arm side.
 *  ----------------------------------------------------------------------------
 */
#define PATH_OF_CODEC   "/dev/dsp"


/** ----------------------------------------------------------------------------
 *  @func   InitializeSound
 *
 *  @desc   The Function configers the audio codec and sets up I2C from the
 *          ARM side.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFILE
 *              Unable to configure the driver.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
InitializeSound () ;


/** ============================================================================
 *  @name   WaveDesc
 *
 *  @desc   Wave File header structure.
 *
 *  @field  RIFF
 *              R' 'I' 'F' 'F'
 *
 *  @field  dwChunkSize
 *              Size of the file - 8
 *
 *  @field  WAVE
 *              'W' 'A' 'V' 'E'
 *
 *  @field  fmt
 *              'f' 'm' 't' ' '
 *
 *  @field  dwHdrSize
 *              Struct size minus 12 = 16
 *
 *  @field  wFormat
 *              0x0001 = PCM
 *
 *  @field  wChannels
 *              1 = mono, 2 = stereo
 *
 *  @field  dwSampleRate
 *              Samples per second
 *
 *  @field  dwBytesPerSecond
 *              Sample rate * block alignment
 *
 *  @field  wBlockAlignment
 *              Channels * bits/sample / 8
 *
 *  @field  wBitsPerSample
 *              8 or 16
 *
 *  @field  Data
 *              'D' 'A' 'T' 'A'
 *
 *  @field  dwSizeofWave
 *              size of waveform data
 *  ============================================================================
 */
typedef struct WaveDesc_tag
{
  Char8  RIFF [4]         ;
  Uint32 dwChunkSize      ;
  Char8  WAVE [4]         ;
  Char8  fmt [4]          ;
  Uint32 dwHdrSize        ;
  Int16  wFormat          ;
  Int16  wChannels        ;
  Uint32 dwSampleRate     ;
  Uint32 dwBytesPerSecond ;
  Int16  wBlockAlignment  ;
  Int16  wBitsPerSample   ;
  Char8  Data [4]         ;
  Uint32 dwSizeofWave     ;
} WaveDesc ;


/** ----------------------------------------------------------------------------
 *  @func   PROCInitialize
 *
 *  @desc   Initializes the DSP.
 *
 *  @arg    argc
 *              Count of the no. of arguments passed.
 *  @arg    argv
 *              List of arguments.
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EMEMORY
 *              Memory error
 *          DSP_EFAIL
 *              General failure
 *          DSP_SALREADYATTACHED
 *              Operation Successfully completed. Also, indicates
 *              that another client had already attached to DSP.
 *          DSP_EINVALIDARG
 *              Parameter ProcId is invalid.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EFILE
 *              Invalid base image.
 *          DSP_SALREATESTARTED
 *              DSP is already in running state.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
PROCInitialize (IN Uint32 processorId, IN Char8 * fileName) ;


/** ----------------------------------------------------------------------------
 *  @func   PlayAudioData
 *
 *  @desc   This Function sends the data to the DSP side through LINK.
 *
 *  @arg    pWaveData
 *              Starting address of the data to be played.
 *  @arg    pWaveEnd
 *              Ending address of the data to be played.
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFILE
 *              Unable to play audio data.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
PlayAudioData (IN Uint32 * pWaveData, IN Uint32 * pWaveEnd) ;


/** ============================================================================
 *  @func   APL_WavePlay
 *
 *  @desc   This is Waveplay Application test case.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
APL_WavePlay (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS status        = DSP_SOK ;
    int        fd            = 0       ;
    ssize_t    numBytesRead  = 0       ;
    Char8  *   pstrInputFile = NULL    ;
    Uint32 *   pWaveData     = NULL    ;
    Uint32 *   pWaveEnd      = NULL    ;
    WaveDesc   wd                      ;


    DBC_Require (argc == 1)    ;
    DBC_Require (argv != NULL) ;

    if ((argc != 1) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of APL_WavePlay. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        pstrInputFile = argv [0] ;
        fd = open (pstrInputFile, O_RDONLY) ;
        if (fd == 0) {
            status = DSP_EFAIL ;
        }

    }

    if (DSP_SUCCEEDED (status)) {
        numBytesRead = read (fd, (void *) &wd, sizeof(wd)) ;
        if (numBytesRead < 0) {
            status = DSP_EFAIL ;
        }

    }

    if (DSP_SUCCEEDED (status)) {
        if (   (wd.RIFF [0] != 'R')
            || (wd.RIFF [1] != 'I')
            || (wd.RIFF [2] != 'F')
            || (wd.RIFF [3] != 'F')
            || (wd.wChannels != 2 )
            || (wd.wBitsPerSample != 16)) {
            status = DSP_EFAIL ;
        }
    }

    if ((DSP_SUCCEEDED (status)) && (wd.dwSizeofWave > 0)) {
        status = TST_Alloc ((Void**)&pWaveData, wd.dwSizeofWave) ;
    }

    if (DSP_SUCCEEDED (status)) {
        numBytesRead = read (fd, (void *) pWaveData, wd.dwSizeofWave) ;
        if (numBytesRead < 0) {
            status = DSP_EFAIL ;
        }
    }

    if (fd != 0) {
        close (fd) ;
    }

    if (DSP_SUCCEEDED (status)) {
        pWaveEnd = (Uint32*) ((Char8*) pWaveData + wd.dwSizeofWave) ;
        status = PlayAudioData (pWaveData, pWaveEnd) ;
    }

    if (pWaveData != NULL) {
        TST_Free ((Void*)&pWaveData) ;
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   PlayAudioData
 *
 *  @desc   This Function sends the data to the DSP side through LINK.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
PlayAudioData (IN Uint32 * pWaveData, IN Uint32 * pWaveEnd)
{
    DSP_STATUS    status            = DSP_SOK ;
    Bool          fDone             = FALSE   ;
    Char8 *       pCurrent          = NULL    ;
    Char8 *       pDst              = NULL    ;
    Uint32        i                 = 0       ;
    Uint32        recalimToBeDone   = 0       ;
    ChannelAttrs  chnlAttr                    ;
    ChannelIOInfo ioReq                       ;
    Char8       * bufArray [BUFNUM]           ;
    Uint32        j                           ;

    chnlAttr.mode      = MODE      ;
    chnlAttr.endianism = ENDIANISM ;
    chnlAttr.size      = CHNL_SIZE ;
    ioReq.size         = BUFSIZE   ;

    status = InitializeSound () ;

    if (DSP_SUCCEEDED (status)) {
        status = PROCInitialize (PROCESSOR_ID ,DSP_IMAGE) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = CHNL_Create (PROCESSOR_ID, CHANNEL_ID, &chnlAttr) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = CHNL_AllocateBuffer (PROCESSOR_ID,
                                      CHANNEL_ID,
                                      bufArray,
                                      BUFSIZE,
                                      BUFNUM) ;
        pCurrent = (Char8 *) pWaveData ;
    }

    for ( ; (!fDone) && (DSP_SUCCEEDED (status)) ; ) {
        if (((Char8 *)pCurrent + BUFSIZE) >= (Char8 *) pWaveEnd) {
           fDone = TRUE ;               /* Break on end-of-file */
        }
        pDst = bufArray [0] ;
        i++ ;

        for (j = 0 ; (j < BUFSIZE) && (pCurrent < (Char8 *) pWaveEnd) ; j++) {
            *pDst++  = *pCurrent++ ;
        }

        ioReq.buffer = bufArray [0] ;

        if (i == BUFNUM) {
            i = 0 ;
        }

        ioReq.size   = j    ;
        status = CHNL_Issue (PROCESSOR_ID, CHANNEL_ID, &ioReq) ;
        recalimToBeDone++ ;

        if (recalimToBeDone == BUFNUM) {
            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim (PROCESSOR_ID,
                                       CHANNEL_ID,
                                       TIMEOUT,
                                       &ioReq) ;
            }
            recalimToBeDone-- ;
        }
    }
    while ((DSP_SUCCEEDED (status)) && (recalimToBeDone != 0)) {
        status = CHNL_Reclaim (PROCESSOR_ID,
                               CHANNEL_ID,
                               TIMEOUT,
                               &ioReq) ;
        recalimToBeDone-- ;
    }
    CHNL_FreeBuffer (PROCESSOR_ID, CHANNEL_ID, bufArray, BUFNUM) ;
    CHNL_Delete (PROCESSOR_ID, CHANNEL_ID) ;
    PROC_Detach (PROCESSOR_ID) ;
    PROC_Destroy () ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   PROCInitialize
 *
 *  @desc   This function initializes the processor.
 *
 *  @modif  chnlResult
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
PROCInitialize (IN Uint32 processorId, IN Char8 * fileName)
{
    DSP_STATUS status = DSP_SOK ;

    status = PROC_Setup () ;

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (processorId, NULL) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Load (processorId, fileName, 0, NULL) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Start (processorId) ;
    }
    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   InitializeSound
 *
 *  @desc   The Function configers the audio codec and sets up I2C from the
 *          ARM side.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
InitializeSound ()
{
    DSP_STATUS status    = DSP_SOK ;
    int        sampLeval = 8000    ;
    int        fd                  ;
    int        osStatus            ;

    fd = open (PATH_OF_CODEC, O_RDONLY, 0);

    if(fd == -1) {
        status = DSP_EFAIL ;
        TST_PrnError ("Failed to open audio driver. Status = [0x%x]", status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        osStatus = ioctl(fd, SNDCTL_DSP_SPEED, &sampLeval);
        if (osStatus < 0) {
            status = DSP_EFAIL ;
            TST_PrnError ("Failed to set sampling rate. Status = [0x%x]",
                          status) ;
        }
        else {
            sampLeval = 100 ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        osStatus = ioctl(fd, MIXER_WRITE(SOUND_MIXER_VOLUME), &sampLeval);
        if (osStatus < 0) {
            status = DSP_EFAIL ;
            TST_PrnError ("Failed to set volume. Status = [0x%x]", status) ;
        }
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
