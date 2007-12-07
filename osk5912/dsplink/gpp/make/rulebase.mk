#   ============================================================================
#   @file   rulebase.mk
#
#   @path   $(DSPLINK)\gpp\make
#
#   @desc   Defines rules for all make targetes exported by the system.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


ifndef RULEBASE_MK

define RULEBASE_MK
endef

#   ============================================================================
#   @rule   all
#
#   @desc   Make all targets and export them
#   ============================================================================
all     : preprocess dirs clean includes objects targets exports

#   ============================================================================
#   @rule   dirs
#
#   @desc   Make the necessary directory structure
#   ============================================================================
dirs    : dirinc dirdeb dirrel direxp

#   ============================================================================
#   @rule   clean
#
#   @desc   Clean the intermediate files generated during the make process
#   ============================================================================
clean   : cleandeb cleanrel

#   ============================================================================
#   @rule   objects
#
#   @desc   Make the objects from specified sources
#   ============================================================================
objects  : objdeb objrel

#   ============================================================================
#   @rule   targets
#
#   @desc   Make the target from specified objects
#   ============================================================================
targets  : trgdeb trgrel

#   ============================================================================
#   @rule   exports
#
#   @desc   Export specified targets from a component
#   ============================================================================
exports : expdeb exprel

#   ============================================================================
#   @rule   debug
#
#   @desc   Make all targets in DEBUG mode and export them
#   ============================================================================
debug   : preprocess dirinc dirdeb direxp cleandeb includes objdeb trgdeb expdeb

#   ============================================================================
#   @rule   release
#
#   @desc   Make all targets in RELEASE mode and export them
#   ============================================================================
release : preprocess dirinc dirrel direxp cleanrel includes objrel trgrel exprel

#   ============================================================================
#   @rule   preprocess
#
#   @desc   Perform the necessary pre-build steps.
#               1. Verify the build configuration for scalability.
#               2. Copy the required HAL files for build.
#   ============================================================================
preprocess :
	$(CMDECHO)$(CFG2C) GPP $(CONFIG_TXT) $(CONFIG_C)
	$(CMDECHO)$(COPY) $(DIR_HAL_SOURCE)$(DIRSEP)*.* $(DIR_HAL_TARGET) > $(TRASH)

endif   # ifndef RULEBASE_MK
