/** ============================================================================
 *  @file   dspdefs.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Defines the DSP object structure and associated structures.
 *          Some structures, already defined in CFG, have been redefined here
 *          to make the DSP subcomponent independent of CFG.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
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
USES (loaderdefs.h)
USES (datadefs.h)
USES (ldrv_mqt.h)


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
 *  @name   LinkObject
 *
 *  @desc   Forward declaration of Link object structure.
 *  ============================================================================
 */
typedef struct LinkObject_tag LinkObject ;

/*  ============================================================================
 *  @name   DataObject
 *
 *  @desc   Forward declaration of Data object structure.
 *  ============================================================================
 */
typedef struct DataObject_tag DataObject ;

/*  ============================================================================
 *  @name   IpsObject
 *
 *  @desc   Forward declaration of IPS object structure.
 *  ============================================================================
 */
typedef struct IpsObject_tag IpsObject ;

/** ============================================================================
 *  @name   IsrObject_tag
 *
 *  @desc   Forward declaration of IsrObject_tag structure.
 *  ============================================================================
 */
struct IsrObject_tag ;


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
 *  @name   DspAddrConvType
 *
 *  @desc   Defines the types of address conversion performed by the
 *          DSP_AddrConvert () API.
 *
 *  @field  GppToDsp
 *              GPP to DSP address conversion.
 *  @field  DspToGpp
 *              DSP to GPP address conversion.
 *  ============================================================================
 */
typedef enum {
    GppToDsp  = 0,
    DspToGpp  = 1
} DspAddrConvType ;

/** ============================================================================
 *  @name   DspIntCtrlCmd
 *
 *  @desc   Defines the types of interrupt control commands handled by the DSP
 *          component.
 *
 *  @field  DspIntCtrlCmd_Enable
 *              Enable interrupt
 *  @field  DspIntCtrlCmd_Disable
 *              Disable interrupt
 *  @field  DspIntCtrlCmd_Send
 *              Send interrupt
 *  @field  DspIntCtrlCmd_Clear
 *              Clear interrupt
 *  @field  DspIntCtrlCmd_WaitClear
 *              Wait for interrupt to be cleared
 *  ============================================================================
 */
typedef enum {
    DspIntCtrlCmd_Enable    = 0,
    DspIntCtrlCmd_Disable   = 1,
    DspIntCtrlCmd_Send      = 2,
    DspIntCtrlCmd_Clear     = 3,
    DspIntCtrlCmd_WaitClear = 4
} DspIntCtrlCmd ;


/** ============================================================================
 *  @name   LinkMemInfo
 *
 *  @desc   Defines the Link Driver Memory information object
 *
 *  @field  entry
 *              Entry number for the MEM record.
 *  @field  virtualAddress
 *              Virtual address.
 *  @field  physicalAddress
 *              Physical address.
 *  @field  size
 *              Indicates the size of MEM TLB entry.
 *  @field  mapInGpp
 *              Flag indicating whether DSP address is mapped to GPP
 *              address space.
 *  ============================================================================
 */
typedef struct LinkMemInfo_tag {
    Uint32  entry       ;
    Uint32  physAddr    ;
    Uint32  dspVirtAddr ;
    Uint32  gppVirtAddr ;
    Uint32  size        ;
    Uint32  mapInGpp    ;
} LinkMemInfo ;


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
 *  @name   FnDspIntCtrl
 *
 *  @desc   Signature of function that perform the specified DSP interrupt
 *          control activity.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    isrObj
 *              Pointer to ISR object.
 *  @arg    cmd
 *              Interrupt control command to be performed.
 *  @arg    arg
 *              Optional input/output argument specific to each control command.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid.
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnDspIntCtrl) (IN         ProcessorId       dspId,
                                    IN         DspObject *       dspObj,
                                    IN  struct IsrObject_tag *   isrObj,
                                    IN         DspIntCtrlCmd     cmd,
                                    IN OUT OPT Pvoid             arg) ;

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
typedef DSP_STATUS (*FnDspRead) (IN  ProcessorId  dspId,
                                 IN  DspObject *  dspObj,
                                 IN  Uint32       dspAddr,
                                 IN  Endianism    endianism,
                                 IN  Uint32       numBytes,
                                 OUT Uint8 *      buffer) ;

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
 *  @name   FnDspAddrConvert
 *
 *  @desc   Signature of function that converts address between GPP and DSP
 *          address space.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    addr
 *              Address to be converted. If DSP address, the addr parameter
 *              reflects the DSP MADU address.
 *  @arg    type
 *              Type of address conversion.
 *
 *  @ret    Converted address.
 *              Operation successfully completed.
 *          ADDRMAP_INVALID.
 *              Specified address is not in mapped range.
 *  ============================================================================
 */
typedef Uint32 (*FnDspAddrConvert) (IN  ProcessorId        dspId,
                                    IN  DspObject *        dspObj,
                                    IN  Uint32             addr,
                                    IN  DspAddrConvType    type) ;

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
 *  @field  intCtrl
 *              Function pointer to interrupt control function for the DSP.
 *  @field  read
 *              Function pointer to read function for the DSP.
 *  @field  write
 *              Function pointer to write function for the DSP.
 *  @field  addrConvert
 *              Function pointer to address conversion function for the DSP.
 *  @field  control
 *              Function pointer to device dependent control functionionality
 *              for the DSP.
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
    FnDspIntCtrl            intCtrl          ;
    FnDspRead               read             ;
    FnDspWrite              write            ;
    FnDspAddrConvert        addrConvert      ;
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
 *              Defined only for debug build.
 *  @field  dspArch
 *              Architecture of the Dsp.
 *  @field  interface
 *              The function pointer interface to access the services of
 *              the DSP subcomponent for this DSP.
 *  @field  loaderInterface
 *              The function pointer interface to access the services of
 *              the loader subcomponent for this DSP.
 *  @field  autoStart
 *              Auto start flag for the DSP.
 *  @field  execName
 *              Name of default DSP executable.
 *  @field  resetVector
 *              Reset vector address for the dsp.
 *  @field  resetCodeSize
 *              Size of code at DSP Reset Vector.
 *  @field  maduSize
 *              MADU size of the DSP.
 *  @field  endian
 *              Endianism of the DSP.
 *  @field  numMemEntries
 *              Number of MEM entries.
 *  @field  memTable
 *              Table of MEM entries.
 *  @field  wordSwap
 *              Indicates whether word-swap is enabled for the DSP MEM.
 *  @field  linkObject
 *              Pointer to link object for the DSP.
 *              Defined only when either CHNL or MSGQ are enabled.
 *  @field  dataObjects
 *              Array of data driver objects supported for the DSP.
 *              Defined only when CHNL is enabled.
 *  @field  numDataDrivers
 *              Number of data drivers supported for the DSP.
 *              Defined only when CHNL is enabled.
 *  @field  mqtId
 *              ID of the MQT used by the DSP.
 *              Defined only when MSGQ is enabled.
 *  @field  dspStats
 *              Profiling information related to the target DSP.
 *              Defined only when profiling is enabled.
 *  @field  arg1
 *              Optional platform specific argument
 *  @field  arg2
 *              Optional platform specific argument
 *  @field  arg3
 *              Optional platform specific argument
 *  @field  arg4
 *              Optional platform specific argument
 *  ============================================================================
 */
struct DspObject_tag {
#if defined (DDSP_DEBUG)
    Char8              dspName    [DSP_MAX_STRLEN] ;
#endif /* if defined (DDSP_DEBUG) */
    DspArch            dspArch                     ;
    DspInterface *     interface                   ;
    LoaderInterface  * loaderInterface             ;
    Bool               autoStart                   ;
    Char8              execName   [DSP_MAX_STRLEN] ;
    Uint32             resetVector                 ;
    Uint32             resetCodeSize               ;
    Uint32             maduSize                    ;
    Uint32             endian                      ;
    Uint32             numMemEntries               ;
    LinkMemInfo *      memTable                    ;
    Bool               wordSwap                    ;
#if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT)
    LinkObject *       linkObject                  ;
#endif /* if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT) */
#if defined (CHNL_COMPONENT)
    Uint32             numDataDrivers              ;
    DataObject *       dataObjects                 ;
#endif /* if defined (CHNL_COMPONENT) */
#if defined (MSGQ_COMPONENT)
    Uint32             mqtId                       ;
#endif /* if defined (MSGQ_COMPONENT) */
#if defined (DDSP_PROFILE)
    DspStats *         dspStats                    ;
#endif /* if defined (DDSP_PROFILE) */
    Uint32             arg1                        ;
    Uint32             arg2                        ;
    Uint32             arg3                        ;
    Uint32             arg4                        ;
} ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (DSPDEFS_H) */
