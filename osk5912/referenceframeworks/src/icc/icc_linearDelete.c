/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== icc_linearDelete.c ========
 *  Functions for the deletion of the linear ICC object.
 */

#pragma CODE_SECTION(ICC_linearDelete, ".text:ICC_linearDelete")

#include <std.h>

#include <mem.h>

#include <icc_linear.h>
#include <utl.h>

/*
 *  ======== ICC_linearDelete ========
 *  Delete the linear ICC object.
 *  The object is always created from the segment that contains 
 *  DSP/BIOS objects. 
 */
Bool ICC_linearDelete(ICC_LinearHandle linearIccHandle)
{  
    /* Check parameters.  Make sure it is a linear ICC. */
    UTL_assert(linearIccHandle != NULL);
    UTL_assert(linearIccHandle->obj.objType == ICC_LINEAROBJ);

    /* Delete the object from the DSP/BIOS segment */
    return (MEM_free(0, linearIccHandle, sizeof(ICC_LinearObj)));
}



