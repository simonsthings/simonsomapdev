#   ============================================================================
#   @file   osdefs.mk
#
#   @path   $(DSPLINK)\gpp\make\Linux
#
#   @desc   This makefile defines OS specific macros used by MAKE system.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


ifndef OSDEFS_MK

define OSDEFS_MK
endef


#   ============================================================================
#   If a specific distribution of the GPP OS is defined in build configuration,
#   try to load the variables specific to the distribution ...
#
#   Apart from setting the variables specific to distribution, it must set the
#   value of variable USE_DISTRIBUTION as 1.
#   ============================================================================
ifneq ("$(DISTRIBUTION)", "")
-include $(MAKEROOT)$(DIRSEP)$(GPPOS)$(DIRSEP)$(DISTRIBUTION).mk
endif


ifneq ("$(USE_DISTRIBUTION)", "1")
#   ============================================================================
#   Set the values of necessary variables to be used for the OS.
#   These values will be used if no specific distribution is specified.
#
#   Linux distribution from Montavista is assumed default
#   for setting the variables below.
#   ============================================================================

#   ----------------------------------------------------------------------------
#   Base directory for the GPP OS
#   ----------------------------------------------------------------------------
BASE_GPPOS      := $(HOME)/montavista/kernel/linux

#   ----------------------------------------------------------------------------
#   Base for code generation tools - compiler, linker, archiver etc.
#   ----------------------------------------------------------------------------
BASE_CGTOOLS    := /opt/montavista/previewkit/arm/v4t_le/bin

#   ----------------------------------------------------------------------------
#   Base directory for include files provided by GPP OS
#   ----------------------------------------------------------------------------
BASE_OSINC      := $(BASE_GPPOS)/include

OSINC_GENERIC   := $(BASE_OSINC)
OSINC_PLATFORM  := $(BASE_OSINC)

ifneq ("$(VARIANT)", "")
OSINC_VARIANT   := $(BASE_OSINC)
endif


#   ----------------------------------------------------------------------------
#   Base directory for libraries provided by GPP OS
#   ----------------------------------------------------------------------------
BASE_OSLIB      := /opt/montavista/previewkit/arm/v4t_le/lib

OSLIB_GENERIC   := $(BASE_OSLIB)
OSLIB_PLATFORM  := $(BASE_OSLIB)

ifneq ("$(VARIANT)", "")
OSLIB_VARIANT   := $(BASE_OSLIB)
endif


#   ============================================================================
#   COMPILER
#   ============================================================================

#   ----------------------------------------------------------------------------
#   Name of the compiler
#   ----------------------------------------------------------------------------
COMPILER        := $(BASE_CGTOOLS)/arm_v4t_le-gcc

#   ----------------------------------------------------------------------------
#   Command line switches used by the compiler
#
#   CC_SW_DEF       Command line defines
#   CC_SW_INC       Search path for header files
#   CC_SW_OBJ       Create object file
#   CC_SW_DEB       Include debug information
#   ----------------------------------------------------------------------------
CC_SW_DEF       := -D
CC_SW_INC       := -I
CC_SW_OBJ       := -o
CC_SW_DEB       := -g

#   ----------------------------------------------------------------------------
#   Standard flags for the compiler
#   ----------------------------------------------------------------------------
STD_CC_FLAGS    := -Wall -Wno-strict-aliasing -c

#   ----------------------------------------------------------------------------
#   Standard flags for the compiler when building an executable
#   ----------------------------------------------------------------------------
EXE_CC_FLAGS    := -O

#   ----------------------------------------------------------------------------
#   Flags for the compiler when building a driver
#   ----------------------------------------------------------------------------
DRV_CC_FLAGS    := -O2

#   ----------------------------------------------------------------------------
#   Flags for the compiler when building a library
#   ----------------------------------------------------------------------------
LIB_CC_FLAGS    := -O2

#   ----------------------------------------------------------------------------
#   Standard definitions for the compiler
#   ----------------------------------------------------------------------------
STD_CC_DEFNS    :=


#   ============================================================================
#   ARCHIVER
#   ============================================================================
ARCHIVER        := $(BASE_CGTOOLS)/arm_v4t_le-ar

#   ----------------------------------------------------------------------------
#   Standard flags for the archiver
#   ----------------------------------------------------------------------------
STD_AR_FLAGS    := r


#   ============================================================================
#   LINKER
#   ============================================================================
LINKER      := $(BASE_CGTOOLS)/arm_v4t_le-gcc

#   ----------------------------------------------------------------------------
#   Command line switches used by the linker
#
#   LD_SW_LIB       Search path for libraries
#   LD_SW_OUT       Output filename
#   LD_SW_RELOC     Generate relocateable output
#   ----------------------------------------------------------------------------
LD_SW_LIB       := -L
LD_SW_OUT       := -o
LD_SW_RELOC     := -r

#   ----------------------------------------------------------------------------
#   Standard flags for the linker
#   ----------------------------------------------------------------------------
STD_LD_FLAGS    := -lc

#   ----------------------------------------------------------------------------
#   Flags for the linker when building an executable
#   ----------------------------------------------------------------------------
EXE_LD_FLAGS    :=

#   ----------------------------------------------------------------------------
#   Flags for the linker when building a driver
#   ----------------------------------------------------------------------------
DRV_LD_FLAGS    := -nostartfiles -nodefaultlibs


endif   # ifeq ($(USE_DISTRIBUTION), 0)

endif   # ifndef OSDEFS_MK
