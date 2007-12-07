/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appResources.h ========
 *
 *  Declaration of appllication-wide constants, global variables, macros,
 *  definition of data structures etc.
 *  This file should be included by every module in the application,
 *  so that all crucial parameters (channels, frame length) is visible
 *  to every module.
 *  This file also includes "appBiosObjects.h" header file which declares all 
 *  relevant BIOS objects defined in the BIOS config and makes it visible to
 *  all modules.
 */
#ifndef APPRESOURCES_
#define APPRESOURCES_

#include "appBiosObjects.h"     /* declaration of BIOS objects */

#ifdef __cplusplus
extern "C" {
#endif

/* general constants, global variables and macro definitions */

/* 
 *  Number of channels in the application. On a mono codec, identical
 *  data frame is copied to be used as input for all channels. With
 *  a physical N-channel input (N=2 for stereo codecs), channels process
 *  different data.
 */
#define NUMCHANNELS  2

/*
 *  Length of one channel data frame, in samples. 
 *  NOTE: pipes' frame sizes in the configuration database must be 
 *  changed if this value is changed.
 */
#define FRAMELEN    80

/* 
 *  Data type holding one sample: 16 bits in this case. FRAMELEN above
 *  is the size, in these units, of one data frame processed by one channel.
 */
typedef Short Sample;   /* Short is 16 bits on every CXX platform */

/* 
 *  Handy macros for converting size in samples to size in words and 
 *  vice versa -- pipe frame sizes are expressed in words (Ints), which 
 *  may not be the same as size in samples. (On a C5X an Int is 16bit wide,
 *  on a C6X an Int is 32bit wide, and we have samples that are 16bit wide.
 */
#define sizeInSamples( bufsizeInWords ) \
    ( (bufsizeInWords) * sizeof( Int ) / sizeof( Sample ) )
#define sizeInWords( bufsizeInSamples ) \
    ( (bufsizeInSamples) * sizeof( Sample ) / sizeof( Int ) )

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* APPRESOURCES_ */

