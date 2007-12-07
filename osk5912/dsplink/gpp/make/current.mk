#   ============================================================================
#   @file   current.mk
#
#   @path   $(DSPLINK)\gpp\make
#
#   @desc   This makefile builds a specific component.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


ifndef CURRENT_MK

define CURRENT_MK
endef

include $(MAKEROOT)$(DIRSEP)$(GPPOS)$(DIRSEP)systools.mk
include $(MAKEROOT)$(DIRSEP)gendefs.mk
include $(MAKEROOT)$(DIRSEP)$(GPPOS)$(DIRSEP)osdefs.mk
include $(MAKEROOT)$(DIRSEP)rulebase.mk


#   ============================================================================
#   Rules to create directory structure
#   ============================================================================

dir_build   := $(PROJ_BUILD)

dir_comp    := $(COMP_BUILD)
dir_comp    += $(COMP_OBJECT)
dir_comp    += $(COMP_BINARY)


$(dir_build) :
	$(CMDECHO)$(RUNIF) "exist $(@)" "" "$(CMDECHO)$(MKDIR) $(@)"

$(dir_comp) : $(dir_build)
	$(CMDECHO)$(RUNIF) "exist $(@)" "" "$(CMDECHO)$(MKDIR) $(@)"


#   ----------------------------------------------------------------------------
#   @rule   dirinc
#
#   @desc   Creates the directories for storing header files
#           exported from a component
#   ----------------------------------------------------------------------------

dir_export_inc  := $(EXPORT_INC_GENERIC)
dir_export_inc  += $(EXPORT_INC_GPPOS)
dir_export_inc  += $(EXPORT_INC_PLATFORM)

ifneq ("$(VARIANT)", "")
dir_export_inc  += $(EXPORT_INC_VARIANT)
endif

dir_build_inc   := $(BUILD_INCLUDE)


dirincbanner :
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) DIRS INCLUDE

$(dir_export_inc) :
	$(CMDECHO)$(RUNIF) "exist $(@)" ""  "$(CMDECHO)$(MKDIR) $(@)"

$(dir_build_inc) : $(dir_build)
	$(CMDECHO)$(RUNIF) "exist $(@)" "" "$(CMDECHO)$(MKDIR) $(@)"


dirinc  : dirincbanner $(dir_export_inc) $(dir_build_inc)


#   ----------------------------------------------------------------------------
#   @rule   dirdeb
#
#   @desc   Creates the directories for storing generated object and target
#           files for the DEBUG build variant.
#   ----------------------------------------------------------------------------

dir_comp_deb    := $(COMP_OBJECT_DEB)
dir_comp_deb    += $(COMP_BINARY_DEB)


dirdebbanner :
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) DIRS DEBUG

$(dir_comp_deb) : $(dir_comp)
	$(CMDECHO)$(RUNIF) "exist $(@)" "" "$(CMDECHO)$(MKDIR) $(@)"


dirdeb  : dirdebbanner $(dir_export_inc) $(dir_comp_deb)


#   ----------------------------------------------------------------------------
#   @rule   dirrel
#
#   @desc   Creates the directories for storing generated object and target
#           files for the RELEASE build variant.
#   ----------------------------------------------------------------------------

dir_comp_rel := $(COMP_OBJECT_REL)
dir_comp_rel += $(COMP_BINARY_REL)


dirrelbanner :
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) DIRS RELEASE

$(dir_comp_rel) :  $(dir_comp)
	$(CMDECHO)$(RUNIF) "exist $(@)" "" "$(CMDECHO)$(MKDIR) $(@)"


dirrel  : dirrelbanner $(dir_export_inc) $(dir_comp_rel)


#   ----------------------------------------------------------------------------
#   @rule   direxp
#
#   @desc   Creates the directories for storing object and target files
#           exported from a component.
#   ----------------------------------------------------------------------------

dir_export_bin  := $(EXPORT_BIN_GENERIC)
dir_export_bin  += $(EXPORT_BIN_GPPOS)
dir_export_bin  += $(EXPORT_BIN_PLATFORM)

ifneq ("$(VARIANT)", "")
dir_export_bin  += $(EXPORT_BIN_VARIANT)
endif

dir_export_bin  += $(EXPORT_BIN_DEB)
dir_export_bin  += $(EXPORT_BIN_REL)


dir_build_exp   := $(BUILD_EXPORT)
dir_build_exp   += $(BUILD_EXPORT_DEB)
dir_build_exp   += $(BUILD_EXPORT_REL)


direxpbanner :
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) DIRS EXPORT

$(dir_export_bin) :
	$(CMDECHO)$(RUNIF) "exist $(@)" "" "$(CMDECHO)$(MKDIR) $(@)"

$(dir_build_exp) : $(dir_build)
	$(CMDECHO)$(RUNIF) "exist $(@)" "" "$(CMDECHO)$(MKDIR) $(@)"


direxp  : direxpbanner $(dir_export_bin) $(dir_build_exp)


#   ============================================================================
#   Rules to clean the generated objects and targets
#   ============================================================================

#   ----------------------------------------------------------------------------
#   @rule   cleandeb
#
#   @desc   Cleans the file generated during the build for DEBUG variant.
#   ----------------------------------------------------------------------------

cleandeb:
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) CLEAN DEBUG
	$(CMDECHO)$(RUNIF) "exist $(COMP_OBJECT_DEB)" "$(DELETE) $(COMP_OBJECT_DEB)$(DIRSEP)*.*" "" >$(TRASH)


#   ----------------------------------------------------------------------------
#   @rule   cleanrel
#
#   @desc   Cleans the file generated during the build for RELEASE variant.
#   ----------------------------------------------------------------------------

cleanrel:
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) CLEAN RELEASE
	$(CMDECHO)$(RUNIF) "exist $(COMP_OBJECT_REL)" "$(DELETE) $(COMP_OBJECT_REL)$(DIRSEP)*.*" "" >$(TRASH)


#   ----------------------------------------------------------------------------
#   @rule   clobber
#
#   @desc   Deletes the directories created during the build process.
#   ----------------------------------------------------------------------------

clobber :
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) CLOBBER
	$(CMDECHO)$(RUNIF) "exist $(COMP_BUILD)" "$(RMTREE) $(COMP_BUILD)" ""


#   ============================================================================
#   Rules to export header files from a component
#   ============================================================================

#   ----------------------------------------------------------------------------
#   @rule   includes
#
#   @desc   Exports header files from a component
#   ----------------------------------------------------------------------------

.PHONY: $(EXP_HEADERS)


incbanner:
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) INCLUDES

$(EXP_HEADERS):
	$(CMDECHO)$(COPY) $(COMP_PATH)$(DIRSEP)$(@) $(BUILD_INCLUDE)$(DIRSEP). >$(TRASH)


includes: incbanner dirinc $(EXP_HEADERS)


#   ============================================================================
#   Rules to build object files.
#   ============================================================================

CMD_MAKEOBJ_GENERIC  = \
	$(RUNIF) "exist $(SOURCE_GENERIC)" \
	"$(MAKE) -C $(SOURCE_GENERIC) -f  $(PROJ_MAK_PLATFORM)$(DIRSEP)compile.mk     \
	$@ VERBOSE=$(VERBOSE)" ""

CMD_MAKEOBJ_GPPOS    = \
	$(RUNIF) "exist $(SOURCE_GPPOS)" \
	"$(MAKE) -C $(SOURCE_GPPOS) -f    $(PROJ_MAK_PLATFORM)$(DIRSEP)compile.mk     \
	$@ VERBOSE=$(VERBOSE)" ""

CMD_MAKEOBJ_PLATFORM = \
	$(RUNIF) "exist $(SOURCE_PLATFORM)" \
	"$(MAKE) -C $(SOURCE_PLATFORM) -f $(PROJ_MAK_PLATFORM)$(DIRSEP)compile.mk     \
	$@ VERBOSE=$(VERBOSE)" ""

ifneq ("$(VARIANT)", "")
CMD_OBJ_VARIANT  = \
	$(RUNIF) "exist $(SOURCE_VARIANT)" \
	"$(MAKE) -C $(SOURCE_VARIANT) -f  $(PROJ_MAK_PLATFORM)$(DIRSEP)compile.mk     \
	$@ VERBOSE=$(VERBOSE)" ""
endif


#   ----------------------------------------------------------------------------
#   @rule   objdeb
#
#   @desc   Builds the object files for DEBUG variant.
#   ----------------------------------------------------------------------------

objdeb  : dirdeb
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) OBJECT DEBUG
	$(CMDECHO)$(CMD_MAKEOBJ_GENERIC)
	$(CMDECHO)$(CMD_MAKEOBJ_GPPOS)
	$(CMDECHO)$(CMD_MAKEOBJ_PLATFORM)
ifneq ("$(VARIANT)", "")
	$(CMD_OBJ_VARIANT)
endif


#   ----------------------------------------------------------------------------
#   @rule   objrel
#
#   @desc   Builds the object files for RELEASE variant.
#   ----------------------------------------------------------------------------

objrel  : dirrel
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) OBJECT RELEASE
	$(CMDECHO)$(CMD_MAKEOBJ_GENERIC)
	$(CMDECHO)$(CMD_MAKEOBJ_GPPOS)
	$(CMDECHO)$(CMD_MAKEOBJ_PLATFORM)
ifneq ("$(VARIANT)", "")
	$(CMD_MAKEOBJ_VARIANT)
endif


#   ============================================================================
#   Rules to build target files.
#   ============================================================================

#   ----------------------------------------------------------------------------
#   @rule   trgdeb
#
#   @desc   Builds the target files for DEBUG variant.
#   ----------------------------------------------------------------------------

trgdeb  :
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) TARGET DEBUG
	$(MAKE) -f  $(PROJ_MAK_PLATFORM)$(DIRSEP)link.mk $@ VERBOSE=$(VERBOSE)

#   ----------------------------------------------------------------------------
#   @rule   trgrel
#
#   @desc   Builds the target files for RELEASE variant.
#   ----------------------------------------------------------------------------

trgrel  :
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) TARGET RELEASE
	$(MAKE) -f  $(PROJ_MAK_PLATFORM)$(DIRSEP)link.mk $@ VERBOSE=$(VERBOSE)

#   ============================================================================
#   Rules to export target files.
#   ============================================================================

export_deb  := $(addsuffix .deb, $(EXP_TARGETS))
export_rel  := $(addsuffix .rel, $(EXP_TARGETS))


.PHONY: $(export_deb) $(export_deb)


$(export_deb):
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) EXPORT DEBUG
	$(CMDECHO)$(COPY) $(BUILD_EXPORT_DEB)$(DIRSEP)$(basename $@)                  \
	$(EXPORT_BIN_DEB)$(DIRSEP). >$(TRASH)

$(export_rel):
	$(CMDHIDE)$(BANNER) $(BANNER_L2) $(COMP_NAME) EXPORT RELEASE
	$(CMDECHO)$(COPY) $(BUILD_EXPORT_REL)$(DIRSEP)$(basename $@)                  \
	$(EXPORT_BIN_REL)$(DIRSEP). >$(TRASH)


#   ----------------------------------------------------------------------------
#   @rule   expdeb
#
#   @desc   Builds the target files for DEBUG variant.
#   ----------------------------------------------------------------------------

expdeb  : $(export_deb)


#   ----------------------------------------------------------------------------
#   @rule   exprel
#
#   @desc   Builds the target files for RELEASE variant.
#   ----------------------------------------------------------------------------

exprel  : $(export_rel)


endif   # ifndef CURRENT_MK
