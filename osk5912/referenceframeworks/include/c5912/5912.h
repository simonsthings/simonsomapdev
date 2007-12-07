#ifndef _5912_H_
#define _5912_H_

/**************************************************************************\
* 5912 C55 soc file
\**************************************************************************/

#include <cslr.h>

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
/** @brief Number of UART instances */
#define CSL_UART_CNT                  3

/** @brief Number of Static Switch(SSW) instances */
#define CSL_SSW_CNT                   1

/** @brief Number of DMA instances */
#define CSL_DMA_CNT                   1

/** @brief Number of IRDA instances */
#define CSL_IRDA_CNT                  2

/** @brief Number of MCBSP instances */
#define CSL_MCBSP_CNT                 3

/** @brief Number of I2C instances */
#define CSL_I2C_CNT                   1

/** @brief Number of TIMER instances */
#define CSL_TIMER_CNT                 3

/** @brief Number of INTC20 instances */
#define CSL_INTC20_CNT                1

/** @brief Number of INTC21 instances */
#define CSL_INTC21_CNT                1

/** @brief Number of CLKRST instances */
#define CSL_CLKRST_CNT                 1

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
/** @brief Base address of CHIP peripheral registers */
#define CSL_CHIP_REGS                 ( 0x00000000u)

/** @brief Base address of UART1 peripheral registers */
#define CSL_UART_1_REGS               ( 0x00008000u)

/** @brief Base address of UART2 peripheral registers */
#define CSL_UART_2_REGS               ( 0x00008400u)

/** @brief Base address of UART3 peripheral registers */
#define CSL_UART_3_REGS               ( 0x0000CC00u)

/** @brief Base address of IRDA1 peripheral registers */
#define CSL_IRDA_1_REGS               ( CSL_UART_1_REGS)

/** @brief Base address of IRDA3 peripheral registers */
#define CSL_IRDA_3_REGS               ( CSL_UART_3_REGS)

/** @brief Base address of Stati Switch(SSW) peripheral registers */
#define CSL_SSW_1_REGS                ( 0x0000E400u)

/** @brief Base address of DMA peripheral registers */
#define CSL_DMA_1_REGS                    ( 0x00000C00u)

/** @brief Base address of MCBSP1 peripheral registers */
#define CSL_MCBSP_1_REGS              ( 0x00008C00u)

/** @brief Base address of MCBSP2 peripheral registers */
#define CSL_MCBSP_2_REGS              ( 0x00008800u)

/** @brief Base address of MCBSP3 peripheral registers */
#define CSL_MCBSP_3_REGS              ( 0x0000B800u)

/** @brief Base address of I2C1 peripheral registers */
#define CSL_I2C_1_REGS                ( 0x00009C00u)

/** @brief Base address of TIMER1 peripheral registers */
#define CSL_TIMER_1_REGS              ( 0x00002800u)

/** @brief Base address of TIMER2 peripheral registers */
#define CSL_TIMER_2_REGS              ( 0x00002C00u)

/** @brief Base address of TIMER3 peripheral registers */
#define CSL_TIMER_3_REGS              ( 0x00003000u)

/** @brief Base address of INTC level2.0 peripheral registers */
#define CSL_INTC20_0_REGS             ( 0x00004800u)

/** @brief Base address of INTC level2.1 peripheral registers */
#define CSL_INTC21_0_REGS             ( 0x00004C00u)

/** @brief Base address of CLKRST peripheral registers */
#define CSL_CLKRST_1_REGS              ( 0x00004000u)

/**************************************************************************\
* Look-up table for each peripheral
\**************************************************************************/
extern void * _CSL_uartlookup[CSL_UART_CNT];
extern void * _CSL_sswlookup[CSL_SSW_CNT]  ;
extern void * _CSL_irdalookup[CSL_IRDA_CNT];
extern void * _CSL_dmalookup[CSL_DMA_CNT]  ;
extern void * _CSL_mcbsplookup[CSL_MCBSP_CNT];
extern void * _CSL_i2clookup[CSL_I2C_CNT];
extern void * _CSL_timerlookup[CSL_TIMER_CNT] ;
extern void * _CSL_clkrstlookup[CSL_CLKRST_CNT] ;

/**************************************************************************\
* Peripheral Instance enumeration
\**************************************************************************/
/** @brief Peripheral Instance enumeration for UART */
typedef enum {
        /** Any Instance of UART */
    CSL_UART_PER_ANY    = -1,
        /** Instance 1 of UART */
    CSL_UART_1          =  0,
    /** Instance 2 of UART */
    CSL_UART_2          =  1,
    /** Instance 3 of UART */
        CSL_UART_3          =  2

} CSL_UartNum;

/** @brief Peripheral Instance enumeration for SSW */
typedef enum {
        /** Any Instance of SSW */
    CSL_SSW_PER_ANY    = -1,
    /** Instance of SSW */
    CSL_SSW                =  0
} CSL_SswNum;

/** @brief Peripheral Instance enumeration for DMA */
typedef enum {
        /** Any Instance of DMA */
    CSL_DMA_PER_ANY    = -1,
    /** Instance of DMA */
    CSL_DMA            =  0
} CSL_DmaNum;

/** @brief Peripheral Instance enumeration for IRDA */
typedef enum {
        /** Any Instance of IRDA */
    CSL_IRDA_PER_ANY    = -1,
    /** Instance 1 of IRDA (This is part of Instance 1 of UART). */
    CSL_IRDA_1          =  0,
    /** Instance 3 of IRDA (This is part of Instance 3 of UART).
    There is no Instance 2 of IRDA, as Instance 2 of UART doesn't support IRDA
    */
        CSL_IRDA_3          =  1
} CSL_IrdaNum;

/** @brief Peripheral Instance enumeration for DMA Channel */
typedef enum {
    /** Any Channel  */
    CSL_DMA_CHA_PER_ANY=  -1,
    /** Channel 0 */
    CSL_DMA_CHA_0      =   0,
    /** Channel 1 */
    CSL_DMA_CHA_1      =   1,
    /** Channel 2 */
    CSL_DMA_CHA_2      =   2,
    /** Channel 3 */
    CSL_DMA_CHA_3      =   3,
    /** Channel 4 */
    CSL_DMA_CHA_4      =   4,
    /** Channel 5 */
    CSL_DMA_CHA_5      =   5
} CSL_DmaChaNum;

/** @brief Peripheral Instance enumeration for MCBSP*/
typedef enum {
        /** Any Instance of McBSP */
    CSL_MCBSP_ANY                  =                 -1,
    /** Instance 1 of McBSP */
    CSL_MCBSP_1                    =                  0,
    /** Instance 2 of McBSP */
    CSL_MCBSP_2                    =                  1,
    /** Instance 3 of McBSP */
    CSL_MCBSP_3                    =                  2
} CSL_McbspNum;

/** @brief Peripheral Instance enumeration for I2C */
typedef enum {
        /** Any Instance of I2C */
    CSL_I2C_PER_ANY    =  -1,
        /** Instance 1 of I2C */
    CSL_I2C_1              =   0
} CSL_I2cNum;

/** @brief Peripheral Instance enumeration for TIMER */
typedef enum {
        /** Any Instance of TIMER */
    CSL_TIMER_PER_ANY    =  -1,
        /** Instance 1 of TIMER */
    CSL_TIMER_1                    =   0,
    /** Instance 2 of TIMER */
    CSL_TIMER_2          =   1,
        /** Instance 3 of TIMER */
    CSL_TIMER_3          =   2
} CSL_TimerNum;

/** @brief Peripheral Instance enumeration for CLKRST */
typedef enum {
        /** Any Instance of CLKRST */
    CSL_CLKRST_PER_ANY    =  -1,
        /** Instance of CLKRST */
    CSL_CLKRST            =   0
} CSL_ClkrstNum;

#endif  /* _5912_H_ */
