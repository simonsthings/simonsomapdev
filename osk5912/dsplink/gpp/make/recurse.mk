#   ============================================================================
#   @file   recurse.mk
#
#   @path   $(DSPLINK)\gpp\make
#
#   @desc   The makefile builds all components in a given directory.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


ifndef RECURSE_MK

define RECURSE_MK
endef

include $(MAKEROOT)$(DIRSEP)$(GPPOS)$(DIRSEP)systools.mk
include $(MAKEROOT)$(DIRSEP)gendefs.mk
include $(MAKEROOT)$(DIRSEP)$(GPPOS)$(DIRSEP)osdefs.mk
include $(MAKEROOT)$(DIRSEP)rulebase.mk


#   ============================================================================
#   Declare global PHONY targets
#   ============================================================================

.PHONY:     dirinc dirdeb dirrel direxp
.PHONY:     cleandeb cleanrel clobber
.PHONY:     includes
.PHONY:     objdeb objrel
.PHONY:     trgdeb trgrel
.PHONY:     expdeb exprel


#   ============================================================================
#   Declare local PHONY targets (for recursion)
#   ============================================================================

r_dirinc    := $(addsuffix .dirinc,   $(DIRS))

r_dirdeb    := $(addsuffix .dirdeb,   $(DIRS))
r_dirrel    := $(addsuffix .dirrel,   $(DIRS))
r_direxp    := $(addsuffix .direxp,   $(DIRS))

r_includes  := $(addsuffix .includes, $(DIRS))

r_cleandeb  := $(addsuffix .cleandeb, $(DIRS))
r_cleanrel  := $(addsuffix .cleanrel, $(DIRS))
r_clobber   := $(addsuffix .clobber,  $(DIRS))

r_objdeb    := $(addsuffix .objdeb,   $(DIRS))
r_objrel    := $(addsuffix .objrel,   $(DIRS))

r_trgdeb    := $(addsuffix .trgdeb,   $(DIRS))
r_trgrel    := $(addsuffix .trgrel,   $(DIRS))

r_expdeb    := $(addsuffix .expdeb,   $(DIRS))
r_exprel    := $(addsuffix .exprel,   $(DIRS))


.PHONY:     $(r_dirinc)
.PHONY:     $(r_dirdeb)   $(r_dirrel)   $(r_direxp)
.PHONY:     $(r_includes)
.PHONY:     $(r_cleandeb) $(r_cleanrel) $(r_clobber)
.PHONY:     $(r_objdeb)   $(r_objrel)
.PHONY:     $(r_trgdeb)   $(r_trgrel)
.PHONY:     $(r_expdeb)   $(r_exprel)

#   ============================================================================
#   Rules to create directory structure
#   ============================================================================

#   ----------------------------------------------------------------------------
#   @rule   dirinc
#
#   @desc   Creates the directory for storing header files
#           exported from a component
#   ----------------------------------------------------------------------------

dirinc      : b_dirinc $(r_dirinc)

b_dirinc    :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) DIRS INCLUDE

$(r_dirinc) :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ----------------------------------------------------------------------------
#   @rule   dirdeb
#
#   @desc   Creates the directories for storing generated object and target
#           files for the DEBUG build variant.
#   ----------------------------------------------------------------------------

dirdeb      : b_dirdeb $(r_dirdeb)

b_dirdeb    :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) DIRS INCLUDE DEBUG

$(r_dirdeb) :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ----------------------------------------------------------------------------
#   @rule   dirrel
#
#   @desc   Creates the directories for storing generated object and target
#           files for the RELEASE build variant.
#   ----------------------------------------------------------------------------

dirrel      : b_dirrel $(r_dirrel)

b_dirrel    :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) DIRS INCLUDE RELEASE

$(r_dirrel) :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ----------------------------------------------------------------------------
#   @rule   direxp
#
#   @desc   Creates the directories for storing object and target files
#           exported from a component.
#   ----------------------------------------------------------------------------

direxp      : b_direxp $(r_direxp)

b_direxp    :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) DIRS EXPORT

$(r_direxp) :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ============================================================================
#   Rules to clean the generated objects and targets
#   ============================================================================

#   ----------------------------------------------------------------------------
#   @rule   cleandeb
#
#   @desc   Cleans the file generated during the build for DEBUG variant.
#   ----------------------------------------------------------------------------

cleandeb    : b_cleandeb $(r_cleandeb)

b_cleandeb  :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) CLEAN DEBUG

$(r_cleandeb)   :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ----------------------------------------------------------------------------
#   @rule   cleanrel
#
#   @desc   Cleans the file generated during the build for RELEASE variant.
#   ----------------------------------------------------------------------------

cleanrel    : b_cleanrel $(r_cleanrel)

b_cleanrel  :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) CLEAN RELEASE

$(r_cleanrel)   :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ----------------------------------------------------------------------------
#   @rule   clobber
#
#   @desc   Deletes the directories created during the build process.
#   ----------------------------------------------------------------------------

clobber     : b_clobber $(r_clobber)

b_clobber   :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) CLOBBER

$(r_clobber)    :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))
	$(CMDECHO)$(RUNIF) "exist $(PROJ_BUILD)" "$(RMTREE) $(PROJ_BUILD)" ""


#   ============================================================================
#   Rules to export header files from a component
#   ============================================================================

#   ----------------------------------------------------------------------------
#   @rule   includes
#
#   @desc   Exports header files from a component
#   ----------------------------------------------------------------------------

includes    : b_includes $(r_includes)

b_includes  :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) INCLUDES

$(r_includes)   :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ============================================================================
#   Rules to build object files.
#   ============================================================================

#   ----------------------------------------------------------------------------
#   @rule   objdeb
#
#   @desc   Builds the object files for DEBUG variant.
#   ----------------------------------------------------------------------------

objdeb      : b_objdeb $(r_objdeb)

b_objdeb    :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) OBJECTS DEBUG

$(r_objdeb) :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ----------------------------------------------------------------------------
#   @rule   objrel
#
#   @desc   Builds the object files for RELEASE variant.
#   ----------------------------------------------------------------------------

objrel      : b_objrel $(r_objrel)

b_objrel    :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) OBJECTS RELEASE

$(r_objrel) :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ============================================================================
#   Rules to build target files.
#   ============================================================================

#   ----------------------------------------------------------------------------
#   @rule   trgdeb
#
#   @desc   Builds the target files for DEBUG variant.
#   ----------------------------------------------------------------------------

trgdeb      : b_trgdeb $(r_trgdeb)

b_trgdeb    :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) TARGETS DEBUG

$(r_trgdeb) :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ----------------------------------------------------------------------------
#   @rule   trgrel
#
#   @desc   Builds the target files for RELEASE variant.
#   ----------------------------------------------------------------------------

trgrel      : b_trgrel $(r_trgrel)

b_trgrel    :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) TARGETS RELEASE

$(r_trgrel) :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ============================================================================
#   Rules to export target files.
#   ============================================================================

#   ----------------------------------------------------------------------------
#   @rule   expdeb
#
#   @desc   Builds the target files for DEBUG variant.
#   ----------------------------------------------------------------------------

expdeb      : b_expdeb $(r_expdeb)

b_expdeb    :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) EXPORTS DEBUG

$(r_expdeb) :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


#   ----------------------------------------------------------------------------
#   @rule   exprel
#
#   @desc   Builds the target files for RELEASE variant.
#   ----------------------------------------------------------------------------

exprel      : b_exprel $(r_exprel)

b_exprel    :
	$(CMDHIDE)$(BANNER) $(BANNER_L1) $(DIR_NAME) EXPORTS RELEASE

$(r_exprel) :
	$(CMDECHO)$(MAKE) -f Makefile -C $(basename $@) $(subst ., , $(suffix $@))


endif   # ifndef RECURSE_MK
