/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912.c ========
 *  Implementation of:
 *      OSK5912_init( )
 *
 *      OSK5912_releaseDsp( )
 *      OSK5912_resetDsp( )
 *
 *      OSK5912_wait( )
 *      OSK5912_waitmsec( )
 *      OSK5912_waitusec( )
 */

/* ------------------------------------------------------------------------ *
 *  Clocking Parameters after initialization                                *
 *      DPLL freq:  96 MHz                 192 MHz                          *
 *      Clock mode: Fully Sync             Scalable Sync                    *
 *      ARM freq:   96 MHz                 192 MHz                          *
 *      DSP freq:   96 MHz                 192 MHz                          *
 *      DSPMMU freq:96 MHz                 192 MHz                          *
 *      TC freq:    96 MHz                 96  MHz                          *
 *      LCD freq:   24 MHz                 24  MHz                          *
 *      PER freq:   24 MHz                 24  MHz                          *
 * ------------------------------------------------------------------------ */

#include <osk5912.h>
#include <osk5912_i2c.h>
#include <osk5912_power.h>

/* ------------------------------------------------------------------------ *
 *  Internal BSL Variables.                                                 *
 * ------------------------------------------------------------------------ */
/* DPLL freq, multiplier and divider */
static Uint16 osk5912_pll_freq = 0;

/* ------------------------------------------------------------------------ *
 *  Helper Function Prototypes                                              *
 * ------------------------------------------------------------------------ */
static void setDpll( Uint16 freq );
static void muxPins( );
static void setupEmif( );

/* ------------------------------------------------------------------------ *
 *  OSK5912_init( Uint32 freq  )                                            *
 *      Configure DPLL1.                                                    *
 *      Configure Pin Muxes                                                 *
 *      Configure System Clocks                                             *
 *      Configure EMIF                                                      *
 *      Release DSP                                                         *
 * ------------------------------------------------------------------------ */
void OSK5912_init( Uint16 freq )
{
    Int16 status;

    /* Set Voltage Control register */
    PMUX_VOLTAGE_CTRL_0 = 0x0FCD;

    /* Disable watchdog */
    WDT_TIMER_MODE = 0x00F5;
    WDT_TIMER_MODE = 0x00A0;

    /* Set DPLL to the desired frequency */
    setDpll( freq );

    /* Set pin mux for peripherals use */
    muxPins( );

    /* Enable ARM peripherals */
    CLK_ARM_RSTCT2 = CLK_ARM_RSTCT2_PER_EN;

    /* Set which components will receive a clock signal when idle */
    CLK_ARM_IDLECT1 = 0
          //| CLK_ARM_IDLECT1_IDL_CLKOUT_ARM
          //| CLK_ARM_IDLECT1_WKUP_MODE
          //| CLK_ARM_IDLECT1_IDLTIM_ARM
          //| CLK_ARM_IDLECT1_IDLAPI_ARM
          //| CLK_ARM_IDLECT1_IDLDPLL_ARM
          //| CLK_ARM_IDLECT1_IDLIF_ARM
          //| CLK_ARM_IDLECT1_IDLPER_ARM
          //| CLK_ARM_IDLECT1_IDLXORP_ARM
          //| CLK_ARM_IDLECT1_IDLWDT_ARM
            ;

    /* Set Access Width */
    /* Configure MPUI register
     * Turn on byte swapping in MPUI config register */
    MPUI_CTRL_REG = 0
            | ( 0 << 21 )         // Word swap control
            | ( 0 << 18 )         // Access priority
            | ( 3 << 16 )         // Byte swap control
            | ( 0xFF << 8 )       // Timeout
            | ( 2 << 4 )          // Access factor
            | ( 1 << 3 )          // MPUI port abort
            | ( 1 << 1 )          // Timeout enabled
            ;
    PRIB_TIPB_CNTL = 0x0000FF22;
    PUBB_TIPB_CNTL = 0x0000FF22;

    /* Configure EMIF settings */
    setupEmif( );

    /* Hold the DSP in reset */
    //OSK5912_resetDsp( );

    /* Release DSP from reset */
    OSK5912_releaseDsp( );

    /* Disable Helen2 Watchdog */
    WDT_WSPR = 0xAAAA;
    while ( WDT_WWPS & WDT_WWPS_W_PEND_WSPR );
    WDT_WSPR = 0x5555;
    while ( WDT_WWPS & WDT_WWPS_W_PEND_WSPR );

    do
    {
        status = 0;

        /* Initialize I2C */
        OSK5912_I2C_init( );

        /* Release ethernet from reset */
        /* Enable USB power ( +5V ) */
        status |= OSK5912_POWER_rset( 0x10, 0x44 );

        /* Set VCCSDRAM to 1.8V, AIC23 power to 3.3V */
        status |= OSK5912_POWER_rset( 0x0E, 0xB8 );
    }
    while ( status );

    /* Delay until the ethernet controller comes up from reset */
    OSK5912_waitusec( 200 );

    /* Enable SYSTEM_CLK_EN for the for GPIO modules */
    ULPD_CAM_CLK_CTRL |= 4;
}


/* ------------------------------------------------------------------------ *
 *  setDpll( Uint16 freq )                                                  *
 *      Setup and Enable DPLL1 ( Digital Phase Lock Loop 1 )                *
 *                                                                          *
 *  Assumption:                                                             *
 *      The 5912 can take 3 different input clock frequencies:              *
 *          12, 13, or 19.2 MHz.  Code is designed for 12 MHz clock.        *
 *                                                                          *
 *  Results:                                                                *
 *      DPLL is set to ( 96/196 MHz )                                       *
 *      ARM/DSP/TC clocks are divided from DPLL freq according to           *
 *          CLOCK_SELECT mode.                                              *
 * ------------------------------------------------------------------------ */
static void setDpll( Uint16 freq )
{
    Uint32 clocking_mode;
    Uint16 pll_mult;
    Uint16 pll_div;
    Uint8  tc_divisor;
    Uint8  lcd_divisor;
    Uint8  per_divisor;

    /* Disable clocks on ARM devices & peripherials - This ensures
     * no unkown clock states occurs when the DPLL is changed. */
    CLK_ARM_IDLECT2 = 0;

    /* Settings for 192 MHz freq */
    if ( freq == 192 )
    {
        /* Select the DPLL multiplier & divisor */
        osk5912_pll_freq = 192;
        pll_mult = 16;
        pll_div = 0;

        /* Select the clocking mode and divisors for other ARM componenets */
        clocking_mode = CLK_ARM_SYSST_CLOCK_SELECT_SYNC_SCAL;
        tc_divisor = 1;                     // Divide TC by 2, max freq = 96 MHz
        lcd_divisor = 3;                    // Divide LCD by 8
        per_divisor = 3;                    // Divide PER by 8
    }
    /* Settings for 96 MHz freq ( default speed ) */
    else
    {
        /* Select the DPLL multiplier & divisor */
        osk5912_pll_freq = 96;
        pll_mult = 8;
        pll_div = 0;

        /* Select the clocking mode and divisors for other ARM componenets */
        clocking_mode = CLK_ARM_SYSST_CLOCK_SELECT_FULL_SYNC;
        tc_divisor = 0;                     // Divide TC by 1, max freq = 96 MHz
        lcd_divisor = 2;                    // Divide LCD by 4
        per_divisor = 2;                    // Divide PER by 4
    }

    /* The DPLL must be locked before ramping up the freq.  This enusres
     * no glitches from the clocks derived from the DPLL. */
    DPLL1_CTL_REG = 0
            | DPLL1_CTL_REG_IOB             // Set IOB ( Initial on Break )
            | ( 1 << 7 )                    // Set PLL_MULT
            | ( 0 << 5 )                    // Set PLL_DIV
            | DPLL1_CTL_REG_PLL_ENABLE      // Enable DPLL
            | ( 0 << 2 )                    // Set BYPASS_DIV
            ;

    /* Poll for lock bit on DPLL */
    while ( ! ( DPLL1_CTL_REG & DPLL1_CTL_REG_LOCK ) );

    /* Set clocking mode */
    CLK_ARM_SYSST = clocking_mode;

    /* Set all clock divisor
     * Values must NOT violate the clocking mode selected eariler.
     * Values range from [n = 0..3] and correspond to divisors [ ( 1/2 )^n ] */
    CLK_ARM_CKCTL = 0
          //| CLK_ARM_CKCTL_ARM_INTHCK_SEL  // Set ARM_INTH_CK to half of ARM_CK
            | ( CLK_ARM_CKCTL & CLK_ARM_CKCTL_EN_DSPCK ) // Retain previous EN_DSPCK value
            | CLK_ARM_CKCTL_ARM_TIMXO       // Set ARM_TIMXO clk to be CK_GEN1
            | ( 0 << 10 )                   // DSPMMU clk divisor
            | ( tc_divisor << 8 )           //     TC clk divisor
            | ( 0 << 6 )                    //    DSP clk divisor
            | ( 0 << 4 )                    //    ARM clk divisor
            | ( lcd_divisor << 2 )          //    LCD clk divisor
            | ( per_divisor << 0 )          //    PER clk divisor
            ;

    /* With the clock divisors set, ramping up the DPLL will not generate
     * glitches from the dervived clocks. */
    DPLL1_CTL_REG = 0
            | DPLL1_CTL_REG_IOB             // Set IOB ( Initial on Break )
            | ( pll_mult << 7 )             // Set PLL_MULT
            | ( pll_div << 5 )              // Set PLL_DIV
            | DPLL1_CTL_REG_PLL_ENABLE      // Enable DPLL
            | ( 0 << 2 )                    // Set BYPASS_DIV
            | ( 3 << 0 )                    // Lock fields
            ;

    /* Loop for 1000+ cycles to ensure the DPLL is locked and the derivied
     * clocks are stable */
    OSK5912_wait( 1000 );

    /* Enable ARM device & peripherial clocks */
    CLK_ARM_IDLECT2 = 0
            | CLK_ARM_IDLECT2_EN_CKOUT_ARM  //
            | CLK_ARM_IDLECT2_DMACK_REQ     //
            | CLK_ARM_IDLECT2_EN_TIMCK      //
            | CLK_ARM_IDLECT2_EN_APICK      //
            | CLK_ARM_IDLECT2_EN_LCDCK      //
            | CLK_ARM_IDLECT2_EN_PERCK      //
            | CLK_ARM_IDLECT2_EN_XORPCK     //
            | CLK_ARM_IDLECT2_EN_WDTCK      //
            ;

    /* Enable traffic controller clocks */
    CLK_ARM_IDLECT3 = 0
          //| CLK_ARM_IDLECT3_IDLTC2_ARM    // T2_CK idle control
            | CLK_ARM_IDLECT3_EN_TC2_CK     // Enable TC2_CK
          //| CLK_ARM_IDLECT3_IDLTC1_ARM    // T1_CK idle control
            | CLK_ARM_IDLECT3_EN_TC1_CK     // Enable TC1_CK
          //| CLK_ARM_IDLECT3_IDLOCPI_ARM   // OCPI idle control
            | CLK_ARM_IDLECT3_EN_OCPI_CK    // Enable L3 OCPI clock
            ;
}


/* ------------------------------------------------------------------------ *
 *  muxPins( )                                                              *
 *      Setup pin mux for OSK5912:                                          *
 *      Follows the OSK 5912 schematic which the exception of:              *
 *      LCD pins replaced with GPIO/MPUIO/Z_STATE                           *
 *      KB  pins replaced with GPIO/MPUIO                                   *
 *      CAM pins replaced with GPIO/MPUIO                                   *
 * ------------------------------------------------------------------------ */
static void muxPins( )
{
    /* Disable previous pin multiplexing */
    PMUX_COMP_MODE_CTRL_0 = 0;

    /* Clear previous pin multiplexing to ensure the correct multiplexing
     * each time the program is running. */
    PMUX_FUNC_MUX_CTRL_0 = 0;
    PMUX_FUNC_MUX_CTRL_1 = 0;
    PMUX_FUNC_MUX_CTRL_2 = 0;
    PMUX_FUNC_MUX_CTRL_3 = 0;
    PMUX_FUNC_MUX_CTRL_4 = 0;
    PMUX_FUNC_MUX_CTRL_5 = 0;
    PMUX_FUNC_MUX_CTRL_6 = 0;
    PMUX_FUNC_MUX_CTRL_7 = 0;
    PMUX_FUNC_MUX_CTRL_8 = 0;
    PMUX_FUNC_MUX_CTRL_9 = 0;
    PMUX_FUNC_MUX_CTRL_A = 0;
    PMUX_FUNC_MUX_CTRL_B = 0;
    PMUX_FUNC_MUX_CTRL_C = 0;
    PMUX_FUNC_MUX_CTRL_D = 0;
    PMUX_FUNC_MUX_CTRL_E = 0;
    PMUX_FUNC_MUX_CTRL_F = 0;
    PMUX_FUNC_MUX_CTRL_10 = 0;
    PMUX_FUNC_MUX_CTRL_11 = 0;
    PMUX_FUNC_MUX_CTRL_12 = 0;
    PMUX_USB_TRANSCEIVER_CTRL = 0;
    PMUX_PULL_DWN_CTRL_0 = 0;
    PMUX_PULL_DWN_CTRL_1 = 0;
    PMUX_PULL_DWN_CTRL_2 = 0;
    PMUX_PULL_DWN_CTRL_3 = 0;
    PMUX_PULL_DWN_CTRL_4 = 0;
    PMUX_PU_PD_SEL_0 = 0;
    PMUX_PU_PD_SEL_1 = 0;
    PMUX_PU_PD_SEL_2 = 0;
    PMUX_PU_PD_SEL_3 = 0;
    PMUX_PU_PD_SEL_4 = 0;
    PMUX_MOD_CONF_CTRL_0 = 0;

    /* Multiplex pins */
    /* page #'s refer to the OMAP5912 Data Manual - SPS231 */

    // page 30 of OMAP5912 ZDY Data Manual
    // SDRAM ( all default to 0 )
    // page 31 of OMAP5912 ZDY Data Manual
    WriteField( PMUX_FUNC_MUX_CTRL_D, 2, MASK09 );  // F10 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_D, 1, MASK15 );  // A14 - Z_STATE
    // page 32 of OMAP5912 ZDY Data Manual
    WriteField( PMUX_FUNC_MUX_CTRL_D, 1, MASK18 );  // C12 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_D, 1, MASK21 );  // D12 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_D, 1, MASK24 );  // E11 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_D, 1, MASK27 );  // A13 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_E, 1, MASK00 );  // B14 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_E, 1, MASK03 );  // A15 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_E, 1, MASK06 );  // F11 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_E, 1, MASK09 );  // B15 - Z_STATE
    // page 33 of OMAP5912 ZDY Data Manual
    WriteField( PMUX_FUNC_MUX_CTRL_E, 1, MASK12 );  // C13 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_E, 1, MASK15 );  // D13 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_E, 1, MASK18 );  // A16 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_E, 1, MASK21 );  // C15 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_E, 1, MASK24 );  // E12 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_E, 1, MASK27 );  // D14 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_F, 1, MASK00 );  // C16 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_F, 1, MASK03 );  // B16 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_F, 1, MASK06 );  // A17 - Z_STATE
    WriteField( PMUX_FUNC_MUX_CTRL_D, 1, MASK12 );  // D15 - Z_STATE
    // page 34 of OMAP5912 ZDY Data Manual
    WriteField( PMUX_FUNC_MUX_CTRL_3, 7, MASK03 );  // D16 - GPIO27
    WriteField( PMUX_FUNC_MUX_CTRL_3, 7, MASK06 );  // E15 - GPIO63
    WriteField( PMUX_FUNC_MUX_CTRL_3, 7, MASK09 );  // B17 - GPIO61
    WriteField( PMUX_FUNC_MUX_CTRL_3, 1, MASK12 );  // C17 - MPUIO6
    WriteField( PMUX_FUNC_MUX_CTRL_3, 1, MASK15 );  // F14 - MPUIO0
    WriteField( PMUX_FUNC_MUX_CTRL_3, 1, MASK18 );  // F13 - MPUIO15
    WriteField( PMUX_FUNC_MUX_CTRL_3, 1, MASK21 );  // D17 - MPUIO13
    WriteField( PMUX_FUNC_MUX_CTRL_3, 1, MASK24 );  // E16 - MPUIO10
    WriteField( PMUX_FUNC_MUX_CTRL_3, 1, MASK27 );  // E17 - MPUIO9
    WriteField( PMUX_FUNC_MUX_CTRL_4, 1, MASK00 );  // F15 - MPUIO8
    WriteField( PMUX_FUNC_MUX_CTRL_3, 7, MASK00 );  // F17 - GPIO28
    // Default Pin Mux                              // G13 - MCBSP1.CLKS
    // Default Pin Mux                              // F16 - MCBSP1.CLKX
    // page 35 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // G15 - MCBSP1.FSX
    // Default Pin Mux                              // G14 - MCBSP1.DX
    // Default Pin Mux                              // G17 - MCBSP1.DR
    WriteField( PMUX_FUNC_MUX_CTRL_4, 7, MASK21 );  // H12 - GPIO57
    WriteField( PMUX_FUNC_MUX_CTRL_4, 7, MASK24 );  // G16 - GPIO39
    // Default Pin Mux                              // H14 - MPU_BOOT
    WriteField( PMUX_FUNC_MUX_CTRL_4, 7, MASK27 );  // H16 - GPIO35
    WriteField( PMUX_FUNC_MUX_CTRL_5, 7, MASK00 );  // H15 - GPIO34
    // page 36 of OMAP5912 ZDY Data Manual
    WriteField( PMUX_FUNC_MUX_CTRL_5, 7, MASK03 );  // H17 - GPIO33
    WriteField( PMUX_FUNC_MUX_CTRL_5, 7, MASK06 );  // J11 - GPIO32
    WriteField( PMUX_FUNC_MUX_CTRL_5, 7, MASK09 );  // H13 - GPIO31
    WriteField( PMUX_FUNC_MUX_CTRL_5, 7, MASK12 );  // J14 - GPIO30
    WriteField( PMUX_FUNC_MUX_CTRL_5, 7, MASK15 );  // J16 - GPIO29
    WriteField( PMUX_FUNC_MUX_CTRL_5, 2, MASK18 );  // J17 - MPUIO12
    WriteField( PMUX_FUNC_MUX_CTRL_5, 2, MASK21 );  // J13 - MPUIO14
    // page 37 of OMAP5912 ZDY Data Manual
    WriteField( PMUX_FUNC_MUX_CTRL_5, 7, MASK24 );  // J12 - GPIO38
    WriteField( PMUX_FUNC_MUX_CTRL_5, 7, MASK27 );  // K12 - GPIO37
    WriteField( PMUX_FUNC_MUX_CTRL_6, 1, MASK00 );  // K17 - UART3.TX
    // Default Pin Mux                              // K15 - UART3.RX
    // Default Pin Mux                              // K16 - GPIO15
    // Default Pin Mux                              // K14 - GPIO14
    // page 38 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // L17 - GPIO13
    // Default Pin Mux                              // L16 - GPIO12
    // Default Pin Mux                              // K13 - GPIO11
    // Default Pin Mux                              // L15 - GPIO7
    // Default Pin Mux                              // L14 - GPIO6
    // Default Pin Mux                              // M17 - GPIO4
    // page 39 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // L13 - GPIO3
    // Default Pin Mux                              // M16 - GPIO2
    // Default Pin Mux                              // M15 - GPIO1
    // Default Pin Mux                              // N17 - GPIO0 - LAN_INTR0
    WriteField( PMUX_FUNC_MUX_CTRL_7, 1, MASK12 );  // N16 - LOW_PWR
    // Default Pin Mux                              // M14 - MPUIO4
    // Default Pin Mux                              // L12 - MPUIO2
    // Default Pin Mux                              // N14 - !MPU_RST
    // page 40 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // N15 - MPUIO1
    // Default Pin Mux                              // P16 - I2C.SCL
    // Default Pin Mux                              // M11 - I2C.SDA
    // Default Pin Mux                              // P14 - UWIRE.SDI
    // Default Pin Mux                              // R15 - UWIRE.SDO
    // Default Pin Mux                              // P15 - UWIRE.SCLK
    WriteField( PMUX_FUNC_MUX_CTRL_8, 1, MASK09 );  // R17 - !UWIRE.CS0
    // page 41 of OMAP5912 ZDY Data Manual
    WriteField( PMUX_FUNC_MUX_CTRL_8, 1, MASK12 );  // R16 - !UWIRE.CS3
    // Default Pin Mux                              // T17 - BFAIL/EXT_FIQ
    // Default Pin Mux                              // N12 - !RST_OUT
    // page 42 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // N10 - MCSI1.SYNC
    // Default Pin Mux                              // U15 - MCSI1.CLK
    WriteField( PMUX_FUNC_MUX_CTRL_9, 1, MASK03 );  // U14 - MCBSP3.CLKX
    // Default Pin Mux                              // P12 - MCSI1.DIN
    // Default Pin Mux                              // R12 - BCLKREQ
    // Default Pin Mux                              // L10 - BCLK
    // page 43 of OMAP5912 ZDY Data Manual
    WriteField( PMUX_FUNC_MUX_CTRL_9, 1, MASK12 );  // R11 - UART1.RTS
    // Default Pin Mux                              // M9  - UART1.CTS
    // Default Pin Mux                              // U13 - UART1.RX
    WriteField( PMUX_FUNC_MUX_CTRL_9, 1, MASK21 );  // T12 - UART1.TX
    // Default Pin Mux                              // P11 - MCSI1.DOUT
    // Default Pin Mux                              // U12 - CLK32K_OUT
    // page 44 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // N9  - RTC_WAKE_INT
    // Default Pin Mux                              // P10 - RTC_ON_NOFF
    // Default Pin Mux                              // T11 - CLK32K_IN
    // Default Pin Mux                              // P9  - MMC.DAT3
    // Default Pin Mux                              // M8  - MMC.CLK
    // Default Pin Mux                              // R8  - MMC.DAT0
    WriteField( PMUX_FUNC_MUX_CTRL_A, 3, MASK18 );  // T9  - CFLASH.RFSFT
    // Default Pin Mux                              // U9  - MMC.DAT1
    // page 45 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // N7 - MMC.CMD
    // Default Pin Mux                              // P8 - !MCSI2.CLK2
    // Default Pin Mux                              // R7 - MCSI2.DIN
    // Default Pin Mux                              // T8 - MCSI2.DOUT
    // Default Pin Mux                              // U8 - MCSI2.SYNC
    // Default Pin Mux                              // T7 - MCLKREQ
    // Default Pin Mux                              // U7 - GPIO9
    // page 46 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // P7 - GPIO8
    // Default Pin Mux                              // R6 - MPUIO3
    // Default Pin Mux                              // L8 - MCBSP2.DR
    // Default Pin Mux                              // N6 - MCBSP2.FSX
    // Default Pin Mux                              // U6 - MCBSP2.CLKR
    // Default Pin Mux                              // U5 - MCBSP2.CLKX
    // Default Pin Mux                              // R5 - MCBSP2.FSR
    // Default Pin Mux                              // T5 - MCBSP2.DX
    // Default Pin Mux                              // U4 - UART2.RX
    // page 47 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // P6 - UART2.CTS
    WriteField( PMUX_FUNC_MUX_CTRL_C, 1, MASK24 );  // T4 - UART2.RTS
    WriteField( PMUX_FUNC_MUX_CTRL_C, 1, MASK27 );  // R4 - UART2.TX
    // Default Pin Mux                              // P5 - UART2.BCLK
    // Default Pin Mux                              // U3 - MCLK
    WriteField( PMUX_FUNC_MUX_CTRL_D, 7, MASK03 );  // P4 - GPIO58
    // Default Pin Mux                              // T2 - USB.DP
    // page 48 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // U1 - USB.DM
    // Default Pin Mux                              // T1 - !FLASH.CSIU
    // Default Pin Mux                              // N3 - !FLASH.RP
    // page 49 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // K1 - FLASH.CLK
    // Default Pin Mux                              // L6 - FLASH.RDY
    // Default Pin Mux                              // H6 - !FLASH.ADV
    // page 50 of OMAP5912 ZDY Data Manual
    // Default Pin Mux                              // J8 - !FLASH.CS2
    // Default Pin Mux                              // J5 - GPIO62
    // Default Pin Mux                              // J3 - !FLASH.CS1
    // Default Pin Mux                              // K3 - !FLASH.CS2U
    // Default Pin Mux                              // J1 - !FLASH.BE[0]
    // Default Pin Mux                              // K2 - !FLASH/BE[1]
    // Default Pin Mux                              // J6 - !FLASH.CS3

    /* Enable pin multiplexing */
    PMUX_COMP_MODE_CTRL_0 = 0x0000EAEF;
}


/* ------------------------------------------------------------------------ *
 *  setupEmif( )                                                            *
 *      Setup EMIFS & EMIFF registers                                       *
 *                                                                          *
 *  Notice:                                                                 *
 *      The settings are hardcoded to work with a Traffic Controller        *
 *      running at 96 MHz.                                                  *
 * ------------------------------------------------------------------------ */
static void setupEmif( )
{
/* ------------------------------------------------------------------------ *
 *  Configure EMIFS ( slow external memory: Flash )                         *
 * ------------------------------------------------------------------------ */
    TC_EMIFS_CONFIG = 0
          //| TC_EMIFS_CONFIG_PDE   // System power down ack disable
          //| TC_EMIFS_CONFIG_PWD_EN// Dynamic auto idle disable
            | ( TC_EMIFS_CONFIG & TC_EMIFS_CONFIG_BM ) // retain previous
          //| TC_EMIFS_CONFIG_WP    // Enable write protect
            ;

    TC_EMIFS_CCS0 = 0
          //| TC_EMIFS_CCS_PGWSTEN  // Page wait state Enable
            | ( 0 << 27 )           // Page wait state
            | ( 0 << 23 )           // Bus turn around wait state
          //| TC_EMIFS_CCS_MAD      // Multiplexed address
          //| TC_EMIFS_CCS_BW       // Bus Width [0: 16-bit][1: 32-bit]
            | ( 0 << 16 )           // Read mode [000b : Asynchronous Read]
            | ( 15 << 12 )          // Page/WE wait states
            | ( 15 << 8 )           // Write wait states
            | ( 15 << 4 )           // Read wait states
          //| TC_EMIFS_CCS_RT       // Data Retimed
            | ( 3 << 0 )            // Ref. Clk = TC_Clk/6
            ;

    TC_EMIFS_CCS1 = 0
          //| TC_EMIFS_CCS_PGWSTEN  // Page wait state Enable
            | ( 0 << 27 )           // Page wait state
            | ( 0 << 23 )           // Bus turn around wait state
          //| TC_EMIFS_CCS_MAD      // Multiplexed address
          //| TC_EMIFS_CCS_BW       // Bus Width [0: 16-bit][1: 32-bit]
            | ( 0 << 16 )           // Read mode [000b : Asynchronous Read]
            | ( 0 << 12 )           // Page/WE wait states
            | ( 10 << 8 )           // Write wait states
            | ( 10 << 4 )           // Read wait states
          //| TC_EMIFS_CCS_RT       // Data Retimed
            | ( 0 << 0 )            // Ref. Clk = TC_Clk/16
            ;

    TC_EMIFS_CCS2 = 0
          //| TC_EMIFS_CCS_PGWSTEN  // Page wait state Enable
            | ( 0 << 27 )           // Page wait state
            | ( 0 << 23 )           // Bus turn around wait state
          //| TC_EMIFS_CCS_MAD      // Multiplexed address
          //| TC_EMIFS_CCS_BW       // Bus Width [0: 16-bit][1: 32-bit]
            | ( 0 << 16 )           // Read mode [000b : Asynchronous Read]
            | ( 15 << 12 )          // Page/WE wait states
            | ( 15 << 8 )           // Write wait states
            | ( 15 << 4 )           // Read wait states
          //| TC_EMIFS_CCS_RT       // Data Retimed
            | ( 3 << 0 )            // Ref. Clk = TC_Clk/6
            ;
/* ------------------------------------------------------------------------ *
 *  Important Setting for Chip Select 3: External Flash Chip                *
 *      EMIFS clock = Traffic Controller Clock / 2                          *
 *      Wait States are configured to both the Micron & Intel Flash used    *
 * ------------------------------------------------------------------------ */
    TC_EMIFS_CCS3 = 0
          //| TC_EMIFS_CCS_PGWSTEN  // Page wait state Enable
            | ( 0 << 27 )           // Page wait state
            | ( 0 << 23 )           // Bus turn around wait state
          //| TC_EMIFS_CCS_MAD      // Multiplexed address
          //| TC_EMIFS_CCS_BW       // Bus Width [0: 16-bit][1: 32-bit]
            | ( 0 << 16 )           // Read mode [000b : Asynchronous Read]
            | ( 3 << 12 )           // Page/WE wait states
            | ( 3 << 8 )            // Write wait states
            | ( 6 << 4 )            // Read wait states
          //| TC_EMIFS_CCS_RT       // Data Retimed
            | ( 1 << 0 )            // Ref. Clk = TC_Clk/2
            ;

/* ------------------------------------------------------------------------ *
 *  Configure EMIFF ( fast external memory: SDRAM )                         *
 * ------------------------------------------------------------------------ */
    /* SDRAM type: mobile DDR SDRAM in High Power High Bandwidth mode */
    TC_EMIFF_OP = 0
            | ( 0 << 25 )           // Timeout for Bank 3 ( in TC cycles )
            | ( 0 << 18 )           // Timeout for Bank 2 ( in TC cycles )
            | ( 0 << 11 )           // Timeout for Bank 1 ( in TC cycles )
            | ( 0 << 4 )            // Timeout for Bank 0 ( in TC cycles )
            | TC_EMIFF_OP_MODE_HPHB // High Power High Bandwidth
            | TC_EMIFF_OP_SDRAM_TYPE_MOBILE_DDR // Mobile DDR SDRAM
            ;

    /* SDRAM Config */
    TC_EMIFF_CONFIG = 0
            | ( 0 << 28 )           // Don't use SDRAM larger than 256MB
          //| TC_EMIFF_CONFIG_CLK   // Enable Clock
          //| TC_EMIFF_CONFIG_PWD   // Power down disabled
            | TC_EMIFF_CONFIG_SDRAM_SDF0   // SDRAM freq
            | ( ( 5400 - 50 ) << 8 )// Autorefresh counter
                                    // = ( refresh per / clock per / # rows ) - 50
                                    //   NOTE: Since burstlen = 8, increase by factor 
                                    //         of 8 minus a 10% padding
                                    // = ( ( 8 * 64ms / 192MHz / 8192 ) * 90% ) - 50
                                    // = ( ( 8 * 750 ) * 90% ) - 50
                                    // = ( 5400 ) - 50
            | ( 0xF << 4 )          // SDRAM type: 32MB ( 256Mbit x 16 x 4 banks )
            | ( 3 << 2 )            // Auto refresh by burst of 8 commands
            | ( 1 << 1 )            // Reserved [ = 1 ]
          //| TC_EMIFF_CONFIG_SLRF  // Self refresh disabled
            ;

/* ------------------------------------------------------------------------ *
 *  Initialization procedures for SDRAM DDR                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
    TC_EMIFF_CMD = TC_EMIFF_CMD_NOP;

    /* Delay loop */
    OSK5912_wait( 200 );

    TC_EMIFF_CMD = TC_EMIFF_CMD_PRECHARGE;
    TC_EMIFF_CMD = TC_EMIFF_CMD_AUTOREFRESH;
    TC_EMIFF_CMD = TC_EMIFF_CMD_AUTOREFRESH;

    /* CAS idle 3, Page burst 8 */
    TC_EMIFF_MRS_NEW = 0
            | ( 3 << 4 )            // CAS idle time of 3
            | ( 0 << 3 )            // Serial
            | ( 3 << 0 )            // Page Burst Length = 8
            ;

    /* EMRS1 self refresh all banks */
    TC_EMIFF_EMRS1 = 0
            | ( 0 << 3 )            // 70 degree C max temp
            | ( 0 << 0 )            // Partial-array self-refresh all banks
            ;

    /* Enable DLL at 90 degress phase ( 25% clock period )*/
    TC_EMIFF_DLL_URD_CTRL = 0x06;
    TC_EMIFF_DLL_LRD_CTRL = 0x06;
    TC_EMIFF_DLL_WRD_CTRL = 0x06;

    /* Delay loop */
    OSK5912_wait( 1800 );
}


/* ------------------------------------------------------------------------ *
 *  OSK5912_releaseDsp( )                                                   *
 *      Releases the DSP from Reset.                                        *
 *      Disables the DSP WDT.                                               *
 *      Configures boot mode to branch to address [0x10000] after release.  *
 *      Inserts an idle loop at DSP address [0x10000]                       *
 * ------------------------------------------------------------------------ */
void OSK5912_releaseDsp( )
{
    /* Verify that the DSP is in not released */
    if ( ( CLK_ARM_RSTCT1 & CLK_ARM_RSTCT1_DSP_EN ) == CLK_ARM_RSTCT1_DSP_EN )
        return;

    /* Configure the DSP for priority registers programming,
     * but do not release the DSP yet. */
    CLK_ARM_RSTCT1 = CLK_ARM_RSTCT1_DSP_RST;

    /* Allow SARAM to be shared by DSP & ARM */
    MPUI_DSP_MPUI_CONFIG = 0;

    /* Enable MPUI clock */
    CLK_ARM_IDLECT2 |= CLK_ARM_IDLECT2_EN_APICK;

    /* Turn MPUI byte swap off & MPUI timeout on */
    MPUI_CTRL_REG = 0
            | ( 0 << 21 )         // Word swap control
            | ( 0 << 18 )         // Access priority
            | ( 0 << 16 )         // Byte swap control
            | ( 0xFF << 8 )       // Timeout
            | ( 2 << 4 )          // Access factor
            | ( 1 << 3 )          // MPUI port abort
            | ( 1 << 1 )          // Timeout enabled
            ;

    /* Insert an idle loop at DSP [0x10000], mapped to ARM [0xE0010000] */
    *( VUint16* )0xE0010000 = 0x2020;   // nop
    *( VUint16* )0xE0010002 = 0x4A7E;   // branch to self
    *( VUint16* )0xE0010004 = 0x2020;   // nop
    *( VUint16* )0xE0010006 = 0x2020;   // nop

    /*  Configure MPUI boot mode.
     *      0: Boot DSP address [0xFFFF00]
     *      1: Boot DSP address [0x080000]
     *      2: Boot DSP into idle state
     *      3: 16-bit download from [0x080000]
     *      4: 32-bit download from [0x080000]
     *      5: Boot DSP address [0x010000]
     *  Other: Boot DSP address [0x024000] */
    MPUI_DSP_BOOT_CONFIG = 0x00000005;

    /* Enable DSP peripheral clocks */
    CLK_DSP_IDLECT2 = 0
            | CLK_DSP_IDLECT2_EN_TIMCK
            | CLK_DSP_IDLECT2_EN_GPIOCK
            | CLK_DSP_IDLECT2_EN_XORPCK
            | CLK_DSP_IDLECT2_EN_WDTCK
            ;

    /* Enable DSP peripherals */
    CLK_DSP_RSTCT2 = CLK_DSP_RSTCT2_PER_EN | CLK_DSP_RSTCT2_WD_PER_EN;

    /* Enable DSP clock */
    CLK_ARM_CKCTL |= CLK_ARM_CKCTL_EN_DSPCK;

    /* Enable DSP peripheral clocks */
    CLK_DSP_IDLECT1 = 0
            | CLK_DSP_IDLECT1_IDLTIM_DSP
            | CLK_DSP_IDLECT1_IDLXORP_DSP
            | CLK_DSP_IDLECT1_IDLWDT_DSP
            ;

    /* Set DSP clock inputs */
    CLK_DSP_CKCTL = 0x0090;

    /* Release DSP from reset */
    CLK_ARM_RSTCT1 = CLK_ARM_RSTCT1_DSP_EN ;//| CLK_ARM_RSTCT1_DSP_RST;
}


/* ------------------------------------------------------------------------ *
 *  OSK5912_resetDsp( )                                                     *
 *      Reset DSP                                                           *
 *      Disables DSP clocks                                                 *
 * ------------------------------------------------------------------------ */
void OSK5912_resetDsp( )
{
    /* Turn off DSP clock after it is placed in reset */
    //CLK_ARM_CKCTL &= ~CLK_ARM_CKCTL_EN_DSPCK;

    /* Reset DSP but do not disable programming of priority registers */
    CLK_ARM_RSTCT1 &= ~CLK_ARM_RSTCT1_DSP_EN;
}


/* ------------------------------------------------------------------------ *
 *  Wait Delay Functions                                                    *
 *      Wait a set amount of time.  Function is tuned with the internal     *
 *      timer module.                                                       *
 * ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ *
 *  OSK5912_wait( Uint32 cycles )                                           *
 *    Wait a fixed amount of cycles                                         *
 *    NOTE: Code must be in internal SRAM memory to function properly       *
 * ------------------------------------------------------------------------ */
void OSK5912_wait( Uint32 cycles )
{
    /* Set type to volatile. */
    VUint32 counter;
    for ( counter = 0 ; counter < cycles ; counter++ )
    {
        /* Simple wait Loop - followed by nop instructions */
        asm( " nop" );
        asm( " nop" );
        asm( " nop" );
    }
}

/* ------------------------------------------------------------------------ *
 *  OSK5912_waitusec( Uint32 usec )                                         *
 *      Wait N microseconds ( based on the wait function above )            *
 * ------------------------------------------------------------------------ */
void OSK5912_waitusec( Uint32 usec )
{
    OSK5912_wait( ( osk5912_pll_freq * usec ) >> 6 );
}

/* ------------------------------------------------------------------------ *
 *  OSK5912_waitmsec( Uint32 msec )                                         *
 *      Wait N milliseconds ( based on the wait function above )            *
 * ------------------------------------------------------------------------ */
void OSK5912_waitmsec( Uint32 msec )
{
    OSK5912_waitusec( msec * 1000 );
}

/* ------------------------------------------------------------------------ *
 *  firstbit( Uint32 mask )                                                 *
 *      Determine where the first bit set in a given mask; Bit begins from  *
 *      0 to 31                                                             *
 * ------------------------------------------------------------------------ */
Int32 firstbit( Uint32 mask )
{
    Int32 shiftamt;
    Uint32 bit;

    /* Find offset of first bit in mask */
    for ( bit = 1, shiftamt = 0 ; shiftamt < 32 ; shiftamt++, bit <<= 1 )
        if ( bit & mask )
            break;

    /* Return location of first bit */
    return shiftamt;
}
