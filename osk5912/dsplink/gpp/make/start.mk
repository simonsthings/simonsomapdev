#   ============================================================================
#   @file   start.mk
#
#   @path   $(DSPLINK)\gpp\make
#
#   @desc   The makefile starts the process of building a component.
#           It defines all the commonly used targets for the 'make' system.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


#   ============================================================================
#   Export the directory separator
#   ============================================================================
export DIRSEP

include $(DSPLINK)$(DIRSEP)config$(DIRSEP)CURRENTCFG.MK


#   ============================================================================
#   Set the root for this 'make' system
#   ============================================================================
export MAKEROOT := $(DSPLINK)$(DIRSEP)gpp$(DIRSEP)make


#   ----------------------------------------------------------------------------
#   Include the appropriate DIRS file (if it exists).
#   It contains list of sub-directories to be considered for build.
#   ----------------------------------------------------------------------------

-include .$(DIRSEP)DIRS


#   ============================================================================
#   Select the appropriate make file to set things rolling...
#   ============================================================================
ifeq ("$(DIRS)", "")

#   ----------------------------------------------------------------------------
#   Include the appropriate COMPONENT file.
#   It contains necessary information to build the component.
#   ----------------------------------------------------------------------------

include .$(DIRSEP)$(GPPOS)$(DIRSEP)COMPONENT

export COMP_NAME
export COMP_PATH

include $(MAKEROOT)$(DIRSEP)current.mk

else

include $(MAKEROOT)$(DIRSEP)recurse.mk

endif
