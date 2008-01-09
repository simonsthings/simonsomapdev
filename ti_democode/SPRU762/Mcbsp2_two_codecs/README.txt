/** McBSP2 interfaced to two codecs
*
* This software runson the DSP side.  However, the gel file needs to
* be loaded on the ARM side.
*
* McBSP2 is a master for transmission and a slave for reception.  
* The implementation has a software check
* routine in the ISRs, which should be replaced by the actual 
* application software.
*
* N represents the number of audio samples per buffer.  The
* array size is twice the number of samples to account for left
* and right channels for a given audio sample.
*
* Each channel is 16-bits, and each frame is 4 channels long to accomodate
* the two stereo codecs.
*
* Once the McBSP transmitter and receiver are taken out of reset,
* the System DMA services the event requests using the following buffering
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
* src_ping[1]
* src_ping[2]
* src_ping[3]
* ...
* src_ping[N*2-2]
* src_ping[N*2-1]		half-frame
* src_pong[1]
* src_pong[2]
* ...
* src_pong[N*2-2]
* src_pong[N*2-1]		end of block
* 
* This example runs with CSL 3.0.  The code was run on the OMAP5912 OSK,
* externally connected to two AIC33 codecs.
*/