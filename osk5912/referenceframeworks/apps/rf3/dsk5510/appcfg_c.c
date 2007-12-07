/*   Do *not* directly modify this file.  It was    */
/*   generated by the Configuration Tool; any  */
/*   changes risk being overwritten.                */

/* INPUT app.cdb */

/*  Include Header File  */
#include "appcfg.h"


#ifdef __cplusplus
#pragma CODE_SECTION(".text:CSL_cfgInit")
#else
#pragma CODE_SECTION(CSL_cfgInit,".text:CSL_cfgInit")
#endif


#ifdef __cplusplus
#pragma FUNC_EXT_CALLED()
#else
#pragma FUNC_EXT_CALLED(CSL_cfgInit)
#endif

/*  Config Structures */
EMIF_Config emifCfg0 = {
    0x0221,        /*  Global Control Register   */
    0xffff,        /*  Global Reset Register   */
    0x3fff,        /*  CE0 Space Control Register 1   */
    0x5fff,        /*  CE0 Space Control Register 2   */
    0x0000,        /*  CE0 Space Control Register 3   */
    0x1038,        /*  CE1 Space Control Register 1   */
    0x0038,        /*  CE1 Space Control Register 2   */
    0x0000,        /*  CE1 Space Control Register 3   */
    0x1050,        /*  CE2 Space Control Register 1   */
    0x0050,        /*  CE2 Space Control Register 2   */
    0x0000,        /*  CE2 Space Control Register 3   */
    0x1050,        /*  CE3 Space Control Register 1   */
    0x0050,        /*  CE3 Space Control Register 2   */
    0x0000,        /*  CE3 Space Control Register 3   */
    0x2b11,        /*  SDRAM Control Register 1   */
    0x0578,        /*  SDRAM Period Register   */
    0xffff,        /*  SDRAM Initialization Register   */
    0x0535         /*  SDRAM Control Register 2   */
};

/*  Handles  */

/*
 *  ======== CSL_cfgInit() ========  
 */
void CSL_cfgInit()
{
        
    EMIF_config(&emifCfg0);

    /* You must use DMA_start() in your main code to start the DMA. */

	
    
    /* You must use MCBSP_start() in your main code to start the MCBSP. */

    
    

}
