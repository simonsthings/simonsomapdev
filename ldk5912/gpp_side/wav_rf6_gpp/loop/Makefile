#   ============================================================================
#   @file   Makefile
#
#   @path   $(DSPLINK)/dsp/src/samples/loop/
#
#   @desc   This file is a standard interface to the make scripts.
#           Usually no change is required in this file.
#
#   @ver    01.30.06
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2005
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


#   ============================================================================
#   Set the device type (GPP/DSP)
#   ============================================================================

export DEVICETYPE := DSP


#   ============================================================================
#   Get the directory separator used on the development host.
#   ============================================================================

ifneq ("$(ComSpec)", "")
DIRSEP ?=\\
else
DIRSEP ?= /
endif


#   ============================================================================
#   Start the build process
#   ============================================================================

include $(DSPLINK)$(DIRSEP)make$(DIRSEP)start.mk
