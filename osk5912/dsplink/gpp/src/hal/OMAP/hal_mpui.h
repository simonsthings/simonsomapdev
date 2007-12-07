/** ============================================================================
 *  @file   hal_mpui.h
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Declares necessary functions for MPU Port Interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (HAL_MPUI_H)
#define HAL_MPUI_H


USES (gpptypes.h)
USES (errbase.h)

#if defined (__cplusplus)
EXTERN "C" {
#endif


/*  ============================================================================
 *  @const  MPUI_XXX constants
 *
 *  @desc   ARM Port Interface offset and bit positions.
 *  ============================================================================
 */
#define     MPUI_CTRL_OFFSET                0x0000
#define     MPUI_HIGH_FREQ_POSBIT           0
#define     MPUI_TIMEOUT_EN_POSBIT          1
#define     MPUI_MPUI_ERR_EN_POSBIT         3
#define     MPUI_ACCESS_FACTOR_POSBIT       4
#define     MPUI_TIMEOUT_POSBIT             8
#define     MPUI_ENDIANISM_POSBIT           16
#define     MPUI_ACCESS_PRIORITY_POSBIT     18


/*  ============================================================================
 *  @const  MPUI_DSP_BOOT_OFFSET
 *
 *  @desc   Offset of API DSP Boot.
 *  ============================================================================
 */
#define     MPUI_DSP_BOOT_OFFSET            0x0018

/*  ============================================================================
 *  @const  MPUI_DSP_STATUS_OFFSET
 *
 *  @desc   Offset of DSP status register for SAM/HOM mode.
 *  ============================================================================
 */
#define     MPUI_DSP_STATUS_OFFSET          0x0014


/*  ============================================================================
 *  @const  HOM_MODE_VAL
 *
 *  @desc   Value to detect HOM -> SAM mode transition.
 *  ============================================================================
 */
#define     HOM_MODE_VAL                    0x0C00

/*  ============================================================================
 *  @const  MPUI_SIZE_OFFSET
 *
 *  @desc   Offset of API Size.
 *  ============================================================================
 */
#define     MPUI_SIZE_OFFSET                0x001C


/** ============================================================================
 *  @name   MPUI_FreqMode
 *
 *  @desc   Enumerates frequency modes for MPUI.
 *
 *  @field  MPUI_FREQ_LOW
 *              Low frequency MPU Clock.
 *  @field  MPUI_FREQ_HIGH
 *              High frequency MPU Clock.
 *  ============================================================================
 */
typedef enum {
    MPUI_FREQ_LOW  = 0,
    MPUI_FREQ_HIGH = 1
} MPUI_FreqMode ;

/** ============================================================================
 *  @name   MPUI_TimeoutEn
 *
 *  @desc   Enumerates Timeout modes for API.
 *
 *  @field  MPUI_TIMEOUT_DIS
 *              Disable timeout.
 *  @field  MPUI_TIMEOUT_EN
 *              Enable timeout.
 *  ============================================================================
 */
typedef enum {
    MPUI_TIMEOUT_DIS = 0,
    MPUI_TIMEOUT_EN  = 1
} MPUI_TimeoutEn ;

/** ============================================================================
 *  @name   MPUI_ErrMode
 *
 *  @desc   Enumerates modes for sending S_NABORT signal for API.
 *
 *  @field  MPUI_ERR_DIS
 *              API Error disable.
 *  @field  MPUI_ERR_EN
 *              API error enable.
 *  ============================================================================
 */
typedef enum {
    MPUI_ERR_DIS = 0,
    MPUI_ERR_EN  = 1
} MPUI_ErrMode ;

/** ============================================================================
 *  @name   MPUI_AccessFactor
 *
 *  @desc   Enumerates for access frequency-division factor.
 *
 *  @field  MPUI_ACCESS_FACTOR_0
 *              MPUI Access frequency division factor 0.
 *  @field  MPUI_ACCESS_FACTOR_1
 *              MPUI Access frequency division factor 1.
 *  @field  MPUI_ACCESS_FACTOR_2
 *              MPUI Access frequency division factor 2.
 *  @field  MPUI_ACCESS_FACTOR_3
 *              MPUI Access frequency division factor 3.
 *  @field  MPUI_ACCESS_FACTOR_4
 *              MPUI Access frequency division factor 4.
 *  @field  MPUI_ACCESS_FACTOR_5
 *              MPUI Access frequency division factor 5.
 *  @field  MPUI_ACCESS_FACTOR_6
 *              MPUI Access frequency division factor 6.
 *  @field  MPUI_ACCESS_FACTOR_7
 *              MPUI Access frequency division factor 7.
 *  @field  MPUI_ACCESS_FACTOR_8
 *              MPUI Access frequency division factor 8.
 *  @field  MPUI_ACCESS_FACTOR_9
 *              MPUI Access frequency division factor 9.
 *  @field  MPUI_ACCESS_FACTOR_10
 *              MPUI Access frequency division factor 10.
 *  @field  MPUI_ACCESS_FACTOR_11
 *              MPUI Access frequency division factor 11.
 *  @field  MPUI_ACCESS_FACTOR_12
 *              MPUI Access frequency division factor 12.
 *  @field  MPUI_ACCESS_FACTOR_13
 *              MPUI Access frequency division factor 13.
 *  @field  MPUI_ACCESS_FACTOR_14
 *              MPUI Access frequency division factor 14.
 *  @field  MPUI_ACCESS_FACTOR_15
 *              MPUI Access frequency division factor 15.
 *  ============================================================================
 */
typedef enum {
    MPUI_ACCESS_FACTOR_0 = 0,
    MPUI_ACCESS_FACTOR_1 = 1,
    MPUI_ACCESS_FACTOR_2 = 2,
    MPUI_ACCESS_FACTOR_3 = 3,
    MPUI_ACCESS_FACTOR_4 = 4,
    MPUI_ACCESS_FACTOR_5 = 5,
    MPUI_ACCESS_FACTOR_6 = 6,
    MPUI_ACCESS_FACTOR_7 = 7,
    MPUI_ACCESS_FACTOR_8 = 8,
    MPUI_ACCESS_FACTOR_9 = 9,
    MPUI_ACCESS_FACTOR_10 = 10,
    MPUI_ACCESS_FACTOR_11 = 11,
    MPUI_ACCESS_FACTOR_12 = 12,
    MPUI_ACCESS_FACTOR_13 = 13,
    MPUI_ACCESS_FACTOR_14 = 14,
    MPUI_ACCESS_FACTOR_15 = 15
} MPUI_AccessFactor ;

/** ============================================================================
 *  @name   MPUI_Timeout
 *
 *  @desc   Enumerates timeout values.
 *
 *  @field  MPUI_TIMEOUT_MIN
 *              Minimum API timeout.
 *  @field  MPUI_TIMEOUT_MAX
 *              Maximum API timeout.
 *  ============================================================================
 */
typedef enum {
    MPUI_TIMEOUT_MIN = 0,
    MPUI_TIMEOUT_MAX = 255
} MPUI_Timeout ;

/** ============================================================================
 *  @name   MPUI_Endianism
 *
 *  @desc   Enumerates endianism conversions.
 *
 *  @field  MPUI_ENDIANISM_NO_CONVERT
 *              No API endianism conversion.
 *  @field  MPUI_ENDIANISM_CONVERT_ALL_ACCESS
 *              API endianism conversion for all access.
 *  @field  MPUI_ENDIANISM_CONVERT_MPUI_MEM_ONLY
 *              API endianism conversion for memory access only.
 *  ============================================================================
 */
typedef enum {
    MPUI_ENDIANISM_NO_CONVERT            = 0,
    MPUI_ENDIANISM_CONVERT_ALL_ACCESS    = 2,
    MPUI_ENDIANISM_CONVERT_MPUI_MEM_ONLY = 3
} MPUI_Endianism ;

/** ============================================================================
 *  @name   MPUI_AccessPriority
 *
 *  @desc   Enumerates access priority.
 *
 *  @field  MPUI_ACCESS_PRIORITY_ARM_DMA_HSAB
 *              Access priority order ARM > DMA > HSAB.
 *  @field  MPUI_ACCESS_PRIORITY_ARM_HSAB_DMA
 *              Access priority order ARM > HSAB > DMA.
 *  @field  MPUI_ACCESS_PRIORITY_DMA_ARM_HSAB
 *              Access priority order DMA > ARM > HSAB.
 *  @field  MPUI_ACCESS_PRIORITY_HSAB_ARM_DMA
 *              Access priority order HSAB > ARM > DMA.
 *  @field  MPUI_ACCESS_PRIORITY_DMA_HSAB_ARM
 *              Access priority order DMA > HSAB > ARM.
 *  @field  MPUI_ACCESS_PRIORITY_HSAB_DMA_ARM
 *              Access priority order HSAB > DMA > ARM.
 *  ============================================================================
 */
typedef enum {
    MPUI_ACCESS_PRIORITY_ARM_DMA_HSAB = 0,
    MPUI_ACCESS_PRIORITY_ARM_HSAB_DMA = 1,
    MPUI_ACCESS_PRIORITY_DMA_ARM_HSAB = 2,
    MPUI_ACCESS_PRIORITY_HSAB_ARM_DMA = 3,
    MPUI_ACCESS_PRIORITY_DMA_HSAB_ARM = 4,
    MPUI_ACCESS_PRIORITY_HSAB_DMA_ARM = 5
} MPUI_AccessPriority ;

/** ============================================================================
 *  @name   MPUI_DSPBootMode
 *
 *  @desc   Enumerates boot modes for DSP.
 *
 *  @field  MPUI_DSP_BOOT_INTERNAL
 *              Internal boot mode.
 *  @field  MPUI_DSP_BOOT_EXTERNAL
 *              External boot mode.
 *  @field  MPUI_DSP_BOOT_EMIF16
 *              EMIF16 boot mode.
 *  @field  MPUI_DSP_BOOT_PSEUDO_EXT
 *              Pseudo external mode.
 *  @field  MPUI_DSP_BOOT_MPNMC
 *              MPNMC boot mode.
 *  ============================================================================
 */
typedef enum {
    MPUI_DSP_BOOT_INTERNAL   = 5,
    MPUI_DSP_BOOT_EXTERNAL   = 4,
    MPUI_DSP_BOOT_EMIF16     = 3,
    MPUI_DSP_BOOT_PSEUDO_EXT = 1,
    MPUI_DSP_BOOT_MPNMC      = 0
} MPUI_DSPBootMode ;


/** ============================================================================
 *  @func   HAL_MpuiSetup
 *
 *  @desc   Setup configuration of the ARM Port Interface.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    freq_mode
 *              frequency mode.
 *  @arg    timeout_en
 *              enable or disable the timeout feature.
 *  @arg    api_err_en
 *              enable the s_nabort signal.
 *  @arg    access_factor
 *              division factor for the access frequency.
 *  @arg    timeout
 *              timeout value.
 *  @arg    endianism
 *              endianism conversion.
 *  @arg    access_priority
 *              APIF access priority.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_MpuiSetup (IN HalObject *           halObject,
               IN MPUI_FreqMode         freq_mode,
               IN MPUI_TimeoutEn        timeout_en,
               IN MPUI_ErrMode          api_err_en,
               IN MPUI_AccessFactor     access_factor,
               IN MPUI_Timeout          timeout,
               IN MPUI_Endianism        endianism,
               IN MPUI_AccessPriority   access_priority) ;


/** ============================================================================
 *  @func   HAL_MpuiBootConfig
 *
 *  @desc   Setup boot configuration of the DSP.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    dsp_boot_mode
 *              DSP boot mode.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_MpuiBootConfig (IN HalObject *      halObject,
                   IN MPUI_DSPBootMode  dsp_boot_mode) ;


/** ============================================================================
 *  @func   HAL_MpuiSetSize
 *
 *  @desc   Setup API size.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    size
 *              set to 0x0 when all the SARAM's need to be accessed in HOM.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_MpuiSetSize (IN HalObject * halObject, IN Uint32 size) ;


/** ============================================================================
 *  @func   HAL_MpuiGetSize
 *
 *  @desc   Get API size.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @ret    API Size.
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Uint32
HAL_MpuiGetSize (IN HalObject * halObject) ;



/** ============================================================================
 *  @func   HAL_IsDspInSAM
 *
 *  @desc   Function checks if MPUI is in SAM mode.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @ret    TRUE
 *              If DSP is in SAM
 *          FALSE
 *              If DSP is in HOM
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Bool
HAL_IsDspInSAM (IN HalObject * halObject) ;


#if defined (__cplusplus)
}
#endif


#endif  /* !defined (HAL_MPUI_H) */
