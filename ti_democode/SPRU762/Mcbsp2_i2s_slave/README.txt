/** McBSP2 interfaced to Two Codecs with Independent External
* Transmit and Receive Rates
*
* This software runs on the ARM side.  The gel file needs to be
* loaded on the ARM side as well.
*
* McBSP2 is a slave for both reception and transmission, and
* operates in I2S mode.  The implementation has a software check
* routine in the ISRs, which should be replaced by the actual 
* application software.
*
* N represents the number of audio samples per buffer.  The
* array size is twice the number of samples to account for left
* and right channels for a given audio sample.
*
* McBSP2 needs to be released from reset during the inactive 
* state of the FSX and FSR signal to prevent data misalignment.
* This is accomplished by detecting the first frame sync using
* the McBSP MPU interrupt, and then using the GPIO capability
* of the McBSP to detect the inactive state.  In this case, since
* FSX/RP=1, the inactive state of FSX/R is high.
*
* Once the McBSP transmitter and receiver are taken out of reset,
* the DMA services the event requests using the following buffering
* scheme.
*
* The implementation uses a ping-pong buffering scheme that 
* takes advantage of the half-frame and end of block DMA interrupts.
* For this implementation, the DMA autoinitializes after a
* block as been transferred.  Ping and pong are placed in 
* consecutive locations in memory so that the DMA can be programmed
* to transfer data for a total of ping + pong samples, hence
* the half-frame interrupt indicating that ping has been transferred
* and the end of block interrupt to indicate that pong has 
* been transferred:
* 
* Memory contents:		DMA interrupt
* srcBufPing[1]
* srcBufPing[2]
* srcBufPing[3]
* ...
* srcBufPing[N*2-2]
* srcBufPing[N*2-1]		half-frame
* srcBufPong[1]
* srcBufPong[2]
* ...
* srcBufPong[N*2-2]
* srcBufPong[N*2-1]		end of block
* 
* This example runs with CSL 3.0.
*
* This code was ran on the OMAP5912 OSK, externally connected to two
* AIC33 codecs.
*
*/
