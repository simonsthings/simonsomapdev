/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== mod_configure.h ========
 */
#ifndef MOD_CONFIGURE_
#define MOD_CONFIGURE_
 
/* 
 * 1. +REPLACE fir WITH YOUR XDAIS ALGORITHM MODULE NAME BELOW. FOR EXAMPLE,
 * IF THE XDAIS ALGORITHM IS g729enc_ti.l62 THEN CHANGE THE fir TO
 * g729enc AND REBUILD/EXECUTE THIS PRE-PROGRAM. USE LOWER-CASE+
 * 
 * 2. +SET NUMBER OF CHANNELS YOUR APPLICATION NEEDS FOR NAMED ALGORITHM+
 * 
 * 3. +UPDATE/SET STATIC PARAMETERS FOR EACH CHANNEL+
 *
 * +REPEAT THIS FOR SEQUENCE FOR NEXT/OTHER ALGORITHMS+
 */
#define MODULENAME          fir 
#define NUMCHANNELS         1

#endif  /* MOD_CONFIGURE_ */


