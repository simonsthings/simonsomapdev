/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== vol_ti_ivol.c ========
 *  VOL Module - TI implementation of a VOL algorithm
 *
 *  This file contains the implementation of the IVOL abstract interface. 
 */

#include <std.h>
#include <string.h>
#include <xdas.h>

#include "ivol.h"
#include "vol_ti_priv.h"

/* data procesing part - scale buffer by gainPercentage */
static Void scale(XDAS_Int16 *buf, Int nWords, Int gainPercentage);

/*
 *  ======== VOL_TI_amplify ========
 */
Void VOL_TI_amplify(IVOL_Handle handle, XDAS_Int16 *in, XDAS_Int16 *out)
{
    VOL_TI_Obj *vol = (VOL_TI_Obj *)handle;
    
    /* Use memcpy to copy input buffer into scaling buffer */
    memcpy((void *)(vol->scaleBuf),(void *)in,(Uns)((vol->frameLen) * sizeof(XDAS_Int16)));

    /* we process the scaling buffer */      
    scale(vol->scaleBuf, vol->frameLen, vol->gainPercentage);
    
    /* done with processing the scaling buffer, now copy it to out */
    memcpy((void *)(out),(void *)(vol->scaleBuf), (Uns)((vol->frameLen) * sizeof(XDAS_Int16)));
}

/*
 *  ======== VOL_TI_control ========
 */
Int VOL_TI_control(IVOL_Handle handle, IVOL_Cmd cmd, IVOL_Status *status)
{
    VOL_TI_Obj *vol = (VOL_TI_Obj *)handle;

    switch (cmd) {
        case IVOL_GETSTATUS:
            status->gainPercentage = vol->gainPercentage;
            break;
        case IVOL_SETSTATUS:
            vol->gainPercentage = status->gainPercentage;
            break;
        default:
            return IALG_EFAIL;
    }

    return ((Int )IALG_EOK);
}

/*
 *  ======== scale ========
 */
static Void scale(XDAS_Int16 *buf, Int nWords, Int gainPercentage)
{
    XDAS_Int16 i;
    /* convert gain percent from 1/100 to 1/256 so we can shift, not divide */
    XDAS_Int32 gain = ( ((XDAS_Int32)gainPercentage << 8) / (XDAS_Int32)100 );
    
    for (i = 0; i < nWords; i++)
    {
        buf[i] = (XDAS_Int16) (((XDAS_Int32)buf[i] * gain) >> 8);
    }
}

