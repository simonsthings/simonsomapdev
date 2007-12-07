/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/* 
 *  ======== ad50.h ========
 */

#ifndef AD50_
#define AD50_

#include <std.h>

#include <csl.h>
#include <csl_mcbsp.h>

#define AD50_CONTROL1_DEFAULT 0x80
#define AD50_CONTROL2_DEFAULT 0x00
#define AD50_CONTROL3_DEFAULT 0x00
#define AD50_CONTROL4_8KHZ    0xa0
#define AD50_CONTROL4_16KHZ   0x90
#define AD50_CONTROL4_DEFAULT AD50_CONTROL4_8KHZ        

/*
 *  Codec registers are kept in an object having the structure below.
 *  Refer to the AD50 codec documentation for more info about these register.
 */
typedef struct AD50_Params {
    Uns control1; /* software reset */
    Uns control2; /*                */
    Uns control3; /*                */
    Uns control4; /* sample rate    */
} AD50_Params;

#define AD50_DEFAULTPARAMS {                                    \
    AD50_CONTROL1_DEFAULT,                                      \
    AD50_CONTROL2_DEFAULT,                                      \
    AD50_CONTROL3_DEFAULT,                                      \
    AD50_CONTROL4_DEFAULT                                       \
}

/*
 *  ======== AD50_init ========
 */
extern Void AD50_init();

/*
 *  ======== AD50_setParams ========
 *
 *  This function takes a pointer to the object of type AD50_Params,
 *  and writes all 4 control words found in it to the codec.
 */
extern Void AD50_setParams( MCBSP_Handle hMcbsp, AD50_Params *params );

#endif /* AD50_ */

