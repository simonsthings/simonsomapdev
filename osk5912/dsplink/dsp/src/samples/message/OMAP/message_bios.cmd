/** ============================================================================
 *  @file   message_bios.cmd
 *
 *  @path   $(DSPLINK)\dsp\src\samples\message\OMAP
 *
 *  @desc   Application-specific linker command file.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

/* Include the generated linker command file */
-lmessage_bioscfg.cmd

SECTIONS {
    /* Force the dsplink section into internal memory */
    .text:dsplink:    {}  > SARAM
}
