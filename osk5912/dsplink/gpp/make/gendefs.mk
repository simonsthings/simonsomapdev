#   ============================================================================
#   @file   gendefs.mk
#
#   @path   $(DSPLINK)\gpp\make
#
#   @desc   This makefile defines generic macros used by MAKE system.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


ifndef GENDEFS_MK

define GENDEFS_MK
endef


.SUFFIXES :
.SUFFIXES : .EXE .DLL .LIB .OBJ. ASM .C .CPP


#   ============================================================================
#   Define common directory names
#   ============================================================================

DIR_INCLUDE     := INCLUDE
DIR_EXPORT      := EXPORT

DIR_OBJECTS     := OBJ
DIR_TARGETS     := BIN

DIR_DEBUG       := DEBUG
DIR_RELEASE     := RELEASE


#   ============================================================================
#   Define common file names
#   ============================================================================

COMP_DETAILS    := COMPONENT
COMP_SOURCES    := SOURCES
COMP_OBJECTS    := OBJECTS
COMP_DEPENDS    := DEPENDS


#   ============================================================================
#   Directory hierarchy for the MAKE system
#   ============================================================================

PROJ_MAKE           := $(BASE_PROJ)$(DIRSEP)make

PROJ_MAK_GENERIC    := $(PROJ_MAKE)
PROJ_MAK_GPPOS      := $(PROJ_MAKE)$(DIRSEP)$(GPPOS)
PROJ_MAK_PLATFORM   := $(PROJ_MAKE)$(DIRSEP)$(GPPOS)$(DIRSEP)$(PLATFORM)

ifeq ("$(VARIANT)", "")
PROJ_MAK_VARIANT    :=
else
PROJ_MAK_VARIANT    := $(PROJ_MAK_PLATFORM)$(DIRSEP)$(VARIANT)
endif


#   ============================================================================
#   Directory hierarchy for global header files
#
#   The changes in directory hierarchy (if any) must be reflected in the rules
#   for making the same.
#   ============================================================================

PROJ_INCLUDE        := $(BASE_PROJ)$(DIRSEP)inc

PROJ_INC_GENERIC    := $(PROJ_INCLUDE)
PROJ_INC_GPPOS      := $(PROJ_INCLUDE)$(DIRSEP)$(GPPOS)
PROJ_INC_PLATFORM   := $(PROJ_INCLUDE)$(DIRSEP)$(GPPOS)$(DIRSEP)$(PLATFORM)

ifeq ("$(VARIANT)", "")
PROJ_INC_VARIANT    :=
else
PROJ_INC_VARIANT    := $(PROJ_INC_PLATFORM)$(DIRSEP)$(VARIANT)
endif


#   ============================================================================
#   Directory hierarchy for component header files
#
#   The changes in directory hierarchy (if any) must be reflected in the rules
#   for making the same.
#
#   These definitions are based on COMP_PATH defined in file $(COMP_DETAILS).
#   Note that the actual values will be calculated on actual usage
#   ============================================================================

COMP_INC_GENERIC    = $(COMP_PATH)
COMP_INC_GPPOS      = $(COMP_PATH)$(DIRSEP)$(GPPOS)
COMP_INC_PLATFORM   = $(COMP_PATH)$(DIRSEP)$(GPPOS)$(DIRSEP)$(PLATFORM)

ifeq ("$(VARIANT)", "")
COMP_INC_VARIANT    =
else
COMP_INC_VARIANT    = $(COMP_INC_PLATFORM)$(DIRSEP)$(VARIANT)
endif


#   ============================================================================
#   Directory hierarchy for exported headers & targets
#
#   The changes in directory heirarchy (if any) must be reflected in the rules
#   for making the directory structure.
#   ============================================================================

PROJ_EXPORT         := $(BASE_PROJ)$(DIRSEP)export

EXPORT_INC          := $(PROJ_EXPORT)$(DIRSEP)$(DIR_INCLUDE)

EXPORT_INC_GENERIC  := $(EXPORT_INC)
EXPORT_INC_GPPOS    := $(EXPORT_INC)$(DIRSEP)$(GPPOS)
EXPORT_INC_PLATFORM := $(EXPORT_INC)$(DIRSEP)$(GPPOS)$(DIRSEP)$(PLATFORM)

ifeq ("$(VARIANT)", "")
EXPORT_INC_VARIANT  :=
else
EXPORT_INC_VARIANT  := $(EXPORT_INC_PLATFORM)$(DIRSEP)$(VARIANT)
endif


EXPORT_BIN          := $(PROJ_EXPORT)$(DIRSEP)$(DIR_TARGETS)

EXPORT_BIN_GENERIC  := $(EXPORT_BIN)
EXPORT_BIN_GPPOS    := $(EXPORT_BIN)$(DIRSEP)$(GPPOS)
EXPORT_BIN_PLATFORM := $(EXPORT_BIN)$(DIRSEP)$(GPPOS)$(DIRSEP)$(PLATFORM)

ifneq ("$(VARIANT)", "")
EXPORT_BIN_VARIANT  :=
else
EXPORT_BIN_VARIANT  := $(EXPORT_BIN_PLATFORM)$(DIRSEP)$(VARIANT)
endif

ifeq ("$(VARIANT)", "")
EXPORT_BIN_DEB      := $(EXPORT_BIN_PLATFORM)$(DIRSEP)$(DIR_DEBUG)
EXPORT_BIN_REL      := $(EXPORT_BIN_PLATFORM)$(DIRSEP)$(DIR_RELEASE)
else
EXPORT_BIN_DEB      := $(EXPORT_BIN_VARIANT)$(DIRSEP)$(DIR_DEBUG)
EXPORT_BIN_REL      := $(EXPORT_BIN_VARIANT)$(DIRSEP)$(DIR_RELEASE)
endif


#   ============================================================================
#   Directory hierarchy for sources in a component
#   ============================================================================

SOURCE_GENERIC    := $(COMP_PATH)
SOURCE_GPPOS      := $(COMP_PATH)$(DIRSEP)$(GPPOS)
SOURCE_PLATFORM   := $(COMP_PATH)$(DIRSEP)$(GPPOS)$(DIRSEP)$(PLATFORM)

ifneq ("$(VARIANT)", "")
SOURCE_VARIANT    := $(COMP_PATH)$(DIRSEP)$(GPPOS)$(DIRSEP)$(PLATFORM)$(DIRSEP)$(VARIANT)
endif


#   ============================================================================
#   Directory hierarchy for generated objects and binaries
#
#   The changes in directory heirarchy (if any) must be reflected in the rules
#   for making the directory structure.
#   ============================================================================

PROJ_BUILD          := $(BASE_PROJ)$(DIRSEP)BUILD

BUILD_INCLUDE       := $(PROJ_BUILD)$(DIRSEP)$(DIR_INCLUDE)
BUILD_EXPORT        := $(PROJ_BUILD)$(DIRSEP)$(DIR_EXPORT)

BUILD_EXPORT_DEB    := $(BUILD_EXPORT)$(DIRSEP)$(DIR_DEBUG)
BUILD_EXPORT_REL    := $(BUILD_EXPORT)$(DIRSEP)$(DIR_RELEASE)

COMP_BUILD          := $(PROJ_BUILD)$(DIRSEP)$(COMP_NAME)

COMP_OBJECT         := $(COMP_BUILD)$(DIRSEP)$(DIR_OBJECTS)
COMP_BINARY         := $(COMP_BUILD)$(DIRSEP)$(DIR_TARGETS)

COMP_OBJECT_DEB     := $(COMP_OBJECT)$(DIRSEP)$(DIR_DEBUG)
COMP_OBJECT_REL     := $(COMP_OBJECT)$(DIRSEP)$(DIR_RELEASE)

COMP_BINARY_DEB     := $(COMP_BINARY)$(DIRSEP)$(DIR_DEBUG)
COMP_BINARY_REL     := $(COMP_BINARY)$(DIRSEP)$(DIR_RELEASE)


#   ============================================================================
#   Level of print(s) for the banner (Used in banner.pl)
#   ============================================================================

BANNER_L1   := 1
BANNER_L2   := 2


#   ============================================================================
#   Specify the configuration default build configuration file.
#   ============================================================================
DEFAULT_BUILD_CONFIG  := $(MAKEROOT)$(DIRSEP)config$(DIRSEP)LINK_FULL


#   ============================================================================
#   Specify the configuration file to be used for this build
#   ============================================================================
ifneq ("$(VARIANT)", "")
CONFIG_TXT  := $(DSPLINK)$(DIRSEP)config$(DIRSEP)all$(DIRSEP)CFG_$(PLATFORM)_$(VARIANT).TXT
CONFIG_C    := $(DSPLINK)$(DIRSEP)config$(DIRSEP)all$(DIRSEP)CFG_$(PLATFORM)_$(VARIANT).c
else
CONFIG_TXT  := $(DSPLINK)$(DIRSEP)config$(DIRSEP)all$(DIRSEP)CFG_$(PLATFORM).TXT
CONFIG_C    := $(DSPLINK)$(DIRSEP)config$(DIRSEP)all$(DIRSEP)CFG_$(PLATFORM).c
endif


#   ============================================================================
#   Specify the source & target directories for HAL sources
#   ============================================================================

DIR_HAL_SOURCE  := $(BASE_PROJ)$(DIRSEP)src$(DIRSEP)hal$(DIRSEP)$(PLATFORM)
DIR_HAL_TARGET  := $(BASE_PROJ)$(DIRSEP)src$(DIRSEP)ldrv$(DIRSEP)$(GPPOS)$(DIRSEP)$(PLATFORM)


#   ============================================================================
#   Configuration specific defines to be supplied to the compiler.
#   ============================================================================
CFG_CC_DEFNS :=

endif   # ifndef GENDEFS_MK
