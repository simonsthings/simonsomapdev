/** ============================================================================
 *  @file   intobject.h
 *
 *  @path   $(DSPLINK)/gpp/inc/Linux/
 *
 *  @desc   Declares an object that encapsulates the interrupt information
 *          reqiured by Linux.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (INTOBJECT_H)
#define INTOBJECT_H


USES (gpptypes.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @name   InterruptObject
 *
 *  @desc   Object encapsulating the OS dependent interrupt information.
 *
 *  @field  intId
 *              Interrupt identifier
 *  ============================================================================
 */
typedef struct InterruptObject_tag {
    Int32  intId ;
} InterruptObject ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (INTOBJECT_H) */
