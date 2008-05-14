/*  ----------------------------------------------------------------------------
 *  Caution ! This is a generated file.
 *  It defines the run-time configuration.
 *  Any change made in this file will be lost.
 *  
 *  The configuration was created on : APR 17, 2006  14:16:02
 *  This file was generated on       : APR 18, 2006  16:35:15
 *  ----------------------------------------------------------------------------
 */


#ifndef __CFG_OMAP_GPP_H__ 
#define __CFG_OMAP_GPP_H__ 


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/*  ----------------------------------------------------------------------------
 *  Maximum number of processors supported in this configuration
 *  ----------------------------------------------------------------------------
 */
#define MAX_PROCESSORS          2

/*  ----------------------------------------------------------------------------
 *  Maximum number of DSPs supported in this configuration
 *  ----------------------------------------------------------------------------
 */
#define MAX_DSPS                1

/*  ----------------------------------------------------------------------------
 *  Maximum number of MSGQs supported in this configuration
 *  ----------------------------------------------------------------------------
 */
#define MAX_MSGQS               16

/*  ----------------------------------------------------------------------------
 *  Identifier for the GPP in this confguration
 *  ----------------------------------------------------------------------------
 */
#define ID_GPP                  1

/*  ----------------------------------------------------------------------------
 *  Maximum data buffer size supported by ZCPYDATA
 *  ----------------------------------------------------------------------------
 */
#define ZCPYDATA_MAX_BUFSIZE    16384

/*  ----------------------------------------------------------------------------
 *  Offset of the area in the memory block.
 *  ----------------------------------------------------------------------------
 */
#define SHMDRV_ADDR_OFFSET      0x0

/*  ----------------------------------------------------------------------------
 *  Offset of SHMIPS in memory
 *  ----------------------------------------------------------------------------
 */
#define SHMIPS_ADDR_OFFSET      (SHMDRV_ADDR_OFFSET + SHMDRV_CTRL_SIZE)

/*  ----------------------------------------------------------------------------
 *  Offset of ZCPYDATA in memory
 *  ----------------------------------------------------------------------------
 */
#define ZCPYDATA_ADDR_OFFSET    (SHMIPS_ADDR_OFFSET + (SHMIPS_CTRL_SIZE) + (SHMIPS_IRP_SIZE * 16 * 1))

/*  ----------------------------------------------------------------------------
 *  Offset of SMAPOOL in memory
 *  ----------------------------------------------------------------------------
 */
#define SMAPOOL_ADDR_OFFSET     (ZCPYDATA_ADDR_OFFSET + ZCPYDATA_CTRL_SIZE)

/*  ----------------------------------------------------------------------------
 *  Total number of channels supported by all link drivers in this configuration
 *  ----------------------------------------------------------------------------
 */
#define MAX_CHANNELS            16



#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* ifndef __CFG_OMAP_GPP_H__ */
