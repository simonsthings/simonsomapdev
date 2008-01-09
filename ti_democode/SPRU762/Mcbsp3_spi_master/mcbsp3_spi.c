/*
 * Copyright (C) 2004 Texas Instruments Incorporated
 * All Rights Reserved
 */
/** McBSP3 SPI Mode
*
* This example illustrates how to use McBSP3 in clock-stop
* mode to interface to a SPI device.  The McBSP is serviced
* by polling the XRDY and RRDY bits, and acts as the master.
*
* The data_write routine writes a 16-bit word to the slave
* by polling XRDY to begin transmission.  It also reads the
* received word in DRR1.
*
* The data_read routine reads a 16-bit word which holds
* relevant data in the lower 8-bits only.
* 
* This example runs with CSL 3.0.
*/
 
#include <stdio.h>

//#include <csl.h>
#include <csl_error.h>
#include <csl_intc.h>
#include <csl_chip.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>
#include <soc.h>

#define N 0x20

void data_write(int command, char write_value);
char data_read(int command);

/*-------Macro that gives 2 CLK delay cycles---------*/
/* This delay depends on the actual DSP frequency and CLKX/R rate */
#define WAIT_FOR_2_CLK  do {                                   \
                               volatile int delayCnt = 2;      \
                               while(delayCnt > 0) --delayCnt; \
                           } while (0)
                           
                           

/*---------Global data definition---------*/
CSL_McbspGlobalSetup gblCfg = {
    CSL_MCBSP_IOMODE_TXDIS_RXDIS,	// pins are McBSP pins, not GPIO
    CSL_MCBSP_DLBMODE_OFF,			// disable Digital Loopback
    CSL_MCBSP_CLKSTP_WITHOUT_DELAY	// SPI mode: clk stop enabled
};									// CLKSTP = 10b

CSL_McbspDataSetup rcvDataCfg = {
    CSL_MCBSP_PHASE_SINGLE,			// SPI mode: single phase only
    CSL_MCBSP_WORDLEN_16,			// AIC33: 8 bits ctr, 8 bits data
    1, 								// SPI mode: length=1 only
    CSL_MCBSP_WORDLEN_8, 			// Default
    1, 								// SPI mode: only one phase is relevant
    CSL_MCBSP_FRMSYNC_DETECT,		// Default
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST,// No companding, MSB first
    CSL_MCBSP_DATADELAY_1_BIT,		// Data delay of 1 only
    CSL_MCBSP_RJUSTDXENA_DXENA_OFF,	// Default
    CSL_MCBSP_INTMODE_ON_READY		// Default
};

CSL_McbspDataSetup xmtDataCfg = {
    CSL_MCBSP_PHASE_SINGLE,			// SPI mode: single phase only
    CSL_MCBSP_WORDLEN_16,			// AIC33: 8 bits ctr, 8 bits data
    1, 								// SPI mode: length=1 only
    CSL_MCBSP_WORDLEN_8,			// Default
    1, 								// Default
    CSL_MCBSP_FRMSYNC_DETECT,		// Default
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST,// No companding, MSB first
    CSL_MCBSP_DATADELAY_1_BIT,		// Data delay of 1 only
    CSL_MCBSP_RJUSTDXENA_DXENA_OFF,	// Default
    CSL_MCBSP_INTMODE_ON_READY		// Default
};

CSL_McbspClkSetup clkCfg = {
    CSL_MCBSP_FSCLKMODE_INTERNAL, 	// FSX is an output
    CSL_MCBSP_FSCLKMODE_EXTERNAL,  	// FSR-- default
    CSL_MCBSP_TXRXCLKMODE_INTERNAL,	// CLKX is an output
    CSL_MCBSP_TXRXCLKMODE_EXTERNAL, // CLKR-- default
    CSL_MCBSP_FSPOL_ACTIVE_LOW,   	// FSX active low
    CSL_MCBSP_FSPOL_ACTIVE_HIGH,	// FSR-- default
    CSL_MCBSP_CLKPOL_TX_RISING_EDGE,// CLKXP = 0
    CSL_MCBSP_CLKPOL_RX_FALLING_EDGE,//CLKRP = 0
    1,  							// FS pulse width is a don't care
    31, 							// FS pulse period is a don't care
    30,  							// CLKGDIV = 30 means divide by input clock by 31
    CSL_MCBSP_SRGCLK_CLKCPU, 		// Use internal clock source
    CSL_MCBSP_CLKPOL_SRG_RISING_EDGE, // Default
    CSL_MCBSP_TXFSMODE_DXRCOPY, 	// FSX when DXR to XSR copy occurs
    CSL_MCBSP_CLKGSYNCMODE_OFF 		// GSYNC = 0 means no clock synchronization
};

CSL_McbspMulChSetup mulChCfg = CSL_MCBSP_MULTICHAN_DEFAULTS;

CSL_McbspHwSetup myHwCfg = {
    &gblCfg,
    &rcvDataCfg,
    &xmtDataCfg,
    &clkCfg,
    &mulChCfg,
    CSL_MCBSP_EMU_STOP,
    NULL
};


CSL_Status status;
CSL_McbspHandle hMcbsp3;
CSL_McbspObj myMcbspObj;

/** Source location */
int src[80];
/** Destination location */
char dst[80];

/*---------main routine---------*/
/** 
 * main sets up the McBSP and makes the function call to
 * read and write from the McBSP DXR1 and DRR1 registers.
*/
int main (void)
{
	int k;
		
	CSL_BitMask16 ctrlMask;

    /* Initialize CSL library, this step is required */
    CSL_mcbspInit(NULL);
    
  /* Open MCBSP Port, this will return a MCBSP handle that will */
  /* be used in calls to other CSL functions                    */
  hMcbsp3 = CSL_mcbspOpen(&myMcbspObj, CSL_MCBSP_3,
                         NULL, &status);

  if ((hMcbsp3 == NULL) || (status != CSL_SOK)) {
      printf ("\nError opening CSL_MCBSP_3");
      exit(1);
  }

  /* Put SRG, Frame-sync, XMT and RCV in reset */
  ctrlMask =   CSL_MCBSP_CTRL_SRG_DISABLE
             | CSL_MCBSP_CTRL_FSYNC_DISABLE
             | CSL_MCBSP_CTRL_TX_DISABLE
             | CSL_MCBSP_CTRL_RX_DISABLE;
  CSL_mcbspHwControl(hMcbsp3, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);

  /* Program MCBSP control registers and wait for 2 clock cycles */
  CSL_mcbspHwSetup(hMcbsp3, &myHwCfg);
  WAIT_FOR_2_CLK;

  /* Start Sample Rate Generator and wait for 2 clock cycles */
  ctrlMask = CSL_MCBSP_CTRL_SRG_ENABLE | CSL_MCBSP_CTRL_FSYNC_ENABLE;
  CSL_mcbspHwControl(hMcbsp3, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
  WAIT_FOR_2_CLK;
		
	        for(k=0; k < N*2; k++)   
	        {    
	        	src[k] = ~(2*k+1); 
	        	dst[k] = 0xDEAD;
	        }

  		/* Enable MCBSP transmit and receive */
  		ctrlMask = CSL_MCBSP_CTRL_TX_ENABLE | CSL_MCBSP_CTRL_RX_ENABLE;
  		CSL_mcbspHwControl(hMcbsp3, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
  		

do{
		data_write(7, 0x8A);	// write 0x8A to register 7
		dst[7] = data_read(7);	// read back the value of register 7
}while(dst[7] != 0x8A);			// check that value is written correctly
		

		// We are done with MCBSP, so close it
        CSL_mcbspClose(hMcbsp3);
	
    return 0;
}

void data_write(int address, char write_value){

    Uint16 response = 0;
    int dummy_data;
	
	address = (address << 9) | (write_value);
	address &= (int)0xFEFF;	// byte-LSB R/W bit is set to 1 for a write

    /* SEND COMMAND: W/R + ADDRESS */
    /* Wait for XRDY signal before writing data to DXR */
    while (!(response & CSL_MCBSP_XRDY)) {
        CSL_mcbspGetHwStatus(hMcbsp3, CSL_MCBSP_QUERY_DEV_STATUS, &response);
    }
    /* Write 8 bit data value to DXR */
    CSL_mcbspWrite (hMcbsp3, CSL_MCBSP_WORDLEN_16, &address);


    /* Dummy Read of DRR1 */
    /* Wait for RRDY signal to read data from DRR */
    response = 0;
    while (!(response & CSL_MCBSP_RRDY)) {
        CSL_mcbspGetHwStatus(hMcbsp3, CSL_MCBSP_QUERY_DEV_STATUS, &response);
    }
    CSL_mcbspRead (hMcbsp3, CSL_MCBSP_WORDLEN_16, &dummy_data);


}


char data_read(int address){
    
    int value_read = 0x0BAD;
    Uint16 response = 0;
    
    address = (int)((address << 1) | (address << 9));
    address |= (int)0x0101;	// byte-LSB R/W bit is cleared for a read
    
    /* SEND COMMAND: R/W + ADDRESS */
    /* Wait for XRDY signal before writing data to DXR */
    while (!(response & CSL_MCBSP_XRDY)) {
        CSL_mcbspGetHwStatus(hMcbsp3, CSL_MCBSP_QUERY_DEV_STATUS, &response);
    }
    /* Write 8 bit data value to DXR */
    CSL_mcbspWrite (hMcbsp3, CSL_MCBSP_WORDLEN_16, &address);

	/* Dummy Read for old data on DRR1 */	
    /* Wait for RRDY signal to read data from DRR */
    response = 0;
    while (!(response & CSL_MCBSP_RRDY)) {
        CSL_mcbspGetHwStatus(hMcbsp3, CSL_MCBSP_QUERY_DEV_STATUS, &response);
    }
    CSL_mcbspRead (hMcbsp3, CSL_MCBSP_WORDLEN_16, &value_read);

    return ((char)value_read & (char)(0x00FF));
}

