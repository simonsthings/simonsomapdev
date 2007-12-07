/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *    ======== osk5912.h ========
 *
 */

#ifndef OSK5912_
#define OSK5912_

#ifdef __cplusplus
extern "C" {
#endif

#include <5912.h>                   // Omap 1512 Register Set
#include <util.h>

/* ------------------------------------------------------------------------ *
 *  Compatability definitions                                               *
 * ------------------------------------------------------------------------ */
#define NULL                        0

typedef unsigned int                Uint32;
typedef unsigned short              Uint16;
typedef unsigned char               Uint8;

typedef volatile unsigned int       VUint32;
typedef volatile unsigned short     VUint16;
typedef volatile unsigned char      VUint8;

typedef signed int                  Int32;
typedef signed short                Int16;
typedef signed char                 Int8;

typedef volatile signed int         VInt32;
typedef volatile signed short       VInt16;
typedef volatile signed char        VInt8;

/* ------------------------------------------------------------------------ *
 *  Vector Table Offsets                                                    *
 * ------------------------------------------------------------------------ */
typedef enum {
    VECTOR_RESET                = 0,
    VECTOR_UNDEFINED            ,
    VECTOR_SOFTWARE_INTERRUPTS  ,
    VECTOR_PREFETCH_ABORT       ,
    VECTOR_DATA_ABORT           ,
    VECTOR_RESERVED             ,
    VECTOR_IRQ                  ,
    VECTOR_FIQ
} RESET_VECTOR_INDEX;

/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
void OSK5912_init( Uint16 freq );

void OSK5912_releaseDsp( );
void OSK5912_resetDsp( );

void OSK5912_wait( Uint32 cycles );
void OSK5912_waitmsec( Uint32 msec );
void OSK5912_waitusec( Uint32 usec );

#ifdef __cplusplus
}
#endif

#endif
