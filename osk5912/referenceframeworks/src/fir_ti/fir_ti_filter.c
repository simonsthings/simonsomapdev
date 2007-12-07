/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== fir_ti_filter.c ========
 *  Implementation of the filter operation. 
 */

#pragma     CODE_SECTION(FIR_TI_filter,         ".text:filter")
#pragma     CODE_SECTION(FIR_TI_gen,            ".text:filter")
 
#include    <std.h>
#include    <string.h>
#include    <xdas.h>

#include    "ifir.h"
#include    "fir_ti.h"
#include    "fir_ti_priv.h"


/*
 *======== FIR_TI_gen ========
 *
 * in = input array
 * coeff = coefficient array
 * out = output array
 * nCoeff = number of coefficients (nCoeff >= 5)
 * nout = number of output samples (nout >= 1)
 *
 * Computes a real FIR filter (direct-form) using coefficients stored
 * in vector coeff. The real data input is stored in vector in. The filter 
 * output result is stored in vector out. This routine has no memory
 * hits regardless of where in, coeff, and out arrays are located in
 * memory. The filter is nout output samples and nCoeff coefficients.
 *
 * No memory bank hits under any conditions.
 *
 */
static Void FIR_TI_gen(XDAS_Int16 *in, XDAS_Int16 *coeff, XDAS_Int16 *out, XDAS_Int16 nCoeff, XDAS_Int16 nout)
{
    XDAS_Int16  i, j;
    XDAS_Int32  sum;  
    
    for (j = 0; j < nout; j++) 
    {
        sum = 0;
        for (i = 0; i < nCoeff; i++)
            sum += (XDAS_Int32)(in[i + j]) * (XDAS_Int32)(coeff[i]);
        out[j] = sum >> 15;
    }
}
        
    
/*
 *  ======== FIR_TI_filter ========
 *  TI's implementation of the filter operation.
 */    
Void FIR_TI_filter(IFIR_Handle handle, Short in[], Short out[])
{
    FIR_TI_Obj *fir = (Void *)handle;
    Short *src = fir->workBuf;
    Short *dst = out;
    
    /* copy input buffer into working buffer */
    memcpy((Void *)(fir->workBuf + fir->filterLenM1), (Void *)in, fir->frameLen * sizeof (Short));

    /* call fir_gen to filter data */
    FIR_TI_gen(src, fir->coeff, dst, fir->filterLenM1 + 1, fir->frameLen);

    /* shift filter history to beginning of working buffer for next frame */
    memcpy((Void *)fir->workBuf, (Void *)(fir->workBuf + fir->frameLen), fir->filterLenM1 * sizeof (Short));
}


