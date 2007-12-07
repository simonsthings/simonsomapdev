/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appIO.c ========
 */

#include <std.h>

// App includes
#include "appIO.h"


Void appIOInit() {
    /* 
     * This function can be used to initialize and setup drivers  
     * used by this project. 
     */
}

extern Void gblUserInit() 
{
    CSL_sysInit();
}



