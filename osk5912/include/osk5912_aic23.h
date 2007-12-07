/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23.h ========
 *  Codec interface for AIC23 on the OSK5912 board
 */

#ifndef OSK5912_AIC23_
#define OSK5912_AIC23_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ *
 *  AIC23 Definitions                                                       *
 * ------------------------------------------------------------------------ */
/* I2C Address for AIC23 - dependent on value of AIC23:CS signal */
//#define OSK5912_AIC23_I2CADDR     0x1a    // AIC23:CS = 0
#define OSK5912_AIC23_I2CADDR       0x1b    // AIC23:CS = 1

/* Codec module definitions */
#define OSK5912_AIC23_NUMREGS       10
#define OSK5912_AIC23_LEFTINVOL     0
#define OSK5912_AIC23_RIGHTINVOL    1
#define OSK5912_AIC23_LEFTHPVOL     2
#define OSK5912_AIC23_RIGHTHPVOL    3
#define OSK5912_AIC23_ANAPATH       4
#define OSK5912_AIC23_DIGPATH       5
#define OSK5912_AIC23_POWERDOWN     6
#define OSK5912_AIC23_DIGIF         7
#define OSK5912_AIC23_SAMPLERATE    8
#define OSK5912_AIC23_DIGACT        9
#define OSK5912_AIC23_RESET         15

/* Frequency definitions */
#define OSK5912_AIC23_FREQ_8KHZ     1
#define OSK5912_AIC23_FREQ_16KHZ    2
#define OSK5912_AIC23_FREQ_24KHZ    3
#define OSK5912_AIC23_FREQ_32KHZ    4
#define OSK5912_AIC23_FREQ_44KHZ    5
#define OSK5912_AIC23_FREQ_48KHZ    6
#define OSK5912_AIC23_FREQ_96KHZ    7

/* ------------------------------------------------------------------------ *
 *  AIC23 Data Structures                                                   *
 * ------------------------------------------------------------------------ */
/* Codec Handle */
typedef int OSK5912_AIC23_CodecHandle;

/* AIC23 Codec Structure */
typedef struct
{
    Int16 regs[OSK5912_AIC23_NUMREGS];
} OSK5912_AIC23_Config;

/* ------------------------------------------------------------------------ *
 *  AIC23 Default Settings                                                  *
 * ------------------------------------------------------------------------ */
#define OSK5912_AIC23_DEFAULTCONFIG { \
    0x0017, /* Set-Up Reg 0       Left line input channel volume control */   \
            /* LRS     0          simultaneous left/right volume: disabled */ \
            /* LIM     0          left line input mute: disabled */           \
            /* XX      00         reserved */                                 \
            /* LIV     10111      left line input volume: 0 dB */             \
                                                                             \
    0x0017, /* Set-Up Reg 1       Right line input channel volume control */  \
            /* RLS     0          simultaneous right/left volume: disabled */ \
            /* RIM     0          right line input mute: disabled */          \
            /* XX      00         reserved */                                 \
            /* RIV     10111      right line input volume: 0 dB */            \
                                                                             \
    0x01f9, /* Set-Up Reg 2       Left channel headphone volume control */    \
            /* LRS     1          simultaneous left/right volume: enabled */  \
            /* LZC     1          left channel zero-cross detect: enabled */  \
            /* LHV     1111001    left headphone volume: 0 dB */              \
                                                                             \
    0x01f9, /* Set-Up Reg 3       Right channel headphone volume control */   \
            /* RLS     1          simultaneous right/left volume: enabled */  \
            /* RZC     1          right channel zero-cross detect: enabled */ \
            /* RHV     1111001    right headphone volume: 0 dB */             \
                                                                             \
    0x0011, /* Set-Up Reg 4       Analog audio path control */                \
            /* X        0         reserved */                                 \
            /* STA     00         sidetone attenuation: -6 dB */              \
            /* STE     0          sidetone: disabled */                       \
            /* DAC     1          DAC: selected */                            \
            /* BYP     0          bypass: off */                              \
            /* INSEL    0         input select for ADC: line */               \
            /* MICM    0          microphone mute: disabled */                \
            /* MICB    1          microphone boost: enabled */                \
                                                                             \
    0x0000, /* Set-Up Reg 5       Digital audio path control */               \
            /* XXXXX    00000     reserved */                                 \
            /* DACM    0          DAC soft mute: disabled */                  \
            /* DEEMP    00        deemphasis control: disabled */             \
            /* ADCHP    0         ADC high-pass filter: disabled */           \
                                                                             \
    0x0000, /* Set-Up Reg 6       Power down control */                       \
            /* X        0         reserved */                                 \
            /* OFF     0          device power: on (i.e. not off) */          \
            /* CLK     0          clock: on */                                \
            /* OSC     0          oscillator: on */                           \
            /* OUT     0          outputs: on */                              \
            /* DAC     0          DAC: on */                                  \
            /* ADC     0          ADC: on */                                  \
            /* MIC     0          microphone: on */                           \
            /* LINE    0          line input: on */                           \
                                                                             \
    0x0043, /* Set-Up Reg 7       Digital audio interface format */           \
            /* XX      00         reserved */                                 \
            /* MS      1          master/slave mode: master */                \
            /* LRSWAP  0          DAC left/right swap: disabled */            \
            /* LRP     0          DAC lrp: MSB on 1st BCLK */                 \
            /* IWL     00         input bit length: 16 bit */                 \
            /* FOR     11         data format: DSP format */                  \
                                                                             \
    0x0081, /* Set-Up Reg 8       Sample rate control */                      \
            /* X        0         reserved */                                 \
            /* CLKOUT  1          clock output divider: 2 (MCLK/2) */         \
            /* CLKIN    0         clock input divider: 2 (MCLK/2) */          \
            /* SR, BOSR 00000     sampling rate: ADC  48 kHz DAC  48 kHz */   \
            /* USB/N    1         clock mode select (USB/normal): USB */      \
                                                                             \
    0x0001  /* Set-Up Reg 9       Digital interface activation */             \
            /* XX..X    00000000  reserved */                                 \
            /* ACT     1          active */                                   \
}

/* ------------------------------------------------------------------------ *
 *  MCBSP Register Index                                                    *
 * ------------------------------------------------------------------------ */

typedef enum
{
    DRR2 = 0,
    DRR1,
    DXR2,
    DXR1,
    SPCR2,
    SPCR1,
    RCR2,
    RCR1,
    XCR2,
    XCR1,
    SRGR2,
    SRGR1,
    MCR2,
    MCR1,
    RCERA,
    RCERB,
    XCERA,
    XCERB,
    PCR0
} MCBSP_REG_INDEX;

/* ------------------------------------------------------------------------ *
 *  AIC23 Prototypes                                                        *
 * ------------------------------------------------------------------------ */

/* Set codec register regnum to value regval */
void OSK5912_AIC23_rset(OSK5912_AIC23_CodecHandle hCodec, Uint16 regnum,
                                                                Uint16 regval);

/* Return value of codec register regnum */
Uint16 OSK5912_AIC23_rget(OSK5912_AIC23_CodecHandle hCodec, Uint16 regnum);

/* Open the codec with id and return handle */
OSK5912_AIC23_CodecHandle OSK5912_AIC23_openCodec( Uint32 id,
                                                OSK5912_AIC23_Config *Config);

/* Close the codec */
void OSK5912_AIC23_closeCodec(OSK5912_AIC23_CodecHandle hCodec);

/* Configure the codec register values */
void OSK5912_AIC23_config(OSK5912_AIC23_CodecHandle hCodec,
                                                OSK5912_AIC23_Config *Config);

/* Write a 16-bit value to the codec */
Uint16 OSK5912_AIC23_write16(OSK5912_AIC23_CodecHandle hCodec, Int16 val);

/* Write a 32-bit value to the codec */
Uint16 OSK5912_AIC23_write32(OSK5912_AIC23_CodecHandle hCodec, Int32 val);

/* Read a 16-bit value from the codec */
Uint16 OSK5912_AIC23_read16(OSK5912_AIC23_CodecHandle hCodec, Int16 *val);

/* Read a 32-bit value from the codec */
Uint16 OSK5912_AIC23_read32(OSK5912_AIC23_CodecHandle hCodec, Int32 *val);

/* Set the codec output gain */
void OSK5912_AIC23_outGain(OSK5912_AIC23_CodecHandle hCodec, Uint16 outGain);

/* Set the codec loopback mode */
void OSK5912_AIC23_loopback(OSK5912_AIC23_CodecHandle hCodec, Uint16 mode);

/* Enable/disable codec mute mode */
void OSK5912_AIC23_mute(OSK5912_AIC23_CodecHandle hCodec, Uint16 mode);

/* Enable/disable codec powerdown modes for DAC, ADC */
void OSK5912_AIC23_powerDown(OSK5912_AIC23_CodecHandle hCodec, Uint16 sect);

/* Set the codec sample rate frequency */
void OSK5912_AIC23_setFreq(OSK5912_AIC23_CodecHandle hCodec, Uint32 freq);

#ifdef __cplusplus
}
#endif

#endif
