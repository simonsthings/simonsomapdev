/** ============================================================================
 *  @file   dspdefs.h
 *
 *  @path   $(DSPLINK)\gpp\inc
 *
 *  @desc   Defines the DSP object structure and associated structures.
 *          Some structures, already defined in CFG, have been redefined here
 *          to make the DSP subcomponent independent of CFG.
 *
 *  @ver    00.13
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (DSPDEFS_H)
#define DSPDEFS_H


USES (gpptypes.h)
USES (dsplink.h)
USES (intobject.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/*  ============================================================================
 *  @const  ADDRMAP_INVALID
 *
 *  @desc   Indicates DSP address mapping to GPP space is invalid.
 *  ============================================================================
 */
#define ADDRMAP_INVALID  0x0


/*  ============================================================================
 *  @name   DspObject
 *
 *  @desc   Forward declaration of DSP Object.
 *  ============================================================================
 */
typedef struct DspObject_tag DspObject ;

/*  ============================================================================
 *  @name   LinkAttrs
 *
 *  @desc   Forward declaration of Link Attributes structure.
 *  ============================================================================
 */
typedef struct LinkAttrs_tag LinkAttrs ;

#if defined (DDSP_PROFILE)
/*  ============================================================================
 *  @name   DspStats
 *
 *  @desc   Forward declaration of DSP Statistics Object.
 *  ============================================================================
 */
typedef struct DspStats_tag DspStats ;
#endif  /* defined (DDSP_PROFILE) */


/** ============================================================================
 *  @name   DspMmuEntry
 *
 *  @desc   MMU Entry for the DSP.
 *
 *  @field  entry
 *              Entry number for the MMU record.
 *  @field  virtualAddress
 *              Virtual address.
 *  @field  physicalAddress
 *              Physical address.
 *  @field  size
 *              Indicates the size of MMU TLB entry.
 *  @field  access
 *              Access permission.
 *  @field  preserve
 *              Indicates whether entry is preserved or not.
 *  @field  mapInGpp
 *              Flag indicating whether DSP address is mapped to GPP
 *              address space.
 *  ============================================================================
 */
typedef struct DspMmuEntry_tag {
    Uint32  entry           ;
    Uint32  virtualAddress  ;
    Uint32  physicalAddress ;
    Uint32  size            ;
    Uint32  access          ;
    Uint32  preserve        ;
    Uint32  mapInGpp        ;
} DspMmuEntry ;


/** ============================================================================
 *  @name   FnDspSetup
 *
 *  @desc   Signature of function that sets up components to make DSP
 *          reachable from GPP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspSetup) (IN ProcessorId dspId,
                                  IN DspObject * dspObj) ;

/** ============================================================================
 *  @name   FnDspInitialize
 *
 *  @desc   Signature of function that resets the DSP and initializes the
 *          components required by DSP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *          DSP_EPOINTER
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspInitialize) (IN ProcessorId dspId,
                                       IN DspObject * dspObj) ;

/** ============================================================================
 *  @name   FnDspFinalize
 *
 *  @desc   Signature of function that resets the DSP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *          DSP_EPOINTER
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspFinalize) (IN ProcessorId dspId,
                                     IN DspObject * dspObj) ;

/** ============================================================================
 *  @name   FnDspStart
 *
 *  @desc   Signature of function that causes DSP to start execution from
 *          the given DSP address.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    dspAddr
 *              Address to start execution from.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *          DSP_EPOINTER
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspStart) (IN ProcessorId dspId,
                                  IN DspObject * dspObj,
                                  IN Uint32      dspAddr) ;

/** ============================================================================
 *  @name   FnDspStop
 *
 *  @desc   Signature of function that stops execution on DSP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *          DSP_EPOINTER
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspStop) (IN ProcessorId dspId,
                                 IN DspObject * dspObj) ;

/** ============================================================================
 *  @name   FnDspIdle
 *
 *  @desc   Signature of function that idles the DSP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *          DSP_EPOINTER
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspIdle) (IN ProcessorId dspId,
                                 IN DspObject * dspObj) ;

/** ============================================================================
 *  @name   FnDspEnableInterrupt
 *
 *  @desc   Signature of function that enables the specified interrupt
 *          for communication with DSP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    intInfo
 *              Pointer to object containing interrupt information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspEnableInterrupt) (IN ProcessorId       dspId,
                                            IN DspObject *       dspObj,
                                            IN InterruptObject * intInfo) ;


/** ============================================================================
 *  @name   FnDspDisableInterrupt
 *
 *  @desc   Signature of function that disables the specified interrupt for
 *          communication with DSP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    intInfo
 *              Pointer to object containing interrupt information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspDisableInterrupt) (IN ProcessorId       dspId,
                                             IN DspObject *       dspObj,
                                             IN InterruptObject * intInfo) ;


/** ============================================================================
 *  @name   FnDspInterrupt
 *
 *  @desc   Signature of function that sends the specified interrupt to DSP.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    intInfo
 *              Pointer to object containing interrupt information.
 *  @arg    arg
 *              Value to send with the interrupt.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid
 *          DSP_EPOINTER
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspInterrupt) (IN ProcessorId       dspId,
                                      IN DspObject *       dspObj,
                                      IN InterruptObject * intInfo,
                                      IN Pvoid             arg) ;

/** ============================================================================
 *  @name   FnDspClearInterrupt
 *
 *  @desc   Clear the DSP Interrupt.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    intInfo
 *              Pointer to object containing interrupt information.
 *  @arg    retVal
 *              Interrupt value present before clearing the interrupt.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *          DSP_EPOINTER
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspClearInterrupt) (IN  ProcessorId       dspId,
                                           IN  DspObject *       dspObj,
                                           IN  InterruptObject * intInfo,
                                           OUT Pvoid             retVal) ;

/** ============================================================================
 *  @name   FnDspRead
 *
 *  @desc   Signature of function that reads data from DSP.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    dspAddr
 *              DSP address to read from.
 *  @arg    endianism
 *              endianness of data - indicates whether swap is required or not.
 *  @arg    numBytes
 *              Number of bytes to read.
 *  @arg    buffer
 *              Buffer to hold the read data.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *          DSP_EPOINTER
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspRead) (IN     ProcessorId  dspId,
                                 IN     DspObject *  dspObj,
                                 IN     Uint32       dspAddr,
                                 IN     Endianism    endianism,
                                 IN OUT Uint32 *     numBytes,
                                 OUT    Uint8 *      buffer) ;

/** ============================================================================
 *  @name   FnDspWrite
 *
 *  @desc   Signature of function that writes data to DSP.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    dspAddr
 *              DSP address to write to.
 *  @arg    endianism
 *              endianness of data - indicates whether swap is required or not.
 *  @arg    numBytes
 *              Number of bytes to write.
 *  @arg    buffer
 *              Buffer containing data to be written.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid.
 *          DSP_EPOINTER
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspWrite) (IN ProcessorId dspId,
                                  IN DspObject * dspObj,
                                  IN Uint32      dspAddr,
                                  IN Endianism   endianism,
                                  IN Uint32      numBytes,
                                  IN Uint8 *     buffer) ;

/** ============================================================================
 *  @name   FnDspControl
 *
 *  @desc   Hook for performing device dependent control operation.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    cmd
 *              Command id.
 *  @arg    arg
 *              Optional argument for the specified command.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid arguments specified.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspControl) (IN  ProcessorId dspId,
                                    IN  DspObject * dspObj,
                                    IN  Int32       cmd,
                                    OPT Pvoid       arg) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @name   FnDspInstrument
 *
 *  @desc   Gets the instrumentation information related to the specified
 *          DSP object.
 *
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    retVal
 *              Placeholder to return the instrumentation information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument(s).
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspInstrument) (IN DspObject * dspObj,
                                       OUT DspStats * retVal) ;
#endif /* if defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @name   FnDspDebug
 *
 *  @desc   Prints debug information of the specified DSP object.
 *
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *  ============================================================================
 */
typedef Void (*FnDspDebug) (IN DspObject * dspObj) ;
#endif /* if defined (DDSP_DEBUG) */


/** ============================================================================
 *  @name   DspInterface
 *
 *  @desc   Interface functions exported by the DSP subcomponent.
 *
 *  @field  setup
 *              Function pointer to setup function for the DSP.
 *  @field  initialize
 *              Function pointer to initialize function for the DSP.
 *  @field  finalize
 *              Function pointer to finalize function for the DSP.
 *  @field  start
 *              Function pointer to start function for the DSP.
 *  @field  stop
 *              Function pointer to stop function for the DSP.
 *  @field  idle
 *              Function pointer to idle function for the DSP.
 *  @field  enableInterrupt
 *              Function pointer to enableInterrupt function for the DSP.
 *  @field  disableInterrupt
 *              Function pointer to disableInterrupt function for the DSP.
 *  @field  interrupt
 *              Function pointer to interrupt function for the DSP.
 *  @field  clearInterrupt
 *              Function pointer to clearInterrupt function for the DSP.
 *  @field  read
 *              Function pointer to read function for the DSP.
 *  @field  write
 *              Function pointer to write function for the DSP.
 *  @field  instrument
 *              Function pointer to instrument function for the DSP.
 *  @field  debug
 *              Function pointer to debug function for the DSP.
 *  ============================================================================
 */
typedef struct DspInterface_tag {
    FnDspSetup              setup            ;
    FnDspInitialize         initialize       ;
    FnDspFinalize           finalize         ;
    FnDspStart              start            ;
    FnDspStop               stop             ;
    FnDspIdle               idle             ;
    FnDspEnableInterrupt    enableInterrupt  ;
    FnDspDisableInterrupt   disableInterrupt ;
    FnDspInterrupt          interrupt        ;
    FnDspClearInterrupt     clearInterrupt   ;
    FnDspRead               read             ;
    FnDspWrite              write            ;
    FnDspControl            control          ;
#if defined (DDSP_PROFILE)
    FnDspInstrument         instrument       ;
#endif /* if defined (DDSP_PROFILE) */
#if defined (DDSP_DEBUG)
    FnDspDebug              debug            ;
#endif /* if defined (DDSP_DEBUG) */
} DspInterface ;


/** ============================================================================
 *  @name   FnLoad
 *
 *  @desc   The Function pointer providing the abstraction to the loader's
 *          load function.
 *
 *  @arg    dspId
 *              Target DSP identifier where the base image is to be loaded.
 *  @arg    loaderObj
 *              This object is used to receive arguments from PMGR.
 *  @arg    argc
 *              Number of arguments to be passed to the base image upon start.
 *  @arg    argv
 *              Arguments to be passed to DSP main application.
 *  @arg    entryPt
 *              Argument for returning entry address for the executable.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFILE
 *              Invalid base image
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP
 *          DSP_EFAIL
 *              General failure, unable to load image onto DSP
 *          DSP_EINVALIDARG
 *              Invalid dspId argument.
 *
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLoad) (IN  ProcessorId     dspId,
                              IN  LoaderObject *  loaderObj,
                              IN  Uint32          argc,
                              IN  Char8 **        argv,
                              OUT Uint32 *        entryPt) ;

/** ============================================================================
 *  @name   FnLoadSection
 *
 *  @desc   The Function pointer providing the abstraction to the loader's
 *          loadSection function.
 *
 *  @arg    dspId
 *              Target DSP identifier where the section is to be loaded.
 *  @arg    loaderObj
 *              This object is used to receive arguments from PMGR.
 *  @arg    sectId
 *              Identifier for section to load.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFILE
 *              Invalid base image
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP
 *          DSP_EFAIL
 *              General failure, unable to load image onto DSP
 *          DSP_EINVALIDARG
 *              Invalid dspId argument.
 *          DSP_EINVALIDSECT
 *              Invalid section name.
 *
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLoadSection) (IN  ProcessorId     dspId,
                                     IN  LoaderObject *  loaderObj,
                                     IN  Uint32          sectId) ;


/** ============================================================================
 *  @name   LoaderInterface
 *
 *  @desc   Structure containing interface functions exported by the loader
 *          subcomponent.
 *
 *  @field  load
 *              Function pointer providing the abstraction to the loader's
 *              load module.
 *  @field  loadSection
 *              Function pointer providing the abstraction to the loader's
 *              loadSection module.
 *  ============================================================================
 */
typedef struct LoaderInterface_tag {
    FnLoad         load         ;
    FnLoadSection  loadSection  ;
} LoaderInterface ;


/** ============================================================================
 *  @name   DspObject
 *
 *  @desc   DSP object.
 *
 *  @field  dspName
 *              Name of the DSP.
 *  @field  execName
 *              Name of default DSP executable.
 *  @field  loaderName
 *              Name of the loader used.
 *  @field  dspArch
 *              Architecture of the Dsp.
 *  @field  loaderInterface
 *              The function pointer interface to access the services of
 *              the loader subcomponent for this DSP.
 *  @field  linkTable
 *              Array of link attributes.
 *  @field  numLinks
 *              Number of links towards the DSP.
 *  @field  autoStart
 *              Auto start flag for the DSP.
 *  @field  resetVector
 *              Reset vector address for the dsp.
 *  @field  wordSize
 *              Word size of the DSP.
 *  @field  endian
 *              Endianism of the DSP.
 *  @field  mmuFlag
 *              Indicates if the MMU is enabled on the DSP.
 *  @field  mmuTable
 *              Table of MMU entries.
 *  @field  numMmuEntries
 *              Number of MMU entries.
 *  @field  interface
 *              The function pointer interface to access the services of
 *              the DSP subcomponent for this DSP.
 *  @field  addrMapInGpp
 *              Array holding GPP address corresponding to DSP address. Size of
 *              the array is 'numMmuEntries' to hold entries corresponding
 *              to the number of entries in DSP MMU table.
 *              The value at an index will be ADDRMAP_INVALID in case no mapping
 *              is required corresponding to a DSP MMU entry.
 *  @field  mqtId
 *              The id of the MQT which is to be used for this DSP.
 *  @field  dspStats
 *              Profiling information related to the target DSP.
 *  ============================================================================
 */
struct DspObject_tag {
    Char8              dspName    [DSP_MAX_STRLEN] ;
    DspArch            dspArch                     ;
    Char8              execName   [DSP_MAX_STRLEN] ;
    LoaderInterface  * loaderInterface             ;
#if defined (CHNL_COMPONENT)
    LinkAttrs *        linkTable                   ;
    Uint32             numLinks                    ;
#endif /* if defined (CHNL_COMPONENT) */
    Uint32             autoStart                   ;
    Uint32             resetVector                 ;
    Uint32             wordSize                    ;
    Uint32             endian                      ;
    Bool               mmuFlag                     ;
    DspMmuEntry *      mmuTable                    ;
    Uint32             numMmuEntries               ;
    DspInterface *     interface                   ;
    Uint32 *           addrMapInGpp                ;
#if defined (MSGQ_COMPONENT)
    Uint32             mqtId                       ;
#endif /* if defined (MSGQ_COMPONENT) */
#if defined (DDSP_PROFILE)
    DspStats *         dspStats                    ;
#endif
} ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (DSPDEFS_H) */
