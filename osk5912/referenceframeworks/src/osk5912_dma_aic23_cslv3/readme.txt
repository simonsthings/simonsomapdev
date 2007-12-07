TITLE
-----
OSK5912_DMA_AIC23_CSLV3 (IOM driver for OSK5912/AIC23 codec)

THIS DRIVER USES CHIP SUPPRT LIBRARY (CSL) V3.X

USAGE
-----
This device driver is a mini-driver for the codec on the OSK5912 
and uses the generic C5912_DMA_MCBSP driver to transfer data to and from the 
serial ports. Because this driver is dependent on the generic C5912_DMA_MCBSP 
driver, an application must link both the libraries from this driver and from 
the generic driver in order for it to function correctly. These two libraries 
are called osk5912_dma_aic23_cslv3.l55l and c5912_dma_mcbsp_cslv3.l55l which
are located in referenceframework/lib directory.

DESCRIPTION
-----------
OSK5912AIC23 is an IOM driver for the OSK 5912.  This board has 
an AIC23 stereo codec on it. The driver is implemented using DMA. 

FILES
-----
- osk5912_dma_aic23_cslv3.pjt: project file for building the driver library 
	for large model on the osk5912 platform. 

- osk5912_dma_aic23_cslv3.c: initialization of McBSP and DMA 

- referenceframework/include/osk5912_dma_aic23_cslv3.h: 
	driver interface header file

- referenceframework/include/c55xx_dma_mcbsp_cslv3.h: 
	generic driver header file

- readme.txt: this file

NOTES
-----
- Files in the library are compiled with "no optimization switch" turned on, 
  and the same is true for the project files. If you plan to use this module 
  in a project release, you should rebuild the library with optimization 
  turned on.

- All buffers provided to the driver must be from the correct memory segment. 
  As default, all the buffers provided to the driver are from the DARAM memory
  segment. If you want to put all the buffers at the SARAM or EMIF memory 
  segment, you should specify the dmaPortType field from the device structure.
  The application won't work if all the buffers provided to the driver stay at
  the wrong memory segment. 

Known Problems At Release
-------------------------
- None.

Q&A
---
Q1: How do I use the default DMA channels? 
Q2: How do I change the DMA channels?
Q3: Where can I find the documentation for writing IOM drivers?

---
Q1: How do I use the default codec parameters and default DMA channels? 
A1: Specify a NULL value to the User-Defined Device parameter when you add the 
    driver from the Configuration Tools. The default DMA channels are 4 and 5 
    which will be plugged into hwi14 and hwi15. The default DMA port type 
    is DARAM. 

Q2: How do I change the DMA channels?
A2: There are two ways to change DMA channels:

    1) Static way:
       -----------
       You need to define the device structure with 
       all the fields before main().
       
       Example1: Default parameters and different DMA channels.

	   OSK5912_DMA_AIC23_CSLV3_DevParams myParams = {
	       OSK5912_DMA_AIC23_CSLV3_VERSION_2
	       2, 
	       3, 
	       OSK5912_DMA_AIC23_CSLV3_PORTTYPE_DEFAULT,
	       { 
               OSK5912_DMA_AIC23_CSLV3_IER_MASK_DEFAULT,
		   OSK5912_DMA_AIC23_CSLV3_IER_MASK_DEFAULT 
	       }, 
	       { 
		   OSK5912_DMA_AIC23_CSLV3_IER_MASK_DEFAULT,
		   OSK5912_DMA_AIC23_CSLV3_IER_MASK_DEFAULT 
	        } 
	   };

	   Pass "_myParams" to the User-Defined Device parameter.
       
    2) Dynamic way:
       ------------
       You need to define a function which will just change some particular
       fields. This function will be implemented in another file called
       initParams.c which will be added into your application. 

       Example1: Default parameters and different DMA channels.

           OSK5912_DMA_AIC23_CSLV3_DevParams myParams = 
               OSK5912_DMA_AIC23_CSLV3_DEVPARAMS_DEFAULT; 
	   void initDevParams()
	   {
	       myParams.rxDmaId = 2;
	       myParams.txDmaId = 3;
           }
	   
	   Within DSP/BIOS Configuration Tools:
	   - Pass "_myParams" to the User-Defined Device parameter.
	   - Pass "_initDevParams" to the User Init Function from the Global 
	     Settings. 

       This example shows how to use default codec and DMA channel
       2 and 3. DMA channel 2 and 3 will be plugged into the correct interrupt.

Q3: Where can I find the documentation for writing IOM drivers?
A3: The literature number is SPRU616, DSP/BIOS Device Driver Developer's Guide.

