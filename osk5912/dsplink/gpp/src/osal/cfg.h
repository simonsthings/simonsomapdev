/** ============================================================================
 *  @file   cfg.h
 *
 *  @path   $(DSPLINK)\gpp\src\osal
 *
 *  @desc   Defines the interface and data structures for the CFG
 *          subcomponent.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (CFG_H)
#define CFG_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


USES (gpptypes.h)
USES (errbase.h)
USES (cfgdefs.h)


/** ============================================================================
 *  @const  CFG_DRIVER_BASE
 *
 *  @desc   Base of the keys to fetch driver related information from the
 *          configuration database.
 *  ============================================================================
 */
#define CFG_DRIVER_BASE         (Uint32) 0x0000

#define CFG_DRIVER_OBJECT       (Uint32) (CFG_DRIVER_BASE)
#define CFG_DRIVER_NAME         (Uint32) (CFG_DRIVER_BASE + 1)
#define CFG_DRIVER_COMPONENTS   (Uint32) (CFG_DRIVER_BASE + 2)
#define CFG_DRIVER_QUEUE        (Uint32) (CFG_DRIVER_BASE + 3)
#define CFG_DRIVER_LINKTABLES   (Uint32) (CFG_DRIVER_BASE + 4)
#define CFG_DRIVER_MMUTABLES    (Uint32) (CFG_DRIVER_BASE + 5)
#if defined (MSGQ_COMPONENT)
#define CFG_DRIVER_NUMMQAS      (Uint32) (CFG_DRIVER_BASE + 6)
#define CFG_DRIVER_NUMMQTS      (Uint32) (CFG_DRIVER_BASE + 7)
#define CFG_DRIVER_LOCALMQAID   (Uint32) (CFG_DRIVER_BASE + 8)
#endif /* if defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @const  CFG_GPP_BASE
 *
 *  @desc   Base of the keys to fetch GPP related information from the
 *          configuration database.
 *  ============================================================================
 */
#define CFG_GPP_BASE            (Uint32) 0x1000

#define CFG_GPP_OBJECT          (Uint32) (CFG_GPP_BASE)
#define CFG_GPP_NAME            (Uint32) (CFG_GPP_BASE + 1)
#define CFG_GPP_NUMDSPS         (Uint32) (CFG_GPP_BASE + 2)


/** ============================================================================
 *  @const  CFG_DSP_BASE
 *
 *  @desc   Base of the keys to fetch DSP related information from the
 *          configuration database.
 *  ============================================================================
 */
#define CFG_DSP_BASE            (Uint32) 0x2000

#define CFG_DSP_OBJECT          (Uint32) (CFG_DSP_BASE)
#define CFG_DSP_NAME            (Uint32) (CFG_DSP_BASE + 1)
#define CFG_DSP_EXECUTABLE      (Uint32) (CFG_DSP_BASE + 2)
#define CFG_DSP_LOADER          (Uint32) (CFG_DSP_BASE + 3)
#define CFG_DSP_LINKTABLE       (Uint32) (CFG_DSP_BASE + 4)
#define CFG_DSP_LINKTABLESIZE   (Uint32) (CFG_DSP_BASE + 5)
#define CFG_DSP_AUTOSTART       (Uint32) (CFG_DSP_BASE + 6)
#define CFG_DSP_WORDSIZE        (Uint32) (CFG_DSP_BASE + 7)
#define CFG_DSP_ENDIANISM       (Uint32) (CFG_DSP_BASE + 8)
#define CFG_DSP_MMUFLAG         (Uint32) (CFG_DSP_BASE + 9)
#define CFG_DSP_MMUTABLE        (Uint32) (CFG_DSP_BASE + 10)
#define CFG_DSP_MMUTABLESIZE    (Uint32) (CFG_DSP_BASE + 11)
#if defined (MSGQ_COMPONENT)
#define CFG_DSP_MQTID           (Uint32) (CFG_DSP_BASE + 12)
#endif /* if defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @const  CFG_LINK_BASE
 *
 *  @desc   Base of the keys to fetch link related information from the
 *          configuration database.
 *  ============================================================================
 */
#define CFG_LINK_BASE           (Uint32) 0x3000

#define CFG_LINK_OBJECT         (Uint32) (CFG_LINK_BASE)
#define CFG_LINK_TABLE          (Uint32) (CFG_LINK_BASE + 1)


/** ============================================================================
 *  @const  CFG_MMU_BASE
 *
 *  @desc   Base of the keys to fetch MMU related information from the
 *          configuration database.
 *  ============================================================================
 */
#define CFG_MMU_BASE            (Uint32) 0x4000

#define CFG_MMU_OBJECT          (Uint32) (CFG_MMU_BASE)
#define CFG_MMU_TABLE           (Uint32) (CFG_MMU_BASE + 1)


#if defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @const  CFG_MQA_BASE
 *
 *  @desc   Base of the keys to fetch MQA related information from the
 *          configuration database.
 *  ============================================================================
 */
#define CFG_MQA_BASE            (Uint32) 0x5000

#define CFG_MQA_OBJECT          (Uint32) (CFG_MQA_BASE)
#define CFG_MQA_NAME            (Uint32) (CFG_MQA_BASE + 1)
#define CFG_MQA_TABLE           (Uint32) (CFG_MQA_BASE + 2)


/** ============================================================================
 *  @const  CFG_MQT_BASE
 *
 *  @desc   Base of the keys to fetch MQT related information from the
 *          configuration database.
 *  ============================================================================
 */
#define CFG_MQT_BASE            (Uint32) 0x6000

#define CFG_MQT_OBJECT          (Uint32) (CFG_MQT_BASE)
#define CFG_MQT_NAME            (Uint32) (CFG_MQT_BASE + 1)
#define CFG_MQT_TABLE           (Uint32) (CFG_MQT_BASE + 2)
#define CFG_MQT_LINKID          (Uint32) (CFG_MQT_BASE + 3)
#endif /* if defined (MSGQ_COMPONENT) */

/** ============================================================================
 *  @const  CFG_ID_LAST
 *
 *  @desc   Last ID used.
 *  ============================================================================
 */
#define CFG_ID_LAST            (Uint32) 0x4FFF


/** ============================================================================
 *  @const  CFG_ID_NONE
 *
 *  @desc   Identifier value used when no ID is associated to the key whose
 *          value is being requested.
 *  ============================================================================
 */
#define CFG_ID_NONE            (Uint32) 0xFFFFFFFF


/** ============================================================================
 *  @func   CFG_Initialize
 *
 *  @desc   This function initializes this sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation Successful.
 *          DSP_EMEMORY
 *              Out of memory error.
 *          DSP_EFAIL
 *              Operation Failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    CFG_Finalize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CFG_Initialize () ;


/** ============================================================================
 *  @func   CFG_Finalize
 *
 *  @desc   This function provides an interface to exit from this
 *          sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation Successful.
 *          DSP_EMEMORY
 *              Out of memory error.
 *          DSP_EFAIL
 *              Operation Failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    CFG_Initialize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CFG_Finalize () ;


/** ============================================================================
 *  @func   CFG_GetRecord
 *
 *  @desc   Gets the record from the configuration.
 *
 *  @arg    key
 *              Key for configuration item.
 *  @arg    id
 *              Context dependent identifier.
 *              The meaning of this argument depends upon the given key.
 *  @arg    record
 *              Location where record is to be stored.
 *
 *  @ret    DSP_SOK
 *              Operation Successful.
 *          DSP_EINVALIDARG
 *              Invalid arguments.
 *          DSP_EFAIL
 *              Operation Failed.
 *
 *  @enter  record must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CFG_GetRecord (IN Uint32 key, IN Uint32 id, OUT Void * record) ;


/** ============================================================================
 *  @func   CFG_GetNumValue
 *
 *  @desc   Gets the numeric value type of configuration.
 *
 *  @arg    key
 *              Key for configuration item.
 *  @arg    id
 *              Context dependent identifier.
 *              The meaning of this argument depends upon the given key.
 *  @arg    value
 *              Location where value is to be stored.
 *
 *  @ret    DSP_SOK
 *              Operation Successful.
 *          DSP_EINVALIDARG
 *              Invalid arguments.
 *          DSP_EFAIL
 *              Operation Failed.
 *
 *  @enter  value must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CFG_GetNumValue (IN Uint32 key, IN Uint32 id, OUT Uint32 * value) ;


/** ============================================================================
 *  @func   CFG_GetStrValue
 *
 *  @desc   Gets the string value type of configuration.
 *
 *  @arg    key
 *              Key for configuration item.
 *  @arg    id
 *              Context dependent identifier.
 *              The meaning of this argument depends upon the given key.
 *  @arg    string
 *              Location where string is to be stored.
 *
 *  @ret    DSP_SOK
 *              Operation Successful.
 *          DSP_EINVALIDARG
 *              Invalid arguments.
 *          DSP_EFAIL
 *              Operation Failed.
 *
 *  @enter  string must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CFG_GetStrValue (IN Uint32 key, IN Uint32 id, OUT Pstr  string) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (CFG_H) */
