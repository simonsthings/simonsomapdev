/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== 5912.h ========
 *  Registers, Register Fields, and Memory Mapping for OSK5912
 */


/* ======================================================================== *
 *  Memory Mapping                                                          *
 * ======================================================================== */
/* ------------------------------------------------------------------------ *
 *  SDRAM Parameters                                                        *
 * ------------------------------------------------------------------------ */
#define OSK5912_SDRAM_BASE_ADDR     0x10000000
#define OSK5912_SDRAM_SIZE          0x02000000 // 64 Mbytes

/* ------------------------------------------------------------------------ *
 *  SRAM Parameters                                                         *
 * ------------------------------------------------------------------------ */
#define OSK5912_SRAM_BASE_ADDR      0x20000000
#define OSK5912_SRAM_SIZE           0x0003E800 // 250 Kbytes





/* ======================================================================== *
 *  Peripheral Registers                                                    *
 * ======================================================================== */
/* ------------------------------------------------------------------------ *
 *  Control Registers for: ARM CLOCK ( CLK_ )                               *
 * ------------------------------------------------------------------------ */
#define CLK_ARM_CKCTL               *( VUint32* )0xFFFECE00
#define CLK_ARM_IDLECT1             *( VUint32* )0xFFFECE04
#define CLK_ARM_IDLECT2             *( VUint32* )0xFFFECE08
#define CLK_ARM_EWUPCT              *( VUint32* )0xFFFECE0C
#define CLK_ARM_RSTCT1              *( VUint32* )0xFFFECE10
#define CLK_ARM_RSTCT2              *( VUint32* )0xFFFECE14
#define CLK_ARM_SYSST               *( VUint32* )0xFFFECE18
#define CLK_ARM_CKOUT1              *( VUint32* )0xFFFECE1C
#define CLK_ARM_CKOUT2              *( VUint32* )0xFFFECE20
#define CLK_ARM_IDLECT3             *( VUint32* )0xFFFECE24

/* ------------------------------------------------------------------------ *
 *  Register Parameters for ARM_CKCTL ( CLK_ARM_CKCTL_ )                    *
 * ------------------------------------------------------------------------ */
#define CLK_ARM_CKCTL_ARM_INTHCK_SEL        0x00004000
#define CLK_ARM_CKCTL_EN_DSPCK              0x00002000
#define CLK_ARM_CKCTL_ARM_TIMXO             0x00001000

/* ------------------------------------------------------------------------ *
 *  Register Parameters for ARM_IDLECT1 ( CLK_ARM_IDLECT1_ )                *
 * ------------------------------------------------------------------------ */
#define CLK_ARM_IDLECT1_IDL_CLKOUT_ARM      0x00001000
#define CLK_ARM_IDLECT1_WKUP_MODE           0x00000400
#define CLK_ARM_IDLECT1_IDLTIM_ARM          0x00000200
#define CLK_ARM_IDLECT1_IDLAPI_ARM          0x00000100
#define CLK_ARM_IDLECT1_IDLDPLL_ARM         0x00000080
#define CLK_ARM_IDLECT1_IDLIF_ARM           0x00000040
#define CLK_ARM_IDLECT1_IDLPER_ARM          0x00000004
#define CLK_ARM_IDLECT1_IDLXORP_ARM         0x00000002
#define CLK_ARM_IDLECT1_IDLWDT_ARM          0x00000001

/* ------------------------------------------------------------------------ *
 *  Register Parameters for ARM_IDLECT2 ( CLK_ARM_IDLECT2_ )                *
 *      Used for enabling Clks sent to various components & peripherals     *
 * ------------------------------------------------------------------------ */
#define CLK_ARM_IDLECT2_EN_CKOUT_ARM        0x00000800
#define CLK_ARM_IDLECT2_DMACK_REQ           0x00000100
#define CLK_ARM_IDLECT2_EN_TIMCK            0x00000080
#define CLK_ARM_IDLECT2_EN_APICK            0x00000040
#define CLK_ARM_IDLECT2_EN_LBCK             0x00000010
#define CLK_ARM_IDLECT2_EN_LCDCK            0x00000008
#define CLK_ARM_IDLECT2_EN_PERCK            0x00000004
#define CLK_ARM_IDLECT2_EN_XORPCK           0x00000002
#define CLK_ARM_IDLECT2_EN_WDTCK            0x00000001

/* ------------------------------------------------------------------------ *
 *  Register Parameters for ARM_RSTCT1 ( CLK_ARM_RSTCT1_ )                  *
 *      Used for Reseting the MPU/DSP                                       *
 * ------------------------------------------------------------------------ */
#define CLK_ARM_RSTCT1_SW_RST               0x00000008
#define CLK_ARM_RSTCT1_DSP_RST              0x00000004
#define CLK_ARM_RSTCT1_DSP_EN               0x00000002
#define CLK_ARM_RSTCT1_ARM_RST              0x00000001

/* ------------------------------------------------------------------------ *
 *  Register Parameters for ARM_RSTCT2 ( CLK_ARM_RSTCT2_ )                  *
 *      Used for Enabling Peripherals                                       *
 * ------------------------------------------------------------------------ */
#define CLK_ARM_RSTCT2_PER_EN               0x00000001

/* ------------------------------------------------------------------------ *
 *  Register Parameters for ARM_SYSST ( CLK_ARM_SYSST_ )                    *
 * ------------------------------------------------------------------------ */
#define CLK_ARM_SYSST_CLOCK_SELECT_FULL_SYNC    0x00000000
#define CLK_ARM_SYSST_CLOCK_SELECT_SYNC_SCAL    0x00001000
#define CLK_ARM_SYSST_CLOCK_SELECT_BYPASS_MODE  0x00002800
#define CLK_ARM_SYSST_CLOCK_SELECT_MIX_MODE_3   0x00003000
#define CLK_ARM_SYSST_CLOCK_SELECT_MIX_MODE_4   0x00003800
#define CLK_ARM_SYSST_IDLE_DSP              0x00000040
#define CLK_ARM_SYSST_POR                   0x00000020
#define CLK_ARM_SYSST_EXT_RST               0x00000010
#define CLK_ARM_SYSST_ARM_MCRST             0x00000008
#define CLK_ARM_SYSST_ARM_WDRST             0x00000004
#define CLK_ARM_SYSST_GLOB_SWRST            0x00000002
#define CLK_ARM_SYSST_DSP_WDRST             0x00000001

/* ------------------------------------------------------------------------ *
 *  Register Parameters for ARM_IDLECT3 ( CLK_ARM_IDLECT3_ )                *
 *      Used for enabling Clks sent to various components & peripherals     *
 * ------------------------------------------------------------------------ */
#define CLK_ARM_IDLECT3_IDLTC2_ARM          0x00000020
#define CLK_ARM_IDLECT3_EN_TC2_CK           0x00000010
#define CLK_ARM_IDLECT3_IDLTC1_ARM          0x00000008
#define CLK_ARM_IDLECT3_EN_TC1_CK           0x00000004
#define CLK_ARM_IDLECT3_IDLOCPI_ARM         0x00000002
#define CLK_ARM_IDLECT3_EN_OCPI_CK          0x00000001





/* ------------------------------------------------------------------------ *
 *  Control Registers for: DSP CLOCK ( CLK_ )                               *
 * ------------------------------------------------------------------------ */
#define CLK_DSP_CKCTL               *( VUint16* )0xE1008000
#define CLK_DSP_IDLECT1             *( VUint16* )0xE1008004
#define CLK_DSP_IDLECT2             *( VUint16* )0xE1008008
#define CLK_DSP_RSTCT2              *( VUint16* )0xE1008014
#define CLK_DSP_SYSST               *( VUint16* )0xE1008018

/* ------------------------------------------------------------------------ *
 *  Register Parameters for DSP_IDLECT1 ( CLK_DSP_IDLECT1_ )                *
 *      Idle Control on DSP                                                 *
 * ------------------------------------------------------------------------ */
#define CLK_DSP_IDLECT1_IDLTIM_DSP          0x0100
#define CLK_DSP_IDLECT1_IDLXORP_DSP         0x0002
#define CLK_DSP_IDLECT1_IDLWDT_DSP          0x0001

/* ------------------------------------------------------------------------ *
 *  Register Parameters for DSP_IDLECT2 ( CLK_DSP_IDLECT2_ )                *
 *      Idle Control on DSP                                                 *
 * ------------------------------------------------------------------------ */
#define CLK_DSP_IDLECT2_EN_TIMCK            0x0020
#define CLK_DSP_IDLECT2_EN_GPIOCK           0x0010
#define CLK_DSP_IDLECT2_EN_XORPCK           0x0002
#define CLK_DSP_IDLECT2_EN_WDTCK            0x0001

/* ------------------------------------------------------------------------ *
 *  Register Parameters for DSP_RSTCT2 ( CLK_DSP_RSTCT2_ )                  *
 *      Reset Control on DSP Peripherals                                    *
 * ------------------------------------------------------------------------ */
#define CLK_DSP_RSTCT2_WD_PER_EN             0x0002
#define CLK_DSP_RSTCT2_PER_EN                0x0001





/* ------------------------------------------------------------------------ *
 *  Control Register for: Digital Phase Lock Loop ( DPLL#_ )                *
 * ------------------------------------------------------------------------ */
#define DPLL1_CTL_REG               *( VUint32* )0xFFFECF00
#define DPLL2_CTL_REG               *( VUint32* )0xFFFED000

/* ------------------------------------------------------------------------ *
 *  Register Parameters for DPLL_CTL_REG ( DPLL_CTL_REG_ )                  *
 *      Reset Control on DSP Peripherals                                    *
 * ------------------------------------------------------------------------ */
#define DPLL1_CTL_REG_LS_DISABLE            0x8000
#define DPLL1_CTL_REG_IAI                   0x4000
#define DPLL1_CTL_REG_IOB                   0x2000
#define DPLL1_CTL_REG_TEST                  0x1000
#define DPLL1_CTL_REG_PLL_ENABLE            0x0010
#define DPLL1_CTL_REG_BREAKLN               0x0002
#define DPLL1_CTL_REG_LOCK                  0x0001





/* ------------------------------------------------------------------------ *
 *  Control Register for: ULPD power management ( ULPD_ )                   *
 * ------------------------------------------------------------------------ */
#define ULPD_COUNTER_32_LSB         *( VUint16* )0xFFFE0800
#define ULPD_COUNTER_32_MSB         *( VUint16* )0xFFFE0804
#define ULPD_COUNTER_HIGH_FREQ_LSB  *( VUint16* )0xFFFE0808
#define ULPD_COUNTER_HIGH_FREQ_MSB  *( VUint16* )0xFFFE080C
#define ULPD_GAUGING_CTRL           *( VUint16* )0xFFFE0810
#define ULPD_IT_STATUS              *( VUint16* )0xFFFE0814
#define ULPD_SETUP_ANALOG_CELL3_ULPD1   *( VUint16* )0xFFFE0824
#define ULPD_SETUP_ANALOG_CELL2_ULPD1   *( VUint16* )0xFFFE0828
#define ULPD_SETUP_ANALOG_CELL1_ULPD1   *( VUint16* )0xFFFE082C
#define ULPD_CLOCK_CTRL             *( VUint16* )0xFFFE0830
#define ULPD_SOFT_REQ               *( VUint16* )0xFFFE0834
#define ULPD_COUNTER_32_FIQ         *( VUint16* )0xFFFE0838
#define ULPD_STATUS_REQ             *( VUint16* )0xFFFE0840
#define ULPD_PLL_DIV                *( VUint16* )0xFFFE0844
#define ULPD_ULPD_PLL_CTRL_STATUS   *( VUint16* )0xFFFE084C
#define ULPD_POWER_CTRL             *( VUint16* )0xFFFE0850
#define ULPD_STATUS_REQ2            *( VUint16* )0xFFFE0854
#define ULPD_SLEEP_STATUS           *( VUint16* )0xFFFE0858
#define ULPD_SETUP_ANALOG_CELL4_ULPD1   *( VUint16* )0xFFFE085C
#define ULPD_SETUP_ANALOG_CELL5_ULPD1   *( VUint16* )0xFFFE0860
#define ULPD_SETUP_ANLOG_CELL6_ULPD1    *( VUint16* )0xFFFE0864
#define ULPD_SOFT_DISABLE_REQ       *( VUint16* )0xFFFE0868
#define ULPD_RESET_STATUS           *( VUint16* )0xFFFE086C
#define ULPD_REVISION_NUMBER        *( VUint16* )0xFFFE0870
#define ULPD_SDW_CLK_DIV_CTRL_SEL   *( VUint16* )0xFFFE0874
#define ULPD_COM_CLK_DIV_CTRL_SEL   *( VUint16* )0xFFFE0878
#define ULPD_CAM_CLK_CTRL           *( VUint16* )0xFFFE087C
#define ULPD_SOFT_REQ2              *( VUint16* )0xFFFE0880


/* ------------------------------------------------------------------------ *
 *  Control Registers for: OMAP1610 Configuration ( PMUX_ )                 *
 * ------------------------------------------------------------------------ */
#define PMUX_FUNC_MUX_CTRL_0        *( VUint32* )0xFFFE1000
#define PMUX_FUNC_MUX_CTRL_1        *( VUint32* )0xFFFE1004
#define PMUX_FUNC_MUX_CTRL_2        *( VUint32* )0xFFFE1008
#define PMUX_COMP_MODE_CTRL_0       *( VUint32* )0xFFFE100C
#define PMUX_FUNC_MUX_CTRL_3        *( VUint32* )0xFFFE1010
#define PMUX_FUNC_MUX_CTRL_4        *( VUint32* )0xFFFE1014
#define PMUX_FUNC_MUX_CTRL_5        *( VUint32* )0xFFFE1018
#define PMUX_FUNC_MUX_CTRL_6        *( VUint32* )0xFFFE101C
#define PMUX_FUNC_MUX_CTRL_7        *( VUint32* )0xFFFE1020
#define PMUX_FUNC_MUX_CTRL_8        *( VUint32* )0xFFFE1024
#define PMUX_FUNC_MUX_CTRL_9        *( VUint32* )0xFFFE1028
#define PMUX_FUNC_MUX_CTRL_A        *( VUint32* )0xFFFE102C
#define PMUX_FUNC_MUX_CTRL_B        *( VUint32* )0xFFFE1030
#define PMUX_FUNC_MUX_CTRL_C        *( VUint32* )0xFFFE1034
#define PMUX_FUNC_MUX_CTRL_D        *( VUint32* )0xFFFE1038
#define PMUX_PULL_DWN_CTRL_0        *( VUint32* )0xFFFE1040
#define PMUX_PULL_DWN_CTRL_1        *( VUint32* )0xFFFE1044
#define PMUX_PULL_DWN_CTRL_2        *( VUint32* )0xFFFE1048
#define PMUX_PULL_DWN_CTRL_3        *( VUint32* )0xFFFE104C
#define PMUX_GATE_INH_CTRL_0        *( VUint32* )0xFFFE1050
#define PMUX_CONF_REV               *( VUint32* )0xFFFE1058
#define PMUX_VOLTAGE_CTRL_0         *( VUint32* )0xFFFE1060
#define PMUX_USB_TRANSCEIVER_CTRL   *( VUint32* )0xFFFE1064
#define PMUX_LDO_PWRDN_CBTRK        *( VUint32* )0xFFFE1068
#define PMUX_TEST_DBG_CTRL_0        *( VUint32* )0xFFFE1070
#define PMUX_MOD_CONF_CTRL_0        *( VUint32* )0xFFFE1080
#define PMUX_FUNC_MUX_CTRL_E        *( VUint32* )0xFFFE1090
#define PMUX_FUNC_MUX_CTRL_F        *( VUint32* )0xFFFE1094
#define PMUX_FUNC_MUX_CTRL_10       *( VUint32* )0xFFFE1098
#define PMUX_FUNC_MUX_CTRL_11       *( VUint32* )0xFFFE109C
#define PMUX_FUNC_MUX_CTRL_12       *( VUint32* )0xFFFE10A0
#define PMUX_PULL_DWN_CTRL_4        *( VUint32* )0xFFFE10AC
#define PMUX_PU_PD_SEL_0            *( VUint32* )0xFFFE10B4
#define PMUX_PU_PD_SEL_1            *( VUint32* )0xFFFE10B8
#define PMUX_PU_PD_SEL_2            *( VUint32* )0xFFFE10BC
#define PMUX_PU_PD_SEL_3            *( VUint32* )0xFFFE10C0
#define PMUX_PU_PD_SEL_4            *( VUint32* )0xFFFE10C4
#define PMUX_FUNC_MUX_DSP_DMA_A     *( VUint32* )0xFFFE10D0
#define PMUX_FUNC_MUX_DSP_DMA_B     *( VUint32* )0xFFFE10D4
#define PMUX_FUNC_MUX_DSP_DMA_C     *( VUint32* )0xFFFE10D8
#define PMUX_FUNC_MUX_DSP_DMA_D     *( VUint32* )0xFFFE10DC
#define PMUX_FUNC_MUX_ARM_DMA_A     *( VUint32* )0xFFFE10EC
#define PMUX_FUNC_MUX_ARM_DMA_B     *( VUint32* )0xFFFE10F0
#define PMUX_FUNC_MUX_ARM_DMA_C     *( VUint32* )0xFFFE10F4
#define PMUX_FUNC_MUX_ARM_DMA_D     *( VUint32* )0xFFFE10F8
#define PMUX_FUNC_MUX_ARM_DMA_E     *( VUint32* )0xFFFE10FC
#define PMUX_FUNC_MUX_ARM_DMA_F     *( VUint32* )0xFFFE1100
#define PMUX_FUNC_MUX_ARM_DMA_G     *( VUint32* )0xFFFE1104
#define PMUX_MOD_CONF_CTRL_1        *( VUint32* )0xFFFE1110
#define PMUX_SECCTRL                *( VUint32* )0xFFFE1120
#define PMUX_CONF_STATUS            *( VUint32* )0xFFFE1130
#define PMUX_RESET_CONTROL          *( VUint32* )0xFFFE1140
#define PMUX_CONF_1611_CTRL         *( VUint32* )0xFFFE1150





/* ------------------------------------------------------------------------ *
 *  Control Registers for: Traffic Controller ( TC_ )                       *
 * ------------------------------------------------------------------------ */
/* OCP-T1/OCP-T2 Registers */
#define TC_OCPT1_PRIOR              *( VUint32* )0xFFFECC00
#define TC_OCPT1_PTOR1              *( VUint32* )0xFFFECCA0
#define TC_OCPT1_PTOR2              *( VUint32* )0xFFFECCA4
#define TC_OCPT1_PTOR3              *( VUint32* )0xFFFECCA8
#define TC_OCPT1_ATOR               *( VUint32* )0xFFFECCAC
#define TC_OCPT1_AADDR              *( VUint32* )0xFFFECCB0
#define TC_OCPT1_ATYPER             *( VUint32* )0xFFFECCB4
#define TC_OCPT_CONFIG_REG          *( VUint32* )0xFFFECCB8
#define TC_OCPT2_PRIOR              *( VUint32* )0xFFFECCD0
#define TC_OCPT2_PTOR1              *( VUint32* )0xFFFECCD4
#define TC_OCPT2_PTOR2              *( VUint32* )0xFFFECCD8
#define TC_OCPT2_PTOR3              *( VUint32* )0xFFFECCDC
#define TC_OCPT2_ATOR               *( VUint32* )0xFFFECCE0
#define TC_OCPT2_AADDR              *( VUint32* )0xFFFECCE4
#define TC_OCPT2_ATYPER             *( VUint32* )0xFFFECCE8

/* EMIFS Registers */
#define TC_EMIFS_PRIOR              *( VUint32* )0xFFFECC04
#define TC_EMIFS_CONFIG             *( VUint32* )0xFFFECC0C
#define TC_EMIFS_CCS0               *( VUint32* )0xFFFECC10
#define TC_EMIFS_CCS1               *( VUint32* )0xFFFECC14
#define TC_EMIFS_CCS2               *( VUint32* )0xFFFECC18
#define TC_EMIFS_CCS3               *( VUint32* )0xFFFECC1C
#define TC_EMIFS_PTOR1              *( VUint32* )0xFFFECC28
#define TC_EMIFS_PTOR2              *( VUint32* )0xFFFECC2C
#define TC_EMIFS_PTOR3              *( VUint32* )0xFFFECC30
#define TC_EMIFS_DWS                *( VUint32* )0xFFFECC40
#define TC_EMIFS_AADDR              *( VUint32* )0xFFFECC44
#define TC_EMIFS_ATYPER             *( VUint32* )0xFFFECC48
#define TC_EMIFS_ATOR               *( VUint32* )0xFFFECC4C
#define TC_EMIFS_ACS0               *( VUint32* )0xFFFECC50
#define TC_EMIFS_ACS1               *( VUint32* )0xFFFECC54
#define TC_EMIFS_ACS2               *( VUint32* )0xFFFECC58
#define TC_EMIFS_ACS3               *( VUint32* )0xFFFECC5C

/* EMIFF Registers */
#define TC_EMIFF_PRIOR              *( VUint32* )0xFFFECC08
#define TC_EMIFF_CONFIG             *( VUint32* )0xFFFECC20
#define TC_EMIFF_MRS                *( VUint32* )0xFFFECC24
#define TC_EMIFF_CONFIG2            *( VUint32* )0xFFFECC3C
#define TC_EMIFF_DLL_WRD_CTRL       *( VUint32* )0xFFFECC64
#define TC_EMIFF_DLL_WRD_STAT       *( VUint32* )0xFFFECC68
#define TC_EMIFF_MRS_NEW            *( VUint32* )0xFFFECC70
#define TC_EMIFF_EMRS0              *( VUint32* )0xFFFECC74
#define TC_EMIFF_EMRS1              *( VUint32* )0xFFFECC78
#define TC_EMIFF_OP                 *( VUint32* )0xFFFECC80
#define TC_EMIFF_CMD                *( VUint32* )0xFFFECC84
#define TC_EMIFF_PTOR1              *( VUint32* )0xFFFECC8C
#define TC_EMIFF_PTOR2              *( VUint32* )0xFFFECC90
#define TC_EMIFF_PTOR3              *( VUint32* )0xFFFECC94
#define TC_EMIFF_AADDR              *( VUint32* )0xFFFECC98
#define TC_EMIFF_ATYPER             *( VUint32* )0xFFFECC9C
#define TC_EMIFF_DLL_LRD_STAT       *( VUint32* )0xFFFECCBC
#define TC_EMIFF_DLL_URD_CTRL       *( VUint32* )0xFFFECCC0
#define TC_EMIFF_DLL_URD_STAT       *( VUint32* )0xFFFECCC4
#define TC_EMIFF_EMRS2              *( VUint32* )0xFFFECCC8
#define TC_EMIFF_DLL_LRD_CTRL       *( VUint32* )0xFFFECCCC

/* ------------------------------------------------------------------------ *
 *  Register Parameters for EMIFS_CCS ( TC_EMIFS_CCS_ )                     *
 * ------------------------------------------------------------------------ */
#define TC_EMIFS_CCS_PGWSTEN                0x80000000
#define TC_EMIFS_CCS_MAD                    0x00400000
#define TC_EMIFS_CCS_BW                     0x00100000
#define TC_EMIFS_CCS_RT                     0x00000004

/* ------------------------------------------------------------------------ *
 *  Register Parameters for EMIFS_CONFIG ( TC_EMIFS_CONFIG_ )               *
 * ------------------------------------------------------------------------ */
#define TC_EMIFS_CONFIG_FR                  0x00000010
#define TC_EMIFS_CONFIG_PDE                 0x00000008
#define TC_EMIFS_CONFIG_PWD_EN              0x00000004
#define TC_EMIFS_CONFIG_BM                  0x00000002
#define TC_EMIFS_CONFIG_WP                  0x00000001

/* ------------------------------------------------------------------------ *
 *  Register Parameters for EMIFF_CONFIG ( TC_EMIFF_CONFIG_ )               *
 * ------------------------------------------------------------------------ */
#define TC_EMIFF_CONFIG_LG_SDRAM_0          0x00000000
#define TC_EMIFF_CONFIG_LG_SDRAM_1          0x10000000
#define TC_EMIFF_CONFIG_LG_SDRAM_2          0x20000000
#define TC_EMIFF_CONFIG_LG_SDRAM_3          0x30000000
#define TC_EMIFF_CONFIG_CLK                 0x08000000
#define TC_EMIFF_CONFIG_PWD                 0x04000000
#define TC_EMIFF_CONFIG_SDRAM_SDF0          0x00000000
#define TC_EMIFF_CONFIG_SDRAM_SDF1          0x01000000
#define TC_EMIFF_CONFIG_SDRAM_SDF2          0x02000000
#define TC_EMIFF_CONFIG_SDRAM_SDF3          0x03000000
#define TC_EMIFF_CONFIG_SLRF                0x00000001

/* ------------------------------------------------------------------------ *
 *  Register Parameters for EMIFF_OP ( TC_EMIFF_OP_ )                       *
 * ------------------------------------------------------------------------ */
#define TC_EMIFF_OP_MODE_LPLB               0x00000000
#define TC_EMIFF_OP_MODE_HPHB               0x00000004
#define TC_EMIFF_OP_MODE_POM0               0x00000008
#define TC_EMIFF_OP_SDRAM_TYPE_REG_SDR      0x00000000
#define TC_EMIFF_OP_SDRAM_TYPE_REG_DDR      0x00000001
#define TC_EMIFF_OP_SDRAM_TYPE_LOW_POW_SDR  0x00000002
#define TC_EMIFF_OP_SDRAM_TYPE_MOBILE_DDR   0x00000003

/* ------------------------------------------------------------------------ *
 *  Register Parameters for EMIFF_CMD ( TC_EMIFF_CMD_ )                     *
 * ------------------------------------------------------------------------ */
#define TC_EMIFF_CMD_NOP                    0x00000000
#define TC_EMIFF_CMD_PRECHARGE              0x00000001
#define TC_EMIFF_CMD_AUTOREFRESH            0x00000002
#define TC_EMIFF_CMD_ENTER_DEEP_SLEEP       0x00000003
#define TC_EMIFF_CMD_EXIT_DEEP_SLEEP        0x00000004
#define TC_EMIFF_CMD_SET_CKE_HIGH           0x00000007
#define TC_EMIFF_CMD_SET_CKE_LOW            0x00000008





/* ------------------------------------------------------------------------ *
 *  Control Registers for: MPU Interface ( MPUI_ )                          *
 * ------------------------------------------------------------------------ */
#define MPUI_CTRL_REG               *( VUint32* )0xFFFEC900
#define MPUI_DEBUG_ADDR             *( VUint32* )0xFFFEC904
#define MPUI_DEBUG_DATA             *( VUint32* )0xFFFEC908
#define MPUI_DEBUG_FLAG             *( VUint32* )0xFFFEC90C
#define MPUI_STATUS_REG             *( VUint32* )0xFFFEC910
#define MPUI_DSP_STATUS_REG         *( VUint32* )0xFFFEC914
#define MPUI_DSP_BOOT_CONFIG        *( VUint32* )0xFFFEC918
#define MPUI_DSP_MPUI_CONFIG        *( VUint32* )0xFFFEC91C





/* ------------------------------------------------------------------------ *
 *  Control Registers for: Arm Watchdog Timer ( WDT_ )                      *
 * ------------------------------------------------------------------------ */
#define WDT_WIDR                    *( VUint32* )0xFFFEB000
#define WDT_WD_SYSCONFIG            *( VUint32* )0xFFFEB010
#define WDT_WD_SYSSTATUS            *( VUint32* )0xFFFEB014
#define WDT_WCLR                    *( VUint32* )0xFFFEB024
#define WDT_WCRR                    *( VUint32* )0xFFFEB028
#define WDT_WLDR                    *( VUint32* )0xFFFEB02C
#define WDT_WTGR                    *( VUint32* )0xFFFEB030
#define WDT_WWPS                    *( VUint32* )0xFFFEB034
#define WDT_WSPR                    *( VUint32* )0xFFFEB048

/* ------------------------------------------------------------------------ *
 *  Register Parameters for WDT_WWPS ( WDT_WWPS_ )                          *
 * ------------------------------------------------------------------------ */
#define WDT_WWPS_W_PEND_WSPR                0x00000010
#define WDT_WWPS_W_PEND_WTGR                0x00000008
#define WDT_WWPS_W_PEND_WLDR                0x00000004
#define WDT_WWPS_W_PEND_WCRR                0x00000002
#define WDT_WWPS_W_PEND_WCLR                0x00000001

/* ------------------------------------------------------------------------ *
 *  Control Registers for: Arm Watchdog Timer ( WDT_ )                      *
 * ------------------------------------------------------------------------ */
#define WDT_CNTL_TIMER              *( VUint16* )0xFFFEC800
#define WDT_LOAD_TIM                *( VUint16* )0xFFFEC804
#define WDT_READ_TIM                *( VUint16* )0xFFFEC804
#define WDT_TIMER_MODE              *( VUint16* )0xFFFEC808

/* ------------------------------------------------------------------------ *
 *  Control Registers for: DSP Watchdog Timer ( DSP_WDT_ )                  *
 * ------------------------------------------------------------------------ */
#define DSP_WDT_CNTL_TIMER          *( VUint16* )0xE1006800
#define DSP_WDT_LOAD_TIM            *( VUint16* )0xE1006804
#define DSP_WDT_READ_TIM            *( VUint16* )0xE1006804
#define DSP_WDT_TIMER_MODE          *( VUint16* )0xE1006808





/* ------------------------------------------------------------------------ *
 *  Control Registers for: Arm 32kHz Timer ( TIM32K_ )                      *
 * ------------------------------------------------------------------------ */
#define TIM32K_CR                   *( VUint32* )0xFFFB9008
#define TIM32K_TVR                  *( VUint32* )0xFFFB9000
#define TIM32K_TCR                  *( VUint32* )0xFFFB9004

/* ------------------------------------------------------------------------ *
 *  Control Registers for: Arm Timer1 ( TIMER1_ )                           *
 * ------------------------------------------------------------------------ */
#define TIMER1_CNTL_TIMER           *( VUint32* )0xFFFEC500
#define TIMER1_LOAD_TIM             *( VUint32* )0xFFFEC504
#define TIMER1_READ_TIM             *( VUint32* )0xFFFEC508

/* ------------------------------------------------------------------------ *
 *  Control Registers for: Arm Timer2 ( TIMER2_ )                           *
 * ------------------------------------------------------------------------ */
#define TIMER2_CNTL_TIMER           *( VUint32* )0xFFFEC600
#define TIMER2_LOAD_TIM             *( VUint32* )0xFFFEC604
#define TIMER2_READ_TIM             *( VUint32* )0xFFFEC608

/* ------------------------------------------------------------------------ *
 *  Control Registers for: Arm Timer3 ( TIMER3_ )                           *
 * ------------------------------------------------------------------------ */
#define TIMER3_CNTL_TIMER           *( VUint32* )0xFFFEC700
#define TIMER3_LOAD_TIM             *( VUint32* )0xFFFEC704
#define TIMER3_READ_TIM             *( VUint32* )0xFFFEC708





/* ------------------------------------------------------------------------ *
 *  Control Registers for: ARM Public TIPB Registers ( PUBB_ )              *
 * ------------------------------------------------------------------------ */
#define PUBB_TIPB_CNTL              *( VUint16* )0xFFFED300
#define PUBB_TIPB_BUS_ALLOC         *( VUint16* )0xFFFED304
#define PUBB_MPU_TIPB_CNTL          *( VUint16* )0xFFFED308
#define PUBB_ENHANCED_TIPB_CNTL     *( VUint16* )0xFFFED30C
#define PUBB_ADDRESS_DBG            *( VUint16* )0xFFFED310
#define PUBB_DATA_DEBUG_LOW         *( VUint16* )0xFFFED314
#define PUBB_DATA_DEBUG_HIGH        *( VUint16* )0xFFFED318
#define PUBB_DEBUG_CNTR_SIG         *( VUint16* )0xFFFED31C

/* ------------------------------------------------------------------------ *
 *  Control Registers for: ARM Private TIPB Registers ( PRIB_ )             *
 * ------------------------------------------------------------------------ */
#define PRIB_TIPB_CNTL              *( VUint16* )0xFFFECA00
#define PRIB_TIPB_BUS_ALLOC         *( VUint16* )0xFFFECA04
#define PRIB_MPU_TIPB_CNTL          *( VUint16* )0xFFFECA08
#define PRIB_ENHANCED_TIPB_CNTL     *( VUint16* )0xFFFECA0C
#define PRIB_ADDRESS_DBG            *( VUint16* )0xFFFECA10
#define PRIB_DATA_DEBUG_LOW         *( VUint16* )0xFFFECA14
#define PRIB_DATA_DEBUG_HIGH        *( VUint16* )0xFFFECA18
#define PRIB_DEBUG_CNTR_SIG         *( VUint16* )0xFFFECA1C





/* ------------------------------------------------------------------------ *
 *  Control Registers for: UART1 TIPB Switch Registers ( UART1_ )           *
 * ------------------------------------------------------------------------ */
#define UART1_BASE_ADDR             ( VUint32* )0xFFFB0000

/* ------------------------------------------------------------------------ *
 *  Control Registers for: UART2 TIPB Switch Registers ( UART2_ )           *
 * ------------------------------------------------------------------------ */
#define UART2_BASE_ADDR             ( VUint32* )0xFFFB0800

/* ------------------------------------------------------------------------ *
 *  Control Registers for: UART3 TIPB Switch Registers ( UART3_ )           *
 * ------------------------------------------------------------------------ */
#define UART3_BASE_ADDR             ( VUint32* )0xFFFB9800

/* ------------------------------------------------------------------------ *
 *  Control Registers for: UART Registers minus Base Address ( UART_ )      *
 *  These Registers are available with "LCR[bit 7] = 0"                     *
 * ------------------------------------------------------------------------ */
#define UART_RHR                    0x00 // Read only
#define UART_THR                    0x00 // Write only
#define UART_IER                    0x04 // To Access: EFR[4]=1
#define UART_IIR                    0x08 // Read only
#define UART_FCR                    0x08 // Write only - To Access: EFR[4]=1
#define UART_LCR                    0x0C
#define UART_MCR                    0x10 // To Access: EFR[4]=1
#define UART_LSR                    0x14 // Read only
#define UART_MSR                    0x18 // Read only
#define UART_TCR                    0x18 // To Access: EFR[4]=1 & MCR[6]=1
#define UART_TLR                    0x1C // To Access: EFR[4]=1 & MCR[6]=1
#define UART_SPR                    0x1C
#define UART_MDR1                   0x20
#define UART_MDR2                   0x24
#define UART_SFLSR                  0x28 // Read only
#define UART_TXFLL                  0x28 // Write only
#define UART_RESUME                 0x2C // Read only
#define UART_TXFLH                  0x2C // Write only
#define UART_SFREGL                 0x30 // Read only
#define UART_RXFLL                  0x30 // Write only
#define UART_SFREGH                 0x34 // Read only
#define UART_RXFLH                  0x34 // Write only
#define UART_BLR                    0x38
#define UART_ACREG                  0x3C
#define UART_SCR                    0x40
#define UART_SSR                    0x44 // Read only
#define UART_EBLR                   0x48
#define UART_MVR                    0x50 // Read only
#define UART_SYSC                   0x54
#define UART_SYSS                   0x58
#define UART_WER                    0x5C

/* ------------------------------------------------------------------------ *
 *  Control Registers for: UART Registers minus Base Address ( UART_ )      *
 *  These Registers are available with "LCR[bit 7] = 1"                     *
 * ------------------------------------------------------------------------ */
#define UART_MODE_LCR_80            0x1000
#define UART_DLL                    UART_MODE_LCR_80 + 0x00
#define UART_DLH                    UART_MODE_LCR_80 + 0x04
#define UART_UASR                   UART_MODE_LCR_80 + 0x38

/* ------------------------------------------------------------------------ *
 *  Control Registers for: UART Registers minus Base Address ( UART_ )      *
 * These Registers are available with "LCR = 0xBF"                          *
 * ------------------------------------------------------------------------ */
#define UART_MODE_LCR_BF            0x2000
#define UART_EFR                    UART_MODE_LCR_BF + 0x08
#define UART_XON1                   UART_MODE_LCR_BF + 0x10
#define UART_XON2                   UART_MODE_LCR_BF + 0x14
#define UART_XOFF1                  UART_MODE_LCR_BF + 0x18
#define UART_XOFF2                  UART_MODE_LCR_BF + 0x1C

/* ------------------------------------------------------------------------ *
 *  Register Parameters for UART_MDR1 ( UART_MDR1_ )                        *
 * ------------------------------------------------------------------------ */
#define UART_MDR1_FRAME_END_MODE            0x80
#define UART_MDR1_SIP_MODE                  0x40
#define UART_MDR1_SCT                       0x20
#define UART_MDR1_SET_TXIR                  0x10
#define UART_MDR1_IR_SLEEP                  0x08

/* ------------------------------------------------------------------------ *
 *  Register Parameters for UART_MDR1_MODE_SELECT ( UART_MDR1_MODE_SELECT_ )*
 * ------------------------------------------------------------------------ */
#define UART_MDR1_MODE_SELECT_16X_MODE      0
#define UART_MDR1_MODE_SELECT_SIR_MODE      1
#define UART_MDR1_MODE_SELECT_16X_AUTO      2
#define UART_MDR1_MODE_SELECT_13X_MODE      3
#define UART_MDR1_MODE_SELECT_MIR_MODE      4
#define UART_MDR1_MODE_SELECT_FIR_MODE      5
#define UART_MDR1_MODE_SELECT_DISABLE       7

/* ------------------------------------------------------------------------ *
 *  Register Parameters for UART_LSR ( UART_LSR_ )                          *
 * ------------------------------------------------------------------------ */
#define UART_LSR_RX_FIFO_STS                0x80
#define UART_LSR_TX_SR_E                    0x40
#define UART_LSR_TX_FIFO_E                  0x20
#define UART_LSR_RX_BI                      0x10
#define UART_LSR_RX_FE                      0x08
#define UART_LSR_RX_PE                      0x04
#define UART_LSR_RX_OE                      0x02
#define UART_LSR_RX_FIFO_E                  0x01

/* ------------------------------------------------------------------------ *
 *  Register Parameters for UART_SYSC ( UART_SYSC_ )                        *
 * ------------------------------------------------------------------------ */
#define UART_SYSC_IDLEMODE_SMART_IDLE       0x10
#define UART_SYSC_IDLEMODE_NO_IDLE          0x08
#define UART_SYSC_IDLEMODE_FORCE_IDLE       0x00
#define UART_SYSC_ENAWAKEUP                 0x04
#define UART_SYSC_SOFTRESET                 0x02
#define UART_SYSC_AUTOIDLE                  0x01

/* ------------------------------------------------------------------------ *
 *  Register Parameters for UART_SYSS ( UART_SYSS_ )                        *
 * ------------------------------------------------------------------------ */
#define UART_SYSS_RESETDONE                 0x01





/* ------------------------------------------------------------------------ *
 *  Control Registers for: Interrupt Handler Level 1 ( INT1_ )              *
 * ------------------------------------------------------------------------ */
#define INT1_ITR                    *( VUint32* ) 0xFFFECB00
#define INT1_MIR                    *( VUint32* ) 0xFFFECB04
#define INT1_SIR_IRQ_CODE           *( VUint8* )  0xFFFECB10
#define INT1_SIR_FIQ_CODE           *( VUint8* )  0xFFFECB14
#define INT1_CONTROL                *( VUint8* )  0xFFFECB18
#define INT1_SISR                   *( VUint32* ) 0xFFFECB9C
#define INT1_GMR                    *( VUint32* ) 0xFFFECBA0
#define INT1_ILR_BASE               ( VUint32* )  0xFFFECB1C // Priority Regs Base
#define INT1_SIZE                   32

/* ------------------------------------------------------------------------ *
 *  Control Registers for: Interrupt Handler Level 2 ( INT2_ )              *
 * ------------------------------------------------------------------------ */
#define INT2_ITR                    0x00
#define INT2_MIR                    0x04
#define INT2_SIR_IRQ_CODE           *( Uint32* )0xFFFE0010
#define INT2_SIR_FIQ_CODE           *( Uint32* )0xFFFE0014
#define INT2_CONTROL                *( Uint32* )0xFFFE0018
#define INT2_SISR                   0x9C
#define INT2_STATUS                 *( Uint32* )0xFFFE00A0
#define INT2_OCP_CFG                *( Uint32* )0xFFFE00A4
#define INT2_INTH_REV               *( Uint32* )0xFFFE00A8
#define INT2_ILR_BASE               0x1C // Priority Regs Base
#define INT2_SIZE                   32
#define INT2_BANKS                  4 // 4: 128 INTR




/* ------------------------------------------------------------------------ *
 *  Control Registers for: Inter-Integrated Circuit ( I2C_ )                *
 * ------------------------------------------------------------------------ */
#define I2C_REV                     *( VUint16* )0xFFFB3800
#define I2C_IE                      *( VUint16* )0xFFFB3804
#define I2C_STAT                    *( VUint16* )0xFFFB3808
#define I2C_SYSS                    *( VUint16* )0xFFFB3810
#define I2C_BUF                     *( VUint16* )0xFFFB3814
#define I2C_CNT                     *( VUint16* )0xFFFB3818
#define I2C_DATA                    *( VUint16* )0xFFFB381C
#define I2C_SYSC                    *( VUint16* )0xFFFB3820
#define I2C_CON                     *( VUint16* )0xFFFB3824
#define I2C_OA                      *( VUint16* )0xFFFB3828
#define I2C_SA                      *( VUint16* )0xFFFB382C
#define I2C_PSC                     *( VUint16* )0xFFFB3830
#define I2C_SCLL                    *( VUint16* )0xFFFB3834
#define I2C_SCLH                    *( VUint16* )0xFFFB3838
#define I2C_SYSTEST                 *( VUint16* )0xFFFB383C

/* ------------------------------------------------------------------------ *
 *  Register Parameters for I2C_REV ( I2C_REV_ )                            *
 * ------------------------------------------------------------------------ */
#define I2C_REV_MASK                        0x00FF // Bit Mask

/* ------------------------------------------------------------------------ *
 *  Register Parameters for I2C_IE ( I2C_IE_ )                              *
 * ------------------------------------------------------------------------ */
#define I2C_IE_GC_IE                        0x0020 // General Call
#define I2C_IE_XRDY_IE                      0x0010 // Trasmit Data Ready
#define I2C_IE_RRDY_IE                      0x0008 // Receive Data Ready
#define I2C_IE_ARDY_IE                      0x0004 // Register Access Ready
#define I2C_IE_NACK_IE                      0x0002 // No Ack
#define I2C_IE_AL_IE                        0x0001 // Arbitration Lost

/* ------------------------------------------------------------------------ *
 *  Register Parameters for I2C_STAT ( I2C_STAT_ )                          *
 * ------------------------------------------------------------------------ */
#define I2C_STAT_SBD                        0x8000 // Single byte data
#define I2C_STAT_BB                         0x1000 // Bus busy
#define I2C_STAT_ROVR                       0x0800 // Receive overrun
#define I2C_STAT_XUDF                       0x0400 // Transmit underflow
#define I2C_STAT_AAS                        0x0200 // Address as slave
#define I2C_STAT_GC                         0x0020 // General call
#define I2C_STAT_XRDY                       0x0010 // Transmit data ready
#define I2C_STAT_RRDY                       0x0008 // Receive data ready
#define I2C_STAT_ARDY                       0x0004 // Register access ready
#define I2C_STAT_NACK                       0x0002 // No acknowledgment
#define I2C_STAT_AL                         0x0001 // Arbitration lost

/* ------------------------------------------------------------------------ *
 *  Register Parameters for I2C_SYSS ( I2C_SYSS_ )                          *
 * ------------------------------------------------------------------------ */
#define I2C_SYSS_RDONE                      0x0001 // Reset Done

/* ------------------------------------------------------------------------ *
 *  Register Parameters for I2C_BUF ( I2C_BUF_ )                            *
 * ------------------------------------------------------------------------ */
#define I2C_BUF_RDMA_EN                     0x8000 // Receive DMA enable
#define I2C_BUF_XDMA_EN                     0x0080 // Transmit DMA enable

/* ------------------------------------------------------------------------ *
 *  Register Parameters for I2C_SYSC ( I2C_SYSC_ )                          *
 * ------------------------------------------------------------------------ */
#define I2C_SYSC_SRST                       0x0002 // Soft Reset

/* ------------------------------------------------------------------------ *
 *  Register Parameters for I2C_CON ( I2C_CON_ )                            *
 * ------------------------------------------------------------------------ */
#define I2C_CON_I2C_EN                      0x8000 // I2C module enable
#define I2C_CON_BE                          0x4000 // Big endian mode
#define I2C_CON_STB                         0x0800 // Start byte mode
#define I2C_CON_MST                         0x0400 // Master/slave mode
#define I2C_CON_TRX                         0x0200 // TX/RX mode
#define I2C_CON_TX                          0x0200 // TX mode
#define I2C_CON_RX                          0x0000 // RX mode
#define I2C_CON_XA                          0x0100 // Expand address
#define I2C_CON_STP                         0x0002 // Stop condition
#define I2C_CON_STT                         0x0001 // Start condition

/* ------------------------------------------------------------------------ *
 *  Register Parameters for I2C_OA ( I2C_OA_ )                              *
 * ------------------------------------------------------------------------ */
#define I2C_OA_MASK                         0x03FF // Bit Mask

/* ------------------------------------------------------------------------ *
 *  Register Parameters for I2C_SA ( I2C_SA_ )                              *
 * ------------------------------------------------------------------------ */
#define I2C_SA_MASK                         0x03FF // Bit Mask





/* ------------------------------------------------------------------------ *
 *  Control Registers for: USB Function Module ( USB_ )                     *
 * ------------------------------------------------------------------------ */
#define USB_REV                     *( VUint16* )0xFFFB4000
#define USB_EP_NUM                  *( VUint16* )0xFFFB4004
#define USB_DATA_WORD               *( VUint16* )0xFFFB4008
#define USB_DATA_BYTE               *( VUint8* ) 0xFFFB4008
#define USB_CTRL                    *( VUint16* )0xFFFB400C
#define USB_STAT_FLG                *( VUint16* )0xFFFB4010
#define USB_RXFSTAT                 *( VUint16* )0xFFFB4014
#define USB_SYSCON1                 *( VUint16* )0xFFFB4018
#define USB_SYSCON2                 *( VUint16* )0xFFFB401C
#define USB_DEVSTAT                 *( VUint16* )0xFFFB4020
#define USB_SOF                     *( VUint16* )0xFFFB4024
#define USB_IRQ_EN                  *( VUint16* )0xFFFB4028
#define USB_DMA_IRQ_EN              *( VUint16* )0xFFFB402C
#define USB_IRQ_SRC                 *( VUint16* )0xFFFB4030
#define USB_EPN_STAT                *( VUint16* )0xFFFB4034
#define USB_DMAN_STAT               *( VUint16* )0xFFFB4038
#define USB_RXDMA_CFG               *( VUint16* )0xFFFB4040
#define USB_TXDMA_CFG               *( VUint16* )0xFFFB4044
#define USB_DATA_DMA                *( VUint16* )0xFFFB4048
#define USB_TXDMA0                  *( VUint16* )0xFFFB4050
#define USB_TXDMA1                  *( VUint16* )0xFFFB4054
#define USB_TXDMA2                  *( VUint16* )0xFFFB4058
#define USB_RXDMA0                  *( VUint16* )0xFFFB4060
#define USB_RXDMA1                  *( VUint16* )0xFFFB4064
#define USB_RXDMA2                  *( VUint16* )0xFFFB4068
#define USB_EP0                     *( VUint16* )0xFFFB4080
#define USB_EP_RX_BASE              ( VUint32* ) 0xFFFB4084
#define USB_EP1_RX                  *( VUint16* )0xFFFB4084
#define USB_EP2_RX                  *( VUint16* )0xFFFB4088
#define USB_EP3_RX                  *( VUint16* )0xFFFB408C
#define USB_EP4_RX                  *( VUint16* )0xFFFB4090
#define USB_EP5_RX                  *( VUint16* )0xFFFB4094
#define USB_EP6_RX                  *( VUint16* )0xFFFB4098
#define USB_EP7_RX                  *( VUint16* )0xFFFB409C
#define USB_EP8_RX                  *( VUint16* )0xFFFB40A0
#define USB_EP9_RX                  *( VUint16* )0xFFFB40A4
#define USB_EP10_RX                 *( VUint16* )0xFFFB40A8
#define USB_EP11_RX                 *( VUint16* )0xFFFB40AC
#define USB_EP12_RX                 *( VUint16* )0xFFFB40B0
#define USB_EP13_RX                 *( VUint16* )0xFFFB40B4
#define USB_EP14_RX                 *( VUint16* )0xFFFB40B8
#define USB_EP15_RX                 *( VUint16* )0xFFFB40BC
#define USB_EP_TX_BASE              ( VUint32* ) 0xFFFB40C4
#define USB_EP1_TX                  *( VUint16* )0xFFFB40C4
#define USB_EP2_TX                  *( VUint16* )0xFFFB40C8
#define USB_EP3_TX                  *( VUint16* )0xFFFB40CC
#define USB_EP4_TX                  *( VUint16* )0xFFFB40D0
#define USB_EP5_TX                  *( VUint16* )0xFFFB40D4
#define USB_EP6_TX                  *( VUint16* )0xFFFB40D8
#define USB_EP7_TX                  *( VUint16* )0xFFFB40DC
#define USB_EP8_TX                  *( VUint16* )0xFFFB40E0
#define USB_EP9_TX                  *( VUint16* )0xFFFB40E4
#define USB_EP10_TX                 *( VUint16* )0xFFFB40E8
#define USB_EP11_TX                 *( VUint16* )0xFFFB40EC
#define USB_EP12_TX                 *( VUint16* )0xFFFB40F0
#define USB_EP13_TX                 *( VUint16* )0xFFFB40F4
#define USB_EP14_TX                 *( VUint16* )0xFFFB40F8
#define USB_EP15_TX                 *( VUint16* )0xFFFB40FC
#define USB_EP_SIZE                 15

/* Register Parameters for USB EP_NUM ( USB_EP_NUM_ ) */
#define USB_EP_NUM_SETUP_SEL                0x0040
#define USB_EP_NUM_EP_SEL                   0x0020
#define USB_EP_NUM_EP_DIR                   0x0010

/* Register Parameters for USB CTRL ( USB_CTRL_ ) */
#define USB_CTRL_CLR_HALT                   0x0080
#define USB_CTRL_SET_HALT                   0x0040
#define USB_CTRL_SET_FIFO_EN                0x0004
#define USB_CTRL_CLR_EP                     0x0002
#define USB_CTRL_RESET_EP                   0x0001

/* Register Parameters for USB STAT ( USB_STAT_ ) */
#define USB_STAT_FLG_EP_HALTED              0x0040 // Non-Isochronous
#define USB_STAT_FLG_STALL                  0x0020
#define USB_STAT_FLG_NAK                    0x0010
#define USB_STAT_FLG_ACK                    0x0008
#define USB_STAT_FLG_FIFO_EN                0x0004
#define USB_STAT_FLG_NON_ISO_FIFO_EMPTY     0x0002
#define USB_STAT_FLG_NON_ISO_FIFO_FULL      0x0001

/* Register Parameters for USB SYSCON2 ( USB_SYSCON2_ ) */
#define USB_SYSCON2_RMT_WKP                 0x0040 // Remote Wakeup
#define USB_SYSCON2_STALL_CMD               0x0020 // Stall Command
#define USB_SYSCON2_DEV_CFG                 0x0008 // Device Config
#define USB_SYSCON2_CLR_CFG                 0x0004 // Clear Config

/* Register Parameters for USB IRQ_SRC ( USB_DEVSTAT_ ) */
#define USB_DEVSTAT_R_WK_OK                 0x0040
#define USB_DEVSTAT_USB_RESET               0x0020
#define USB_DEVSTAT_SUS                     0x0010
#define USB_DEVSTAT_CFG                     0x0008
#define USB_DEVSTAT_ADD                     0x0004
#define USB_DEVSTAT_DEF                     0x0002
#define USB_DEVSTAT_ATT                     0x0001

/* Register Parameters for USB IRQ_SRC ( USB_IRQ_SRC_ ) */
#define USB_IRQ_SRC_TXN_DONE                0x0400
#define USB_IRQ_SRC_RXN_CNT                 0x0200
#define USB_IRQ_SRC_RXN_EOT                 0x0100
#define USB_IRQ_SRC_SOF                     0x0080
#define USB_IRQ_SRC_EPN_RX                  0x0020
#define USB_IRQ_SRC_EPN_TX                  0x0010
#define USB_IRQ_SRC_DS_CHG                  0x0008
#define USB_IRQ_SRC_SETUP                   0x0004
#define USB_IRQ_SRC_EP0_RX                  0x0002
#define USB_IRQ_SRC_EP0_TX                  0x0001





/* ------------------------------------------------------------------------ *
 *  Control Registers for: USB On-The-Go Module ( OTG_ )                    *
 * ------------------------------------------------------------------------ */
#define OTG_REV                     *( VUint32* )0xFFFB0400
#define OTG_SYSCON_1                *( VUint32* )0xFFFB0404
#define OTG_SYSCON_2                *( VUint32* )0xFFFB0408
#define OTG_CTRL                    *( VUint32* )0xFFFB040C
#define OTG_IRQ_EN                  *( VUint32* )0xFFFB0410
#define OTG_IRQ_SRC                 *( VUint32* )0xFFFB0414
#define OTG_OUTCTRL                 *( VUint32* )0xFFFB0418
#define OTG_TEST                    *( VUint32* )0xFFFB0420
#define OTG_VC                      *( VUint32* )0xFFFB04FC





/* ------------------------------------------------------------------------ *
 *  Control Registers for: DSP MCBSP1 ( DSP_MCBSP1_ )                       *
 * ------------------------------------------------------------------------ */
#define DSP_MCBSP1_DRR2             *( VUint16* )0xE1011800
#define DSP_MCBSP1_DRR1             *( VUint16* )0xE1011802
#define DSP_MCBSP1_DXR2             *( VUint16* )0xE1011804
#define DSP_MCBSP1_DXR1             *( VUint16* )0xE1011806
#define DSP_MCBSP1_SPCR2            *( VUint16* )0xE1011808
#define DSP_MCBSP1_SPCR1            *( VUint16* )0xE101180A
#define DSP_MCBSP1_RCR2             *( VUint16* )0xE101180C
#define DSP_MCBSP1_RCR1             *( VUint16* )0xE101180E
#define DSP_MCBSP1_XCR2             *( VUint16* )0xE1011810
#define DSP_MCBSP1_XCR1             *( VUint16* )0xE1011812
#define DSP_MCBSP1_SRGR2            *( VUint16* )0xE1011814
#define DSP_MCBSP1_SRGR1            *( VUint16* )0xE1011816
#define DSP_MCBSP1_MCR2             *( VUint16* )0xE1011818
#define DSP_MCBSP1_MCR1             *( VUint16* )0xE101181A
#define DSP_MCBSP1_RCERA            *( VUint16* )0xE101181C
#define DSP_MCBSP1_RCERB            *( VUint16* )0xE101181E
#define DSP_MCBSP1_XCERA            *( VUint16* )0xE1011820
#define DSP_MCBSP1_XCERB            *( VUint16* )0xE1011822
#define DSP_MCBSP1_PCR0             *( VUint16* )0xE1011824
#define DSP_MCBSP1_RCERC            *( VUint16* )0xE1011826
#define DSP_MCBSP1_RCERD            *( VUint16* )0xE1011828
#define DSP_MCBSP1_XCERC            *( VUint16* )0xE101182A
#define DSP_MCBSP1_XCERD            *( VUint16* )0xE101182C
#define DSP_MCBSP1_RCERE            *( VUint16* )0xE101182E
#define DSP_MCBSP1_RCERF            *( VUint16* )0xE1011830
#define DSP_MCBSP1_XCERE            *( VUint16* )0xE1011832
#define DSP_MCBSP1_XCERF            *( VUint16* )0xE1011834
#define DSP_MCBSP1_RCERG            *( VUint16* )0xE1011836
#define DSP_MCBSP1_RCERH            *( VUint16* )0xE1011838
#define DSP_MCBSP1_XCERG            *( VUint16* )0xE101183A
#define DSP_MCBSP1_XCERH            *( VUint16* )0xE101183C

/* ------------------------------------------------------------------------ *
 *  Control Registers for: DSP MCBSP2 ( DSP_MCBSP2_ )                       *
 * ------------------------------------------------------------------------ */
#define ARM_MCBSP2_DRR2             *( VUint16* )0xFFFB1000
#define ARM_MCBSP2_DRR1             *( VUint16* )0xFFFB1002
#define ARM_MCBSP2_DXR2             *( VUint16* )0xFFFB1004
#define ARM_MCBSP2_DXR1             *( VUint16* )0xFFFB1006
#define ARM_MCBSP2_SPCR2            *( VUint16* )0xFFFB1008
#define ARM_MCBSP2_SPCR1            *( VUint16* )0xFFFB100A
#define ARM_MCBSP2_RCR2             *( VUint16* )0xFFFB100C
#define ARM_MCBSP2_RCR1             *( VUint16* )0xFFFB100E
#define ARM_MCBSP2_XCR2             *( VUint16* )0xFFFB1010
#define ARM_MCBSP2_XCR1             *( VUint16* )0xFFFB1012
#define ARM_MCBSP2_SRGR2            *( VUint16* )0xFFFB1014
#define ARM_MCBSP2_SRGR1            *( VUint16* )0xFFFB1016
#define ARM_MCBSP2_MCR2             *( VUint16* )0xFFFB1018
#define ARM_MCBSP2_MCR1             *( VUint16* )0xFFFB101A
#define ARM_MCBSP2_RCERA            *( VUint16* )0xFFFB101C
#define ARM_MCBSP2_RCERB            *( VUint16* )0xFFFB101E
#define ARM_MCBSP2_XCERA            *( VUint16* )0xFFFB1020
#define ARM_MCBSP2_XCERB            *( VUint16* )0xFFFB1022
#define ARM_MCBSP2_PCR0             *( VUint16* )0xFFFB1024
#define ARM_MCBSP2_RCERC            *( VUint16* )0xFFFB1026
#define ARM_MCBSP2_RCERD            *( VUint16* )0xFFFB1028
#define ARM_MCBSP2_XCERC            *( VUint16* )0xFFFB102A
#define ARM_MCBSP2_XCERD            *( VUint16* )0xFFFB102C
#define ARM_MCBSP2_RCERE            *( VUint16* )0xFFFB102E
#define ARM_MCBSP2_RCERF            *( VUint16* )0xFFFB1030
#define ARM_MCBSP2_XCERE            *( VUint16* )0xFFFB1032
#define ARM_MCBSP2_XCERF            *( VUint16* )0xFFFB1034
#define ARM_MCBSP2_RCERG            *( VUint16* )0xFFFB1036
#define ARM_MCBSP2_RCERH            *( VUint16* )0xFFFB1038
#define ARM_MCBSP2_XCERG            *( VUint16* )0xFFFB103A
#define ARM_MCBSP2_XCERH            *( VUint16* )0xFFFB103C

/* ------------------------------------------------------------------------ *
 *  Control Registers for: DSP MCBSP3 ( DSP_MCBSP3_ )                       *
 * ------------------------------------------------------------------------ */
#define DSP_MCBSP3_DRR2             *( VUint16* )0xE1017000
#define DSP_MCBSP3_DRR1             *( VUint16* )0xE1017002
#define DSP_MCBSP3_DXR2             *( VUint16* )0xE1017004
#define DSP_MCBSP3_DXR1             *( VUint16* )0xE1017006
#define DSP_MCBSP3_SPCR2            *( VUint16* )0xE1017008
#define DSP_MCBSP3_SPCR1            *( VUint16* )0xE101700A
#define DSP_MCBSP3_RCR2             *( VUint16* )0xE101700C
#define DSP_MCBSP3_RCR1             *( VUint16* )0xE101700E
#define DSP_MCBSP3_XCR2             *( VUint16* )0xE1017010
#define DSP_MCBSP3_XCR1             *( VUint16* )0xE1017012
#define DSP_MCBSP3_SRGR2            *( VUint16* )0xE1017014
#define DSP_MCBSP3_SRGR1            *( VUint16* )0xE1017016
#define DSP_MCBSP3_MCR2             *( VUint16* )0xE1017018
#define DSP_MCBSP3_MCR1             *( VUint16* )0xE101701A
#define DSP_MCBSP3_RCERA            *( VUint16* )0xE101701C
#define DSP_MCBSP3_RCERB            *( VUint16* )0xE101701E
#define DSP_MCBSP3_XCERA            *( VUint16* )0xE1017020
#define DSP_MCBSP3_XCERB            *( VUint16* )0xE1017022
#define DSP_MCBSP3_PCR0             *( VUint16* )0xE1017024
#define DSP_MCBSP3_RCERC            *( VUint16* )0xE1017026
#define DSP_MCBSP3_RCERD            *( VUint16* )0xE1017028
#define DSP_MCBSP3_XCERC            *( VUint16* )0xE101702A
#define DSP_MCBSP3_XCERD            *( VUint16* )0xE101702C
#define DSP_MCBSP3_RCERE            *( VUint16* )0xE101702E
#define DSP_MCBSP3_RCERF            *( VUint16* )0xE1017030
#define DSP_MCBSP3_XCERE            *( VUint16* )0xE1017032
#define DSP_MCBSP3_XCERF            *( VUint16* )0xE1017034
#define DSP_MCBSP3_RCERG            *( VUint16* )0xE1017036
#define DSP_MCBSP3_RCERH            *( VUint16* )0xE1017038
#define DSP_MCBSP3_XCERG            *( VUint16* )0xE101703A
#define DSP_MCBSP3_XCERH            *( VUint16* )0xE101703C

/* Register Parameters for MCBSP PCR0 ( MCBSP_PCR0_ ) */
#define MCBSP_PCR0_IDLE_EN                  0x4000
#define MCBSP_PCR0_XIOEN                    0x2000
#define MCBSP_PCR0_RIOEN                    0x1000
#define MCBSP_PCR0_FSXM                     0x0800
#define MCBSP_PCR0_FSRM                     0x0400
#define MCBSP_PCR0_CLKXM                    0x0200
#define MCBSP_PCR0_CLKRM                    0x0100
#define MCBSP_PCR0_SCLKME                   0x0080
#define MCBSP_PCR0_CLKS_STAT                0x0040
#define MCBSP_PCR0_DX_STAT                  0x0020
#define MCBSP_PCR0_DR_STAT                  0x0010
#define MCBSP_PCR0_FSXP                     0x0008
#define MCBSP_PCR0_FSRP                     0x0004
#define MCBSP_PCR0_CLKXP                    0x0002
#define MCBSP_PCR0_CLKRP                    0x0001

/* Register Parameters for MCBSP RCR2 ( MCBSP_RCR2_ ) */
#define MCBSP_RCR2_RPHASE                   0x8000
#define MCBSP_RCR2_RFIG                     0x0004

/* Register Parameters for MCBSP XCR2 ( MCBSP_XCR2_ ) */
#define MCBSP_XCR2_XPHASE                   0x8000
#define MCBSP_XCR2_XFIG                     0x0004

/* Register Parameters for MCBSP SPCR1 ( MCBSP_SPCR1_ ) */
#define MCBSP_SPCR1_DLB                     0x8000
#define MCBSP_SPCR1_DXENA                   0x0020
#define MCBSP_SPCR1_ABIS                    0x0010
#define MCBSP_SPCR1_RSYNCERR                0x0008
#define MCBSP_SPCR1_RFULL                   0x0004
#define MCBSP_SPCR1_RRDY                    0x0002
#define MCBSP_SPCR1_RRST                    0x0001
#define MCBSP_SPCR1_RRST                    0x0001

/* Register Parameters for MCBSP SPCR2 ( MCBSP_SPCR2_ ) */
#define MCBSP_SPCR2_FREE                    0x0200
#define MCBSP_SPCR2_SOFT                    0x0100
#define MCBSP_SPCR2_FRST                    0x0080
#define MCBSP_SPCR2_GRST                    0x0040
#define MCBSP_SPCR2_XSYNCERR                0x0008
#define MCBSP_SPCR2_XEMPTY                  0x0004
#define MCBSP_SPCR2_XRDY                    0x0002
#define MCBSP_SPCR2_XRST                    0x0001
