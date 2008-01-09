/** McBSP1 in a TDM data highway in multichannel mode
*
* This code is ran on the DSP.  The gel file needs to be loaded on
* the ARM side, however.
*
* McBSP1 is a master for transmission and a slave for reception.  
* The implementation has a software check
* routine in the ISRs, which should be replaced by the actual 
* application software.  McBSP1 transmits on channels 0 and 1, and
* receives on channels 3 and 4.
*
* N represents the number of audio samples per buffer.  The
* array size is twice the number of samples to account for left
* and right channels for a given audio sample.
*
* This particular example also uses McBSP3 as a multichannel device
* on the same TDM data highway.  McBSP3 is a slave transmitter and
* receiver.  It has transmits channels 3 and 4 and receives channels
* 0 and 1.
*
* Neither McBSP transfers on channels 2 and 5 to prevent data
* contention between multichannel devices.
*
* McBSP3 needs to be released from reset during the inactive 
* state of the FSX signal to prevent data misalignment.
* This is accomplished by detecting the first frame sync using
* the McBSP DSP interrupt.  In this case, since
* FSX/RP=0, the inactive state of FSX/R is low.  This can be
* accomplished since the frame sync width is 1-bit so the duty cycle
* can be enough to release the transmitter in the inactive state.
*
* Each channel is 16-bits, and each frame is 6 channels long to accomodate
* the 4 transmission channels and 2 dummy channels.
*
* Once the McBSP transmitter and receiver are taken out of reset,
* the DSP DMA services the event requests using the following buffering
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
* This buffering scheme is implemented for both McBSP1 and McBSP3.
*
* This example runs with CSL 3.0.
*
* Furthermore, this example was tested on the OMAP5912 OSK board
*
*/