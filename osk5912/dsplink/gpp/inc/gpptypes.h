/** ============================================================================
 *  @file   gpptypes.h
 *
 *  @path   $(DSPLINK)\gpp\inc
 *
 *  @desc   Defines the type system for DSP/BIOS Link
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (GPPTYPES_H)
#define GPPTYPES_H


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  IN/OUT/OPTIONAL/CONST
 *
 *  @desc   Argument specification syntax
 *  ============================================================================
 */
#define IN                              /* The argument is INPUT  only */
#define OUT                             /* The argument is OUTPUT only */
#define OPT                             /* The argument is OPTIONAL    */
#define CONST   const

/** ============================================================================
 *  @macro  USES
 *
 *  @desc   Empty macro to indicate header file dependency
 *  ============================================================================
 */
#define USES(filename)


/** ============================================================================
 *  @macro  Data types
 *
 *  @desc   Basic data types
 *  ============================================================================
 */
typedef signed char         Int8 ;      /*  8 bit value */
typedef signed short int    Int16 ;     /* 16 bit value */
typedef signed long  int    Int32 ;     /* 32 bit value */

typedef unsigned char       Uint8 ;     /*  8 bit value */
typedef unsigned short int  Uint16 ;    /* 16 bit value */
typedef unsigned long  int  Uint32 ;    /* 32 bit value */

typedef float               Real32 ;    /* 32 bit value */
typedef double              Real64 ;    /* 64 bit value */

typedef short int           Bool ;      /* 16 bit value */

typedef char                Char8 ;     /*  8 bit value */
typedef short               Char16 ;    /* 16 bit value */

typedef unsigned char       Uchar8 ;    /*  8 bit value */
typedef unsigned short      Uchar16 ;   /* 16 bit value */

typedef void                Void ;
typedef void *              Pvoid ;

typedef Char8 *             Pstr ;
typedef Uchar8 *            Pustr ;


/** ============================================================================
 *  @const  TRUE/FALSE
 *
 *  @desc   Boolean constants
 *  ============================================================================
 */
#if !defined (FALSE)
#define FALSE   0
#endif

#if !defined (TRUE)
#define TRUE    1
#endif


/** ============================================================================
 *  @const  NULL
 *
 *  @desc   Definition is language specific
 *  ============================================================================
 */
#if !defined (NULL)

#if defined (__cplusplus)
#define NULL    0
#else  /* defined (__cplusplus) */
#define NULL ((void *)0)
#endif /* defined (__cplusplus) */

#endif /* !defined (NULL) */


/** ============================================================================
 *  @macro  REG8/REG16/REG32
 *
 *  @desc   Macros to access register fields.
 *  ============================================================================
 */
#define REG8(A)         (*(volatile Char8  *) (A))
#define REG16(A)        (*(volatile Uint16 *) (A))
#define REG32(A)        (*(volatile Uint32 *) (A))


/** ============================================================================
 *  @macro  DSP/BIOS Link specific types
 *
 *  @desc   These types are used across DSP/BIOS Link.
 *  ============================================================================
 */
typedef Uint32     ProcessorId ;
typedef Uint32     ChannelId ;
typedef Uint32     LinkId ;

#if defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @macro  MsgQueueId
 *
 *  @desc   This type is used for identifying the different message queues.
 *  ============================================================================
 */
typedef Uint16     MsgQueueId ;

/** ============================================================================
 *  @macro  AllocatorId
 *
 *  @desc   This type is used for identifying the different allocators used by
 *          DSPLINK.
 *  ============================================================================
 */
typedef Uint16     AllocatorId ;

/** ============================================================================
 *  @macro  TransportId
 *
 *  @desc   This type is used for identifying the different transports used by
 *          DSPLINK.
 *  ============================================================================
 */
typedef Uint16     TransportId ;
#endif /* if defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @macro  OS Specific standard definitions
 *
 *  @desc   Free for OEMs to add their own generic stuff, if they so desire
 *  ============================================================================
 */
#if defined (OS_WINCE)

#endif  /* defined (OS_WINCE) */


#if defined (OS_NUCLEUS)

#endif  /* defined (OS_NUCLEUS) */


#if defined (OS_LINUX)

#endif  /* defined (OS_LINUX) */


/** ============================================================================
 *  @macro  Calling convention
 *
 *  @desc   Definition of CDECL, DLLIMPORT, DLLEXPORT can be defined by
 *          OEM for his compiler
 *  ============================================================================
 */
#define STATIC          static
#define EXTERN          extern


#if defined (OS_WINCE)
/*  ------------------------------------------- WINCE               */
#define CDECL           __cdecl
#define DLLIMPORT       __declspec (dllexport)
#define DLLEXPORT       __declspec (dllexport)
/*  ------------------------------------------- WINCE               */
#endif  /* defined (OS_WINCE) */


#if defined (OS_NUCLEUS)
/*  ------------------------------------------- NUCLEUS             */
#define CDECL
#define DLLIMPORT
#define DLLEXPORT
/*  ------------------------------------------- NUCLEUS             */
#endif  /* defined (OS_NUCLEUS) */

#if defined (OS_LINUX)
/*  ------------------------------------------- LINUX               */
#define CDECL
#define DLLIMPORT
#define DLLEXPORT
/*  ------------------------------------------- LINUX               */
#endif  /* defined (OS_NUCLEUS) */


/* Derived calling conventions */
#define NORMAL_API      CDECL
#define IMPORT_API      DLLIMPORT
#define EXPORT_API      DLLEXPORT


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

#endif  /* !defined (GPPTYPES_H) */
