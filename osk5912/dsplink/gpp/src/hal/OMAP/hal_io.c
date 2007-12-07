/** ============================================================================
 *  @file   hal_io.c
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Defines necessary functions for IO operations.
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

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                   */
#include <profile.h>

/*  ----------------------------------- OSAL Headers                */
#include <print.h>

/*  ----------------------------------- Hardware Abstraction Layer  */
#include <hal_object.h>
#include <hal_io.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_HAL_IO

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_HAL_IO, __LINE__)


/** ============================================================================
 *  @func   HAL_ReadDspData
 *
 *  @desc   Use HPI to read the DSP internal memory
 *          This function assumes that the numBytes specified is a multiple of
 *          2 since the DSP wordsize is 2.
 *          Also for shared memory it is assumed that the starting address is
 *          aligned.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ReadDspData (IN  HalObject * halObject,
                 IN  Endianism   endianism,
                 OUT Uint8 *     hostBuf,
                 IN  Uint32      dspAddr,
                 IN  Uint32      numBytes)
{
    Uint32            baseAddr = halObject->baseDSPMEM ;
    Uint32            i                                ;
    Uint32            offset                           ;
    Uint16            temp                             ;

    TRC_5ENTER ("HAL_ReadDspData",
                halObject, endianism, hostBuf, dspAddr, numBytes) ;

    DBC_Require (halObject != NULL) ;

    offset = dspAddr ;

    /* If address is ODD, read first byte */
    if (dspAddr % 2) {
        offset-- ;

        *(hostBuf++) = *((Uint8 *) (baseAddr + offset)) ;

        numBytes-- ;
        offset += 2 ;
    }

    /* DSP is word accessible. Two bytes are read per DSP Address */
    if (endianism == Endianism_Big) {
        for (i = 0 ; i < (numBytes / 2) ; i++) {
            temp = REG16 (baseAddr + offset) ;

            *(hostBuf++) = (Uint8) temp ;
            *(hostBuf++) = (Uint8) (temp >> 8) ;

            offset += 2 ;
        }
    }
    else {
        for (i = 0 ; i < (numBytes / 2) ; i++) {
            temp = REG16 (baseAddr + offset) ;

            *(hostBuf++) = (Uint8) (temp >> 8) ;
            *(hostBuf++) = (Uint8) temp ;

            offset += 2 ;
        }
    }

    /* If number of bytes requested is ODD, read last byte */
    if (numBytes % 2) {
        temp = REG16 (baseAddr + offset) ;

        *(hostBuf++) = (Uint8) (temp >> 8) ;
    }

    TRC_0LEAVE ("HAL_ReadDspData") ;
}


/** ============================================================================
 *  @func   HAL_WriteDspData
 *
 *  @desc   Use HPI to write to the DSP internal memory
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_WriteDspData (IN HalObject * halObject,
                  IN Endianism   endianism,
                  IN Uint8 *     hostBuf,
                  IN Uint32      dspAddr,
                  IN Uint32      numBytes)
{
    Uint32  baseAddr  = halObject->baseDSPMEM ;
    Uint32  i                                 ;
    Uint32  offset                            ;
    Uint8   tempByte1                         ;
    Uint8   tempByte2                         ;
    Uint16  temp16                            ;

    TRC_5ENTER ("HAL_WriteDspData",
                halObject, endianism, hostBuf, dspAddr, numBytes) ;

    DBC_Require (halObject != NULL) ;

    offset = dspAddr ;

    /* If address is ODD, write first byte */
    if (dspAddr % 2) {
        offset-- ;

        tempByte1 = *(hostBuf++) ;

        temp16  = REG16 (baseAddr + offset) ;

        if (endianism == Endianism_Big) {
            temp16  = (Uint16) ((tempByte1 << 8) | (temp16 & 0x00FF));
        }
        else {
            temp16  = (Uint16) (temp16 << 8) ;
            temp16 |= (Uint16) tempByte1 ;
        }

        REG16 (baseAddr + offset) = temp16 ;

        offset += 2 ;
        numBytes-- ;
    }

    /* DSP is word accessible. Two bytes are written per DSP Address */
    if (endianism == Endianism_Big) {
        for (i = 0 ; i < (numBytes / 2) ; i++) {
            tempByte1 = *(hostBuf++) ;
            tempByte2 = *(hostBuf++) ;

            temp16  = (Uint16) tempByte1 ;
            temp16 |= (Uint16) (tempByte2 << 8) ;

            REG16 (baseAddr + offset) = temp16 ;

            offset += 2 ;
        }
    }
    else {
        for (i = 0 ; i < (numBytes / 2) ; i++) {
            tempByte1 = *(hostBuf++) ;
            tempByte2 = *(hostBuf++) ;

            temp16  = (Uint16) tempByte2 ;
            temp16 |= (Uint16) (tempByte1 << 8) ;

            REG16 (baseAddr + offset) = temp16 ;

            offset += 2 ;
        }
    }

    /* If number of bytes requested is ODD, write last byte */
    if (numBytes % 2) {
        temp16 = REG16 (baseAddr + offset) ;

        if (endianism == Endianism_Big) {
            temp16  = (Uint16) ((temp16 & 0xFF00) | (*hostBuf)) ;
        }
        else {
            temp16  = (Uint16) (temp16 >> 8) ;
            temp16 |= (Uint16) ((*hostBuf) << 8) ;
        }

        REG16 (baseAddr + offset) = temp16 ;
    }

    TRC_0LEAVE ("HAL_WriteDspData") ;
}


/** ============================================================================
 *  @func   HAL_WriteExtDspData
 *
 *  @desc   Write to External memory.
 *          This function assumes that the numBytes specified is a multiple of
 *          2 since the DSP wordsize is 2.
 *          Also for shared memory it is assumed that the starting address is
 *          aligned.
 *
 *  @modif  Nond
 *  ============================================================================
 */
NORMAL_API
Void
HAL_WriteExtDspData (IN HalObject *  halObject,
                     IN Endianism    endianism,
                     IN Uint8 *      hostBuf,
                     IN Uint32       dspAddr,
                     IN Uint32       numBytes)
{
    Uint32      bytes    = 0    ;
    Int32       remain   = 0    ;
    Int32       start    = 0    ;
    Uint32      offset          ;
    Uint32      temp32          ;
    Uint16      temp16_1        ;
    Uint16      temp16_2        ;
    Uint8       tempByte1       ;
    Uint8       tempByte2       ;
    Uint8       remainByte [4]  ;
    Int32       i               ;

    TRC_5ENTER ("HAL_WriteExtDspData",
                halObject, endianism, hostBuf, dspAddr, numBytes) ;

    TRC_1PRINT (TRC_LEVEL1, "Bytes to be written: [%d]", numBytes) ;

    DBC_Require (halObject != NULL) ;

    for (i = 0 ; i < 4 ; i++) {
        remainByte [i] = 0x0 ;
    }

    offset = dspAddr ;

    /* If the starting address is not word aligned */
    start = (dspAddr % 4) ;
    if (start) {
        Uint32 tmpStart = dspAddr - start ;

        for (i = start ; i < 4 ; i++) {
           remainByte [i] = *(hostBuf++) ;
        }

        /* swap two bytes and place in temp16_1 */
        temp16_1  = (Uint16) remainByte [1] ;
        temp16_1 |= (Uint16) (remainByte [0] << 8) ;

        /* swap next two-bytes and place in temp16_2 */
        temp16_2  = (Uint16) remainByte [3] ;
        temp16_2 |= (Uint16) (remainByte [2] << 8) ;

        /* create 32 bit temp32 with word swapped temp16_1 and temp16_2 */
        temp32  = (Uint32) temp16_2 ;
        temp32 |= (Uint32) (temp16_1 << 16) ;

        /*  Finally append bytes to the original data */
        *(Uint32 *) (tmpStart) |= temp32 ;

        bytes += (4 - start) ;

        offset += bytes ;
    }

    for (i = 0 ; i < 4 ; i++) {
        remainByte [i] = 0x0 ;
    }

    while (bytes < numBytes) {
        remain = numBytes - bytes ;
        /* This is the terminating condition - i.e if there are less than
         * four bytes left.
         */
        if (remain < 4) {
            TRC_1PRINT (TRC_LEVEL1, "Remaining Bytes: [%d]", remain) ;

            for (i = 0 ; i < remain ; i++) {
                remainByte [i] = *(hostBuf++) ;
            }

            /* swap two bytes and place in temp16_1 */
            temp16_1  = (Uint16) remainByte [1] ;
            temp16_1 |= (Uint16) (remainByte [0] << 8) ;

            /* swap next two-bytes and place in temp16_2 */
            temp16_2  = (Uint16) remainByte [3] ;
            temp16_2 |= (Uint16) (remainByte [2] << 8) ;

            /* create 32 bit temp32 with word swapped temp16_1 and temp16_2 */
            temp32  = (Uint32) temp16_2 ;
            temp32 |= (Uint32) (temp16_1 << 16) ;

            *(Uint32 *) (offset) = temp32 ;

            bytes += remain ;
        }
        else {
            /* first two bytes */
            tempByte1 = *(hostBuf++) ;
            tempByte2 = *(hostBuf++) ;

            /* swap two bytes and place in temp16_1 */
            temp16_1  = (Uint16) tempByte2 ;
            temp16_1 |= (Uint16) (tempByte1 << 8) ;

            /* next two-bytes */
            tempByte1 = *(hostBuf++) ;
            tempByte2 = *(hostBuf++) ;

            /* swap next two-bytes and place in temp16_2 */
            temp16_2  = (Uint16) tempByte2 ;
            temp16_2 |= (Uint16) (tempByte1 << 8) ;

            /* create 32 bit temp32 with word swapped temp16_1 and temp16_2 */
            temp32  = (Uint32) temp16_2 ;
            temp32 |= (Uint32) (temp16_1 << 16) ;

            *(Uint32 *) (offset) = temp32 ;

            offset += 4 ;
            bytes  += 4 ;
        }
    }

    TRC_1PRINT (TRC_LEVEL1, "Total Bytes Written : [%d]", bytes) ;

#if defined (DDSP_DEBUG)
    if (bytes != numBytes) {
        TRC_2PRINT (TRC_LEVEL1,"Error: Bytes Requested : [%d] Written : [%d]",
                                numBytes, bytes) ;
    }
#endif  /* defined (DDSP_DEBUG) */

    TRC_0LEAVE ("HAL_WriteExtDspData") ;
}


#if defined (__cplusplus)
}
#endif
