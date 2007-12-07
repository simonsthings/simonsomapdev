/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

/* ==============================================================================
 * The purpose of this header file is to gather all the primitive "C"
 * data types into one consolidated file. This file is expected to be
 * included in the basic types file exported by other software components
 * such as for example CSL. This file will *not* specify the types
 * for aggregate data such as "C" structures or vectored quantities.
 * All the types defined in this header shall be bracked with _TI_STD_TYPES
 * symbol definition scope as a means of protection against multiple definitions
 * -----------------------------------------------------------------------------
 */

#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES

typedef unsigned short  Bool;   /* Boolean truth value - 0 or 1 only */
typedef int             Int;    /* Signed base integer quanity */
typedef unsigned int    Uns;    /* This is depricated support only */
typedef char            Char;
typedef char*           String;
typedef void*           Ptr;    /* Arbitrary pointer (works for pointing to any datum) */

/* Unsigned integer definitions (32bit, 16bit, 8bit) follow... */ 
typedef unsigned long   Uint32;
typedef unsigned int    Uint16;
typedef unsigned char   Uint8;

/* Signed integer definitions (32bit, 16bit, 8bit) follow... */ 
typedef long            Int32;
typedef int             Int16;
typedef char            Int8; 

#endif /* _TI_STD_TYPES */
