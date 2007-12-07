#!/bin/bash -f
#   ============================================================================
#   @file   gppenv.bash
#
#   @path   $(DSPLINK)/etc/host/scripts/linux
#
#   @desc   Sets the necessary environment for GPP side development for
#           DSP/BIOS(TM) Link
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================

#   ----------------------------------------------------------------------------
#   @name   DSPLINK
#
#   @desc   Root directory of DSP/BIOS LINK.
#   ----------------------------------------------------------------------------
export DSPLINK=$HOME/dsplink

#   ----------------------------------------------------------------------------
#   @name   PATH
#
#   @desc   Appends the path to the host scripts
#   ----------------------------------------------------------------------------
export PATH=$PATH:$DSPLINK/etc/host/scripts/Linux
