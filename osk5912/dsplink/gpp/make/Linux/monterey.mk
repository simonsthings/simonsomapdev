#   ============================================================================
#   @file   monterey.mk
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


ifndef MONTEREY_MK

define MONTEREY_MK
endef


#   ============================================================================
#   Let the make system know that a specific distribution for the GPP OS
#   is being used.
#   ============================================================================
USE_DISTRIBUTION := 1


#   ============================================================================
#   Base directory for the GPP OS
#   ============================================================================
BASE_GPPOS      := $(HOME)/PigeonPoint/ML/arm925t/usr/src/linux


#   ============================================================================
#   Base for code generation tools - compiler, linker, archiver etc.
#   ============================================================================
BASE_CGTOOLS    := $(HOME)/PigeonPoint/ML/usr/bin


#   ============================================================================
#   Base directory for include files provided by GPP OS
#   ============================================================================
BASE_OSINC      := $(BASE_GPPOS)/include

OSINC_GENERIC   := $(BASE_OSINC)
OSINC_PLATFORM  := $(BASE_OSINC)

ifneq ("$(VARIANT)", "")
OSINC_VARIANT   := $(BASE_OSINC)
endif


#   ============================================================================
#   Base directory for libraries provided by GPP OS
#   ============================================================================
BASE_OSLIB      := $(BASE_GPPOS)/usr/lib

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
COMPILER        := $(BASE_CGTOOLS)/arm925t-gcc

CROSS_COMPILE   := arm925t
export CROSS_COMPILE

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
#   Flags for the compiler when building an executable
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
ARCHIVER        := $(BASE_CGTOOLS)/arm925t-ar

#   ----------------------------------------------------------------------------
#   Standard flags for the archiver
#   ----------------------------------------------------------------------------
STD_AR_FLAGS    := r


#   ============================================================================
#   LINKER
#   ============================================================================
LINKER      := $(BASE_CGTOOLS)/arm925t-gcc

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


endif   # ifndef MONTEREY_MK
