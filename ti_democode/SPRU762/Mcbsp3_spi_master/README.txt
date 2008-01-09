/** McBSP3 SPI Mode
*
* This code runs on the DSP side.  However, the gel file
* needs to be loaded on the ARM side.
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
* This example runs with CSL 3.0, and was run using an
* OMAP5912 OSK externally connected to an AIC33 device.
*/