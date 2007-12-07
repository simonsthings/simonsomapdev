/** ============================================================================
 *  @file   coff_int.c
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Defines generic functions of COFF loader.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Headers                */
#include <kfile.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <coff_int.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */

/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID         ID_GEN_COFF

/** ============================================================================
 *  @func   COFF_Read8
 *
 *  @desc   Reads a Int8 from file.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Int8
COFF_Read8 (IN KFileObject * fileObj)
{
    Int8 retVal = 0 ;

    TRC_1ENTER ("COFF_Read8", fileObj) ;

    DBC_Require (fileObj != NULL) ;

    KFILE_Read ((Char8 *) &retVal, READ_REC_SIZE, sizeof (Int8), fileObj) ;

    TRC_0LEAVE ("COFF_Read8") ;

    return retVal ;
}


/** ============================================================================
 *  @func   COFF_Read16
 *
 *  @desc   Reads a Int16 from file.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Int16
COFF_Read16 (IN KFileObject * fileObj, IN Bool swap)
{
    Int16 retVal ;

    TRC_2ENTER ("COFF_Read16", fileObj, swap) ;

    DBC_Require (fileObj != NULL) ;

    KFILE_Read ((Char8 *) &retVal, READ_REC_SIZE, sizeof (Int16), fileObj) ;

    if (swap) {
        retVal = SWAP_WORD (retVal) ;
    }

    TRC_0LEAVE ("COFF_Read16") ;

    return retVal ;
}


/** ============================================================================
 *  @func   COFF_Read32
 *
 *  @desc   Reads a Int32 from file.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Int32
COFF_Read32 (IN KFileObject * fileObj, IN Bool swap)
{
    Int32 retVal ;

    TRC_2ENTER ("COFF_Read32", fileObj, swap) ;

    DBC_Require (fileObj != NULL) ;

    KFILE_Read ((Char8 *) &retVal, READ_REC_SIZE, sizeof (Int32), fileObj) ;

    if (swap) {
        retVal = SWAP_LONG (retVal) ;
    }

    TRC_0LEAVE ("COFF_Read32") ;

    return retVal ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
