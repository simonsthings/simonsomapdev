
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










--------------------------------------------------------------------
Printed from folder "C:\SPRU762\Mcbsp3_spi_master" .
See spru762c.pdf (OMAP5912 Multimedia Processor Multichannel Buffered Serial Ports (McBSPs)) for more explanation.

The following pages are collected together from different pdf files from Texas Instruments. Each page should have the originating pdf document stated at the bottom (where the omap5912.pdf seems to have the internal name SPRS231E).
The two layout pages at the end are from a document called spru715.pdf (OSK5912 Board Design Guide).

I put these pages together to give you a hint on how to find the correct OSK connector pins for a certain port. I was looking for information about how to connect an external A/D-Converter to the OSK5912 trough the 4 large connecters (CONN A, CONN B, CONN C, CONN D) on the back. I wanted to use McBSP3, but there seemed to be only one dedicated pin for this among the OSK connectors.

I have found out that many output pins of the OMAP5912 can have different signals on them, depending on the state of certain "Multiplexing Control Registers". The gel file on the next page sets these in the function "configure_nmp_muxes()". There, the pins are specified which carry the McBSP3 signals for the "McBSP3 SPI Mode" demo program of spru762.zip.

Cheers,
Simon