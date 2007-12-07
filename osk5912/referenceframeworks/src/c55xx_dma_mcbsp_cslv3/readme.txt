TITLE
-----
C55XX_DMA_MCBSP_CSLV3 (IOM driver for C55XX McBSPs)

THIS DRIVER USES CHIP SUPPRT LIBRARY (CSL) V3.X

USAGE
-----
This device driver is a mini-driver for the McBSP on the C55XX, specially
modified for the Innovator 5910 and OSK 5912, to transfer data to and from the
serial ports. 

DESCRIPTION
-----------
C55XX_DMA_MCBSP is an IOM driver for the C55XX on the INNOVATOR 5910 and the 
OSK 5912. The driver is implemented using DMA. 

FILES
-----
- c55xx_dma_mcbsp_cslv3.c: implementation of McBSP driver 

- referenceframework/include/c55xx_dma_mcbsp_cslv3.h: 
	generic driver header file

- c5910_dma_mcbsp_cslv3.pjt: project file for building the driver library 
	for large model on the 5910 platform. 

- c5910_dma_mcbsp_cslv3.c: chip-specific wrapper for the McBSP driver on 5910

- c5912_dma_mcbsp_cslv3.pjt: project file for building the driver library 
	for large model on the 5912 platform. 

- c5912_dma_mcbsp_cslv3.c: chip-specific wrapper for the McBSP driver on 5912

- readme.txt: this file

NOTES
-----
- Files in the library are compiled with "no optimization switch" turned on, 
  and the same is true for the project files. If you plan to use this module 
  in a project release, you should rebuild the library with optimization 
  turned on.

Known Problems At Release
-------------------------
- None.

Q&A
---
Q1: Where can I find the documentation for writing IOM drivers?

---
Q1: Where can I find the documentation for writing IOM drivers?
A1: The literature number is SPRU616, DSP/BIOS Device Driver Developer's Guide.

