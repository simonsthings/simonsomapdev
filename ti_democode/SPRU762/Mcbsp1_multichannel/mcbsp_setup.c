/*
 * Set up the McBSP registers
 *
 *
 */

#include <csl.h>
#include <csl_error.h>
#include <csl_intc.h>
#include <csl_chip.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>
#include <soc.h>


/*-------Macro that gives 2 CLK delay cycles---------*/
/* The delay depends on the DSP frequency and the CLKX/R rate */
#define WAIT_FOR_2_CLK  do {                                   \
                               volatile int delayCnt = 20;      \
                               while(delayCnt > 0) --delayCnt; \
                           } while (0)
                           
                           
/******** McBSP 1 Master **********/
/*---------Global data definition---------*/
CSL_McbspGlobalSetup gblCfg1 = {
    CSL_MCBSP_IOMODE_TXDIS_RXDIS,	// set up pins as McBSP pins
    CSL_MCBSP_DLBMODE_OFF,			// disable digital loopback mode
    CSL_MCBSP_CLKSTP_DISABLE		// disable clock stop mode
};

CSL_McbspDataSetup rcvDataCfg1 = {
    CSL_MCBSP_PHASE_SINGLE,			// one phase only
    CSL_MCBSP_WORDLEN_16,			// 16-bit words
    6, /* FRMLEN1 */				// frame length=6 channels (highest channel used)
    CSL_MCBSP_WORDLEN_8, 			// default
    1, /* FRMLEN2 */				// default
    CSL_MCBSP_FRMSYNC_DETECT,		// detect unexpected frames
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST,// MSB first
    CSL_MCBSP_DATADELAY_2_BITS,		// 2-bit data delay
    CSL_MCBSP_RJUSTDXENA_RJUST_RZF, // right justified
    CSL_MCBSP_INTMODE_ON_READY		// default
};

CSL_McbspDataSetup xmtDataCfg1 = {
    CSL_MCBSP_PHASE_SINGLE,			// one phase only
    CSL_MCBSP_WORDLEN_16,			// 16-bit words
    6, /* FRMLEN1 */				// frame length=6 chanenls (highest channel used)
    CSL_MCBSP_WORDLEN_8,			// default
    1, /* FRMLEN2 */				// default
    CSL_MCBSP_FRMSYNC_DETECT,		// detect unexpected frames
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST,// MSB first
    CSL_MCBSP_DATADELAY_2_BITS,		// 2-bit data delay
    CSL_MCBSP_RJUSTDXENA_DXENA_OFF,	// DXENA not supported
    CSL_MCBSP_INTMODE_ON_READY		// default
};

CSL_McbspClkSetup clkCfg1 = {
    CSL_MCBSP_FSCLKMODE_INTERNAL, 	// master FSX transmitter
    CSL_MCBSP_FSCLKMODE_EXTERNAL, 	// slave FSR receiver
    CSL_MCBSP_TXRXCLKMODE_INTERNAL, // master CLKX 
    CSL_MCBSP_TXRXCLKMODE_EXTERNAL, // slave CLKR
    CSL_MCBSP_FSPOL_ACTIVE_HIGH,   	// FSX is active high
    CSL_MCBSP_FSPOL_ACTIVE_HIGH, 	// FSR is active high
    CSL_MCBSP_CLKPOL_TX_RISING_EDGE,// CLKX transmits on rising edge
    CSL_MCBSP_CLKPOL_RX_FALLING_EDGE,// CLKR samples on falling edge
    0,  							// FSG width: 1 bit
    95, //		 Frame-sync pulse period = 16*6 bits, minimum FPER 
    24,  							// CLKGDIV = 25
    CSL_MCBSP_SRGCLK_CLKCPU, 		// SCLKME = 0, CLKSM = 1, internal source
    CSL_MCBSP_CLKPOL_SRG_RISING_EDGE, // default
    CSL_MCBSP_TXFSMODE_SRG, 		// Frame-sync generated periodically
    CSL_MCBSP_CLKGSYNCMODE_OFF  	// default
};

CSL_McbspMulChSetup mulChCfg1 = CSL_MCBSP_MULTICHAN_DEFAULTS;

CSL_McbspHwSetup myHwCfg1 = {
    &gblCfg1,
    &rcvDataCfg1,
    &xmtDataCfg1,
    &clkCfg1,
    &mulChCfg1,
    CSL_MCBSP_EMU_STOP,
//    CSL_MCBSP_EMU_FREERUN,
    NULL
};

/******** McBSP 3 Slave **********/
/*---------Global data definition---------*/
CSL_McbspGlobalSetup gblCfg3 = {
    CSL_MCBSP_IOMODE_TXDIS_RXDIS,	// pins work as McBSP pins
    CSL_MCBSP_DLBMODE_OFF,			// disable digital loopback mode
    CSL_MCBSP_CLKSTP_DISABLE		// disable clock stop mode
};

CSL_McbspDataSetup rcvDataCfg3 = {
    CSL_MCBSP_PHASE_SINGLE,			// single phase only
    CSL_MCBSP_WORDLEN_16,			// 16-bit words
    6, /* FRMLEN1 */				// frame length=6 channels (highest channel used)
    CSL_MCBSP_WORDLEN_8, 			// default
    1, /* FRMLEN2 */				// default
    CSL_MCBSP_FRMSYNC_DETECT,		// detect unexpected frames 
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST,// MSB first
    CSL_MCBSP_DATADELAY_2_BITS,		// 2-bit data delay
    CSL_MCBSP_RJUSTDXENA_RJUST_RZF,	// default
	CSL_MCBSP_INTMODE_ON_SYNCERR	// interrupt on unexpected frame sync
};

CSL_McbspDataSetup xmtDataCfg3 = {
    CSL_MCBSP_PHASE_SINGLE,			// single phase only
    CSL_MCBSP_WORDLEN_16,			// 16-bit words
    6, /* FRMLEN1 */				// frame length=6 channels (highest channel used)
    CSL_MCBSP_WORDLEN_8,			// default
    1, /* FRMLEN2 */				// default
    CSL_MCBSP_FRMSYNC_DETECT,		// detect unexpected frames
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST,// MSB first
    CSL_MCBSP_DATADELAY_2_BITS,		// 2-bit data delay
    CSL_MCBSP_RJUSTDXENA_DXENA_OFF,	// dxena not supported
    CSL_MCBSP_INTMODE_ON_FSYNC		// interrupt on frame sync detection
};

CSL_McbspClkSetup clkCfg3 = {
    CSL_MCBSP_FSCLKMODE_EXTERNAL, 	// slave FSX transmitter
    CSL_MCBSP_FSCLKMODE_EXTERNAL, 	// slave FSR receiver
    CSL_MCBSP_TXRXCLKMODE_EXTERNAL,	// slave CLKX trasmitter
    CSL_MCBSP_TXRXCLKMODE_EXTERNAL,	// slave CLKR receiver
    CSL_MCBSP_FSPOL_ACTIVE_HIGH,	// FSX is active high
    CSL_MCBSP_FSPOL_ACTIVE_HIGH,	// FSR is active high
    CSL_MCBSP_CLKPOL_TX_RISING_EDGE,// CLKX transmits on rising edge
    CSL_MCBSP_CLKPOL_RX_FALLING_EDGE,//CLKR samples on falling edge
    0,  							// default
    95, 							// default
    255, 							// default
    CSL_MCBSP_SRGCLK_CLKCPU, 		// SCLKME = 0, CLKSM = 1, internal source
    CSL_MCBSP_CLKPOL_SRG_RISING_EDGE, // default
    CSL_MCBSP_TXFSMODE_SRG, 		// default
    CSL_MCBSP_CLKGSYNCMODE_OFF 		// default
};

CSL_McbspMulChSetup mulChCfg3 = CSL_MCBSP_MULTICHAN_DEFAULTS;

CSL_McbspHwSetup myHwCfg3 = {
    &gblCfg3,
    &rcvDataCfg3,
    &xmtDataCfg3,
    &clkCfg3,
    &mulChCfg3,
    CSL_MCBSP_EMU_STOP,
//    CSL_MCBSP_EMU_FREERUN,
    NULL
};



void mcbsp_setup (CSL_McbspHandle *hMcbsp1, CSL_McbspHandle *hMcbsp3)
{
    /* Initialize CSL library, this step is required */
    CSL_mcbspInit(NULL);

  	/* Program MCBSP control registers and wait for 2 clock cycles */
	CSL_mcbspHwSetup((CSL_McbspHandle)*hMcbsp1, &myHwCfg1);
  	CSL_mcbspHwSetup((CSL_McbspHandle)*hMcbsp3, &myHwCfg3);

	/* set multichannel mode registers */
	((CSL_McbspHandle)*hMcbsp1)->regs->MCR1 = 0x0201;
	((CSL_McbspHandle)*hMcbsp1)->regs->MCR2 = 0x0201;

	((CSL_McbspHandle)*hMcbsp3)->regs->MCR1 = 0x0201;
	((CSL_McbspHandle)*hMcbsp3)->regs->MCR2 = 0x0201;
  
    /* McBSP3 channels enabled */
	((CSL_McbspHandle)*hMcbsp3)->regs->RCERA = 0x0003;	// receive channels 0 & 1
	((CSL_McbspHandle)*hMcbsp3)->regs->XCERA = 0x0018;	// transmit channels 3 & 4
  	/* McBSP1 channels enabled */
	((CSL_McbspHandle)*hMcbsp1)->regs->RCERA = 0x0018;	// receive channels 3 & 4
	((CSL_McbspHandle)*hMcbsp1)->regs->XCERA = 0x0003;	// transmit channels 0 & 1

}
