/** ============================================================================
 *  @file   prcs.h
 *
 *  @path   $(DSPLINK)\gpp\src\osal
 *
 *  @desc   Defines the interfaces and data structures for the sub-component
 *          PRCS.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (PRCS_H)
#define PRCS_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   PrcsObject
 *
 *  @desc   Forward declaration to a OS specific client identifier object.
 *  ============================================================================
 */
typedef struct PrcsObject_tag  PrcsObject ;


/** ============================================================================
 *  @func   PRCS_Initialize
 *
 *  @desc   Initializes the PRCS subcomponent.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_Initialize () ;


/** ============================================================================
 *  @func   PRCS_Finalize
 *
 *  @desc   Releases resources used by the PRCS subcomponent.
 *
 *  @arg    None.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_Finalize () ;


/** ============================================================================
 *  @func   PRCS_Create
 *
 *  @desc   Creates a PrcsObject and populates it with information to identify
 *          the client.
 *
 *  @arg    prcsObj
 *              OUT argument to store the created object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *
 *  @enter  prcsObj must be a valid pointer.
 *
 *  @leave  Valid object is returned in case of success.
 *
 *  @see    PRCS_Delete
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_Create (OUT PrcsObject ** prcsObj) ;


/** ============================================================================
 *  @func   PRCS_Delete
 *
 *  @desc   Frees up resources used by the specified object.
 *
 *  @arg    prcsObj
 *              Object to be deleted.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EPOINTER
 *              Invalid prcsObj
 *
 *  @enter  prcsObj must be a valid object.
 *
 *  @leave  None
 *
 *  @see    PRCS_Create
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_Delete (IN PrcsObject * prcsObj) ;


/** ============================================================================
 *  @func   PRCS_IsEqual
 *
 *  @desc   Compares two clients to check if they are "equal". Equality is
 *          defined by implementation on the specific OS port.
 *
 *  @arg    client1
 *              First client's information
 *  @arg    client2
 *              Second client's information
 *  @arg    isEqual
 *              Place holder for result of comparison
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *
 *  @enter  client1 must be a valid object.
 *          client2 must be a valid object.
 *          isEqual must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    PRCS_Create
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_IsEqual (IN  PrcsObject *  client1,
              IN  PrcsObject *  client2,
              OUT Bool *        isEqual) ;


/** ============================================================================
 *  @func   PRCS_IsSameContext
 *
 *  @desc   Checks if the two clients share same context (address space).
 *
 *  @arg    client1
 *              First client's information
 *  @arg    client2
 *              Second client's information
 *  @arg    isSame
 *              Place holder for result of comparison
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *
 *  @enter  client1 must be a valid object.
 *          client2 must be a valid object.
 *          isSame must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    PRCS_Create
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_IsSameContext (IN  PrcsObject *  client1,
                    IN  PrcsObject *  client2,
                    OUT Bool *        isSame) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (PRCS_H) */
