#   ============================================================================
#   @file   compile.mk
#
#   @path   $(DSPLINK)\gpp\make\Linux\OMAP
#
#   @desc   This makefile contains rules to compile sources.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


ifndef COMPILE_MK

define COMPILE_MK
endef


include $(MAKEROOT)/$(GPPOS)/systools.mk
include $(MAKEROOT)/gendefs.mk
include $(MAKEROOT)/$(GPPOS)/osdefs.mk
include $(MAKEROOT)/rulebase.mk

-include $(COMP_SOURCES)
-include ../$(COMP_DETAILS)
-include $(GPPOS)/$(COMP_DETAILS)
-include $(COMP_DETAILS)


#   ============================================================================
#   Clear environment variables so the don't interfere in the build process.
#   ============================================================================

INCLUDE     :=
LIB         :=
C_OPTION    :=
C_DIR       :=
A_DIR       :=


#   ============================================================================
#   Standard include search path
#   ============================================================================

#   ----------------------------------------------------------------------------
#   Header files with global scope
#   ----------------------------------------------------------------------------
STD_INC_PATH    := $(CC_SW_INC)$(PROJ_INC_GENERIC)
STD_INC_PATH    += $(CC_SW_INC)$(PROJ_INC_GPPOS)
STD_INC_PATH    += $(CC_SW_INC)$(PROJ_INC_PLATFORM)

ifneq ("$(VARIANT)", "")
STD_INC_PATH    += $(CC_SW_INC)$(PROJ_INC_VARIANT)
endif

#   ----------------------------------------------------------------------------
#   Header files with component scope
#   ----------------------------------------------------------------------------
STD_INC_PATH    += $(CC_SW_INC)$(COMP_INC_GENERIC)
STD_INC_PATH    += $(CC_SW_INC)$(COMP_INC_GPPOS)
STD_INC_PATH    += $(CC_SW_INC)$(COMP_INC_PLATFORM)

ifneq ("$(VARIANT)", "")
STD_INC_PATH    += $(CC_SW_INC)$(COMP_INC_VARIANT)
endif

#   ----------------------------------------------------------------------------
#   Header files from the target operating system
#   ----------------------------------------------------------------------------
STD_INC_PATH    += $(CC_SW_INC)$(OSINC_GENERIC)
STD_INC_PATH    += $(CC_SW_INC)$(OSINC_PLATFORM)

ifneq ("$(VARIANT)", "")
STD_INC_PATH    += $(CC_SW_INC)$(OSINC_VARIANT)
endif

STD_INC_PATH    += $(CC_SW_INC)$(BUILD_INCLUDE)


#   ============================================================================
#   Standard flags for the compiler
#   ============================================================================
STD_CC_FLAGS    += $(STD_INC_PATH)

ifeq ("$(COMP_TYPE)", "EXE")
STD_CC_FLAGS    += $(EXE_CC_FLAGS)
endif   # ifeq ("$(COMP_TYPE)", "EXE")

ifeq ("$(COMP_TYPE)", "DRV")
STD_CC_FLAGS    += $(DRV_CC_FLAGS)
endif   # ifeq ("$(COMP_TYPE)", "DRV")

ifeq ("$(COMP_TYPE)", "LIB")
STD_CC_FLAGS    += $(LIB_CC_FLAGS)
endif   # ifeq ("$(COMP_TYPE)", "LIB")


#   ============================================================================
#   Standard definitions for the compiler
#   ============================================================================
STD_CC_DEFNS    += $(CC_SW_DEF)OS_LINUX

ifeq ("$(TRACE)", "1")
STD_CC_DEFNS += $(CC_SW_DEF)TRACE_ENABLE
endif

ifeq ("$(PROFILE)", "1")
STD_CC_DEFNS += $(CC_SW_DEF)DDSP_PROFILE
endif

ifeq ("$(PROFILE)", "2")
STD_CC_DEFNS += $(CC_SW_DEF)DDSP_PROFILE_DETAILED $(CC_SW_DEF)DDSP_PROFILE
endif


#   ============================================================================
#   Compile time scalability options
#   ============================================================================

ifeq ($(USE_MSGQ), 1)
STD_CC_DEFNS += $(CC_SW_DEF)MSGQ_COMPONENT
endif

ifeq ($(USE_CHNL), 1)
STD_CC_DEFNS += $(CC_SW_DEF)CHNL_COMPONENT
endif

ifeq ($(USE_PROC), 1)
STD_CC_DEFNS += $(CC_SW_DEF)PROC_COMPONENT
endif

ifeq ($(USE_DSP), 1)
STD_CC_DEFNS += $(CC_SW_DEF)DSP_COMPONENT
endif


#   ============================================================================
#   Standard way of invoking the compiler
#   ============================================================================

COMPILER_DEB     = $(COMPILER) $(CC_SW_DEB) \
				   $(STD_CC_FLAGS) $(USR_CC_FLAGS) \
				   $(STD_CC_DEFNS) $(USR_CC_DEFNS) -DDDSP_DEBUG \
				   $(CC_SW_OBJ)$(COMP_OBJECT_DEB)/$(basename $(basename $@)).o


COMPILER_REL     = $(COMPILER) \
				   $(STD_CC_FLAGS) $(USR_CC_FLAGS) \
				   $(STD_CC_DEFNS) $(USR_CC_DEFNS) \
				   $(CC_SW_OBJ)$(COMP_OBJECT_REL)/$(basename $(basename $@)).o

#   ============================================================================
#   Get the C file containing configuration information.
#   ============================================================================

ifeq ("$(CONFIG)", "")
source_cfg :=
else
source_cfg := $(CONFIG)

.PHONY: $(source_cfg)

$(source_cfg): $(CONFIG_C)
	$(CMDECHO)$(DISPLAY) Fetching the configuration file [$(@)]...
ifeq ("$(VARIANT)", "")
	$(CMDECHO)$(COPY) $(CONFIG_C) $(COMP_PATH)/$(GPPOS)/$(PLATFORM)/$(CONFIG)
else
	$(CMDECHO)$(COPY) $(CONFIG_C) $(COMP_PATH)/$(GPPOS)/$(PLATFORM)/$(VARIANT)/$(CONFIG)
endif

endif


#   ============================================================================
#   Rules to compile sources
#   ============================================================================

sources_deb := $(addsuffix .deb, $(SOURCES))
sources_rel := $(addsuffix .rel, $(SOURCES))

.PHONY: $(sources_deb) $(sources_rel)

#   ----------------------------------------------------------------------------
#   @rule   objdeb
#
#   @desc   Builds the object files for DEBUG variant.
#   ----------------------------------------------------------------------------

objdeb  : $(source_cfg) $(sources_deb)

$(sources_deb):
	$(CMDECHO)$(DISPLAY) Compiling $(basename $@)...
	$(CMDECHO)$(COMPILER_DEB) $(basename $@)

#   ----------------------------------------------------------------------------
#   @rule   objrel
#
#   @desc   Builds the object files for RELEASE variant.
#   ----------------------------------------------------------------------------

objrel  : $(source_cfg) $(sources_rel)

$(sources_rel):
	$(CMDECHO)$(DISPLAY) Compiling $(basename $@)...
	$(CMDECHO)$(COMPILER_REL) $(basename $@)


endif   # ifndef COMPILE_MK
