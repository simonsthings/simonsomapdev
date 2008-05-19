/** ============================================================================
 *  @file   loop.cmd
 *
 *  @path   $(DSPLINK)/dsp/src/samples/loop/DspBios/Davinci/
 *
 *  @desc   Application-specific linker command file.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

/* Include the generated linker command file (Specify path from the sample
 * base path).
 */
-lDspBios/Davinci/loopcfg.cmd

SECTIONS {
.csl_vect: {} > DDR
}