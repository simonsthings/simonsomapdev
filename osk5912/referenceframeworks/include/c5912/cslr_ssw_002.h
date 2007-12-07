#ifndef _CSLR_SSW_001_H_
#define _CSLR_SSW_001_H_

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 UART1_CFGREG;
    const char RSVD0[15];
    volatile Uint16 UART2_CFGREG;
    const char RSVD1[15];
    volatile Uint16 UART3_CFGREG;
    const char RSVD2[39];
    volatile Uint16 MCBSP2_CFGREG;
    const char RSVD3[7];
    volatile Uint16 I2C_CFGREG;
    const char RSVD4[7];
    volatile Uint16 SPI_CFGREG;
    const char RSVD5[7];
    volatile Uint16 DMTIMER1_CFGREG;
    const char RSVD6[7];
    volatile Uint16 DMTIMER2_CFGREG;
    const char RSVD7[7];
    volatile Uint16 DMTIMER3_CFGREG;
    const char RSVD8[7];
    volatile Uint16 DMTIMER4_CFGREG;
    const char RSVD9[7];
    volatile Uint16 DMTIMER5_CFGREG;
    const char RSVD10[7];
    volatile Uint16 DMTIMER6_CFGREG;
    const char RSVD11[15];
    volatile Uint16 DMTIMER7_CFGREG;
    const char RSVD12[7];
    volatile Uint16 DMTIMER8_CFGREG;
    const char RSVD13[15];
    volatile Uint16 MMCSD2_CFGREG;
} CSL_SswRegs;


/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* UART1_CFGREG */

#define CSL_SSW_UART1_CFGREG_DSPSW_MASK  (0x00000002u)
#define CSL_SSW_UART1_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_UART1_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_UART1_CFGREG_MCUSW_MASK  (0x00000001u)
#define CSL_SSW_UART1_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_UART1_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_UART1_CFGREG_RESETVAL    (0x00000001u)

/* UART2_CFGREG */

#define CSL_SSW_UART2_CFGREG_DSPSW_MASK  (0x00000002u)
#define CSL_SSW_UART2_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_UART2_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_UART2_CFGREG_MCUSW_MASK  (0x00000001u)
#define CSL_SSW_UART2_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_UART2_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_UART2_CFGREG_RESETVAL    (0x00000001u)

/* UART3_CFGREG */

#define CSL_SSW_UART3_CFGREG_DSPSW_MASK  (0x00000002u)
#define CSL_SSW_UART3_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_UART3_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_UART3_CFGREG_MCUSW_MASK  (0x00000001u)
#define CSL_SSW_UART3_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_UART3_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_UART3_CFGREG_RESETVAL    (0x00000001u)

/* MCBSP2_CFGREG */

#define CSL_SSW_MCBSP2_CFGREG_DSPSW_MASK (0x00000002u)
#define CSL_SSW_MCBSP2_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_MCBSP2_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_MCBSP2_CFGREG_MCUSW_MASK (0x00000001u)
#define CSL_SSW_MCBSP2_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_MCBSP2_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_MCBSP2_CFGREG_RESETVAL   (0x00000001u)

/* I2C_CFGREG */

#define CSL_SSW_I2C_CFGREG_DSPSW_MASK    (0x00000002u)
#define CSL_SSW_I2C_CFGREG_DSPSW_SHIFT   (0x00000001u)
#define CSL_SSW_I2C_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_I2C_CFGREG_MCUSW_MASK    (0x00000001u)
#define CSL_SSW_I2C_CFGREG_MCUSW_SHIFT   (0x00000000u)
#define CSL_SSW_I2C_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_I2C_CFGREG_RESETVAL      (0x00000001u)

/* SPI_CFGREG */

#define CSL_SSW_SPI_CFGREG_DSPSW_MASK    (0x00000002u)
#define CSL_SSW_SPI_CFGREG_DSPSW_SHIFT   (0x00000001u)
#define CSL_SSW_SPI_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_SPI_CFGREG_MCUSW_MASK    (0x00000001u)
#define CSL_SSW_SPI_CFGREG_MCUSW_SHIFT   (0x00000000u)
#define CSL_SSW_SPI_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_SPI_CFGREG_RESETVAL      (0x00000001u)

/* DMTIMER1_CFGREG */

#define CSL_SSW_DMTIMER1_CFGREG_DSPSW_MASK (0x00000002u)
#define CSL_SSW_DMTIMER1_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_DMTIMER1_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_DMTIMER1_CFGREG_MCUSW_MASK (0x00000001u)
#define CSL_SSW_DMTIMER1_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_DMTIMER1_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_DMTIMER1_CFGREG_RESETVAL (0x00000001u)

/* DMTIMER2_CFGREG */

#define CSL_SSW_DMTIMER2_CFGREG_DSPSW_MASK (0x00000002u)
#define CSL_SSW_DMTIMER2_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_DMTIMER2_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_DMTIMER2_CFGREG_MCUSW_MASK (0x00000001u)
#define CSL_SSW_DMTIMER2_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_DMTIMER2_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_DMTIMER2_CFGREG_RESETVAL (0x00000001u)

/* DMTIMER3_CFGREG */

#define CSL_SSW_DMTIMER3_CFGREG_DSPSW_MASK (0x00000002u)
#define CSL_SSW_DMTIMER3_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_DMTIMER3_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_DMTIMER3_CFGREG_MCUSW_MASK (0x00000001u)
#define CSL_SSW_DMTIMER3_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_DMTIMER3_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_DMTIMER3_CFGREG_RESETVAL (0x00000001u)

/* DMTIMER4_CFGREG */

#define CSL_SSW_DMTIMER4_CFGREG_DSPSW_MASK (0x00000002u)
#define CSL_SSW_DMTIMER4_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_DMTIMER4_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_DMTIMER4_CFGREG_MCUSW_MASK (0x00000001u)
#define CSL_SSW_DMTIMER4_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_DMTIMER4_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_DMTIMER4_CFGREG_RESETVAL (0x00000001u)

/* DMTIMER5_CFGREG */

#define CSL_SSW_DMTIMER5_CFGREG_DSPSW_MASK (0x00000002u)
#define CSL_SSW_DMTIMER5_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_DMTIMER5_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_DMTIMER5_CFGREG_MCUSW_MASK (0x00000001u)
#define CSL_SSW_DMTIMER5_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_DMTIMER5_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_DMTIMER5_CFGREG_RESETVAL (0x00000001u)

/* DMTIMER6_CFGREG */

#define CSL_SSW_DMTIMER6_CFGREG_DSPSW_MASK (0x00000002u)
#define CSL_SSW_DMTIMER6_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_DMTIMER6_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_DMTIMER6_CFGREG_MCUSW_MASK (0x00000001u)
#define CSL_SSW_DMTIMER6_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_DMTIMER6_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_DMTIMER6_CFGREG_RESETVAL (0x00000001u)

/* DMTIMER7_CFGREG */

#define CSL_SSW_DMTIMER7_CFGREG_DSPSW_MASK (0x00000002u)
#define CSL_SSW_DMTIMER7_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_DMTIMER7_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_DMTIMER7_CFGREG_MCUSW_MASK (0x00000001u)
#define CSL_SSW_DMTIMER7_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_DMTIMER7_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_DMTIMER7_CFGREG_RESETVAL (0x00000001u)

/* DMTIMER8_CFGREG */

#define CSL_SSW_DMTIMER8_CFGREG_DSPSW_MASK (0x00000002u)
#define CSL_SSW_DMTIMER8_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_DMTIMER8_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_DMTIMER8_CFGREG_MCUSW_MASK (0x00000001u)
#define CSL_SSW_DMTIMER8_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_DMTIMER8_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_DMTIMER8_CFGREG_RESETVAL (0x00000001u)

/* MMCSD2_CFGREG */

#define CSL_SSW_MMCSD2_CFGREG_DSPSW_MASK (0x00000002u)
#define CSL_SSW_MMCSD2_CFGREG_DSPSW_SHIFT (0x00000001u)
#define CSL_SSW_MMCSD2_CFGREG_DSPSW_RESETVAL (0x00000000u)

#define CSL_SSW_MMCSD2_CFGREG_MCUSW_MASK (0x00000001u)
#define CSL_SSW_MMCSD2_CFGREG_MCUSW_SHIFT (0x00000000u)
#define CSL_SSW_MMCSD2_CFGREG_MCUSW_RESETVAL (0x00000001u)

#define CSL_SSW_MMCSD2_CFGREG_RESETVAL   (0x00000001u)

#endif