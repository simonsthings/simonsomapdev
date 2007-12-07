/** ============================================================================
 *  @file   ldrv.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines constants and interfaces to initialize and finalize
 *          sub-component LDRV.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LDRV_H)
#define LDRV_H


USES (gpptypes.h)
USES (errbase.h)
USES (cfg.h)
USES (dspdefs.h)
USES (linkdefs.h)
USES (profile.h)
USES (ldrv_mqt.h)
USES (ldrv_mqa.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   LDRV_Object
 *
 *  @desc   Structure to hold all component wide globals.
 *
 *  @field  numDsps
 *              Number of DSPs connected to the GPP.
 *  @field  numLinkTables
 *              Number of link tables specified in configuration database.
 *  @field  numMmuTables
 *              Number of MMU tables specified in configuration database.
 *  @field  dspObjects
 *              Array of DSP objects.
 *  @field  linkTables
 *              Array of pointers to link tables.
 *  @field  mmuTables
 *              Array of pointers to MMU tables.
 *  @field  numMqas
 *              Number of allocators.
 *  @field  numMqts
 *              Number of transports.
 *  @field  localMqt
 *              Local transport identifier.
 *  @field  mqaObjects
 *              Array of allocator objects.
 *  @field  mqtObjects
 *              Array of transport objects.
 *  @field  procStats
 *              Statistics object for processor subcomponent.
 *  @field  chnlStats
 *              Statistics object for channel subcomponent.
 *  @field  msgqStats
 *              Statistics object for messaging subcomponent.
 *  ============================================================================
 */
typedef struct LDRV_Object_tag {
    Uint32          numDsps       ;
    Uint32          numLinkTables ;
    Uint32          numMmuTables  ;
    DspObject *     dspObjects    ;
    LinkAttrs **    linkTables    ;
    DspMmuEntry **  mmuTables     ;

#if defined (MSGQ_COMPONENT)
    Uint32          numMqas       ;
    Uint32          numMqts       ;
    Uint32          localMqt      ;
    MqaObject *     mqaObjects    ;
    MqtObject *     mqtObjects    ;
#endif /* if defined (MSGQ_COMPONENT) */

#if defined (DDSP_PROFILE)
    ProcStats       procStats     ;
#if defined (CHNL_COMPONENT)
    ChnlStats       chnlStats     ;
#endif /* if defined (CHNL_COMPONENT) */
#if defined (MSGQ_COMPONENT)
    MsgqStats       msgqStats     ;
#endif /* if defined (MSGQ_COMPONENT) */
#endif /* if defined (DDSP_PROFILE) */
} LDRV_Object ;


/** ============================================================================
 *  @func   LDRV_Initialize
 *
 *  @desc   Initializes the LDRV sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory error.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LDRV_Finalize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_Initialize () ;


/** ============================================================================
 *  @func   LDRV_Finalize
 *
 *  @desc   Releases resources used by LDRV sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory error.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LDRV_Initialize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_Finalize () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (LDRV_H) */
