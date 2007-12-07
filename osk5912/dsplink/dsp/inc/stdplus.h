/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== stdplus.h ========
 */

#ifndef STD_
#warn std.h must be included before stdplus.h
#endif

#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES

/* Handle the 6x ISA */
#if defined(_6x_)

/* Unsigned integer definitions (32bit, 16bit, 8bit) follow... */ 
typedef unsigned        Uint32;
typedef unsigned short  Uint16;
typedef unsigned char   Uint8;

/* Signed integer definitions (32bit, 16bit, 8bit) follow... */ 
typedef int             Int32;
typedef short int       Int16;
typedef char            Int8; 

/* Handle the 54 and 55 ISAs */
#elif defined(_54_) || defined(_55_)

/* Unsigned integer definitions (32bit, 16bit) follow... */ 
typedef unsigned long   Uint32;
typedef unsigned        Uint16;

/* 
 *  Legacy support : unsigned char should be used in favor of Uint8
 *  on C5000 platforms since it does not support exact 8-bit types
 */
typedef unsigned char   Uint8;

/* Signed integer definitions (32bit, 16bit) follow... */ 
typedef long int        Int32;
typedef int             Int16;

/* 
 *  Legacy support : char should be used in favor of Int8
 *  on C5000 platforms since it does not support exact 8-bit types
 */
typedef char            Int8; 

/* Other ISAs not supported */
#else
#error The ISA used is not supported.
#endif /* defined(_6x) */

#endif /* _TI_STD_TYPES */



