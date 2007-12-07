#   ============================================================================
#   @file   link.mk
#
#   @path   $(DSPLINK)\gpp\make\Linux\OMAP
#
#   @desc   This makefile contains rules to link sources.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


ifndef LINK_MK

define LINK_MK
endef


include $(MAKEROOT)/$(GPPOS)/systools.mk
include $(MAKEROOT)/gendefs.mk
include $(MAKEROOT)/$(GPPOS)/osdefs.mk
include $(MAKEROOT)/rulebase.mk

include $(GPPOS)/$(COMP_DETAILS)


#   ============================================================================
#   Clear environment variables
#   ============================================================================

INCLUDE     :=
LIB         :=


#   ============================================================================
#   Search path for standard libraries
#   ============================================================================

STD_LIB_PATH    := $(addprefix -L, $(OSLIB_GENERIC))
STD_LIB_PATH    += $(addprefix -L, $(OSLIB_PLATFORM))

ifneq ("$(OSLIB_VARIANT)", "")
STD_LIB_PATH    += $(addprefix -L, $(OSLIB_VARIANT))
endif


#   ============================================================================
#   Rules to build targets
#   ============================================================================

target_deb  := $(addsuffix .deb, $(COMP_TARGET))
target_rel  := $(addsuffix .rel, $(COMP_TARGET))


#   ============================================================================
#   @rule   trgdeb
#
#   @desc   Builds the target for DEBUG variant.
#   ============================================================================

ifeq ("$(COMP_TYPE)", "LIB")
trgdeb  : libdeb
endif

ifeq ("$(COMP_TYPE)", "DRV")
trgdeb  : drvdeb
endif

ifeq ("$(COMP_TYPE)", "EXE")
trgdeb  : exedeb
endif


#   ============================================================================
#   @rule   trgrel
#
#   @desc   Builds the target for RELEASE variant.
#   ============================================================================

ifeq ("$(COMP_TYPE)", "LIB")
trgrel  : librel
endif

ifeq ("$(COMP_TYPE)", "DRV")
trgrel  : drvrel
endif

ifeq ("$(COMP_TYPE)", "EXE")
trgrel  : exerel
endif


#   ############################################################################
#   BUILD AN ARCHIVE
#   ############################################################################


ifeq ("$(COMP_TYPE)", "LIB")

#   ----------------------------------------------------------------------------
#   Name of the target archive
#   ----------------------------------------------------------------------------

ARCHIVE_DEB   = $(COMP_BINARY_DEB)/$(basename $(@))
ARCHIVE_REL   = $(COMP_BINARY_REL)/$(basename $(@))

#   ----------------------------------------------------------------------------
#   Append suitable path to each user defined library
#   ----------------------------------------------------------------------------

ifeq ("$(USR_LIBS)", "")

USR_LIBS_DEB    :=
USR_LIBS_REL    :=

DEPENDS_DEB     :=
DEPENDS_REL     :=

ARGS_ARCH_DEB   := $(COMP_OBJECT_DEB)
ARGS_ARCH_REL   := $(COMP_OBJECT_REL)

depend_deb      :=
depend_rel      :=

else

DEPENDS_DEB     := $(COMP_OBJECT_DEB)/DEPEND
DEPENDS_REL     := $(COMP_OBJECT_REL)/DEPEND

USR_LIBS_DEB    := $(addprefix $(BUILD_EXPORT_DEB)/, $(USR_LIBS))
USR_LIBS_REL    := $(addprefix $(BUILD_EXPORT_REL)/, $(USR_LIBS))

ARGS_ARCH_DEB   := $(COMP_OBJECT_DEB)     $(DEPENDS_DEB)
ARGS_ARCH_REL   := $(COMP_OBJECT_REL)     $(DEPENDS_REL)

depend_deb      := $(addsuffix .dep, $(USR_LIBS_DEB))
depend_rel      := $(addsuffix .dep, $(USR_LIBS_REL))

endif

CMD_ARCHIVE     := $(ARCHIVER) $(STD_AR_FLAGS)

#   ----------------------------------------------------------------------------
#   @rule   libdeb
#
#   @desc   Builds the target library for DEBUG variant.
#   ----------------------------------------------------------------------------

$(target_deb):
	$(CMDECHO)$(DISPLAY) Generating $(basename $@)...
	$(CMDECHO)$(CMD_ARCHIVE) $(ARCHIVE_DEB) $(foreach dir, $(ARGS_ARCH_DEB), $(wildcard $(dir)/*.o)) $(USR_LIBS_DEB)
	$(CMDECHO)$(COPY) $(ARCHIVE_DEB) $(BUILD_EXPORT_DEB)/.

libdeb  : $(target_deb)


#   ----------------------------------------------------------------------------
#   @rule   librel
#
#   @desc   Builds the target library for RELEASE variant.
#   ----------------------------------------------------------------------------

$(target_rel):
	$(CMDECHO)$(DISPLAY) Generating $(basename $@)...
	$(CMDECHO)$(CMD_ARCHIVE) $(ARCHIVE_REL) $(foreach dir, $(ARGS_ARCH_REL), $(wildcard $(dir)/*.o)) $(USR_LIBS_REL)
	$(CMDECHO)$(COPY) $(ARCHIVE_REL) $(BUILD_EXPORT_REL)/.

librel  : $(target_rel)


endif   # ifeq ("$(COMP_TYPE)", "LIB")


#   ############################################################################
#   BUILD A DRIVER MODULE
#   ############################################################################


ifeq ("$(COMP_TYPE)", "DRV")
#   ----------------------------------------------------------------------------
#   Name of the target driver module
#   ----------------------------------------------------------------------------
DRIVER_DEB   = $(COMP_BINARY_DEB)/$(basename $(@))
DRIVER_REL   = $(COMP_BINARY_REL)/$(basename $(@))

#   ----------------------------------------------------------------------------
#   Append suitable path to each user defined library
#   ----------------------------------------------------------------------------

ifeq ("$(USR_LIBS)", "")

USR_LIBS_DEB    :=
USR_LIBS_REL    :=

ARGS_ARCH_DEB   := $(COMP_OBJECT_DEB)
ARGS_ARCH_REL   := $(COMP_OBJECT_REL)

else

DEPENDS_DEB     := $(COMP_OBJECT_DEB)/DEPEND
DEPENDS_REL     := $(COMP_OBJECT_REL)/DEPEND

USR_LIBS_DEB    := $(addprefix $(BUILD_EXPORT_DEB)/, $(USR_LIBS))
USR_LIBS_REL    := $(addprefix $(BUILD_EXPORT_REL)/, $(USR_LIBS))

ARGS_ARCH_DEB   := $(COMP_OBJECT_DEB) $(DEPENDS_DEB)
ARGS_ARCH_REL   := $(COMP_OBJECT_REL) $(DEPENDS_REL)

endif

CMD_LINK        := $(LINKER) $(DRV_LD_FLAGS) $(LD_SW_RELOC)

#   ----------------------------------------------------------------------------
#   @rule   drvdeb
#
#   @desc   Builds the target driver module for DEBUG variant.
#   ----------------------------------------------------------------------------

$(target_deb):
	$(CMDECHO)$(DISPLAY) Generating $(basename $@)...
	$(CMDECHO)$(CMD_LINK) $(LD_SW_OUT)$(DRIVER_DEB) $(foreach dir, $(ARGS_ARCH_DEB), $(wildcard $(dir)/*.o)) $(USR_LIBS_DEB)
	$(CMDECHO)$(COPY) $(DRIVER_DEB) $(BUILD_EXPORT_DEB)/.

drvdeb  : $(target_deb)


#   ----------------------------------------------------------------------------
#   @rule   drvrel
#
#   @desc   Builds the target driver module for RELEASE variant.
#   ----------------------------------------------------------------------------

$(target_rel):
	$(CMDECHO)$(DISPLAY) Generating $(basename $@)...
	$(CMDECHO)$(CMD_LINK) $(LD_SW_OUT)$(DRIVER_REL) $(foreach dir, $(ARGS_ARCH_REL), $(wildcard $(dir)/*.o)) $(USR_LIBS_REL)
	$(CMDECHO)$(COPY) $(DRIVER_REL) $(BUILD_EXPORT_REL)/.

drvrel  : $(target_rel)


endif   # ifeq ("$(COMP_TYPE)", "DRV")


#   ############################################################################
#   BUILD AN EXECUTABLE
#   ############################################################################


ifeq ("$(COMP_TYPE)", "EXE")
#   ----------------------------------------------------------------------------
#   Name of the target executable
#   ----------------------------------------------------------------------------
EXECUTABLE_DEB   = $(COMP_BINARY_DEB)/$(basename $(@))
EXECUTABLE_REL   = $(COMP_BINARY_REL)/$(basename $(@))

#   ----------------------------------------------------------------------------
#   Append suitable path to each user defined library
#   ----------------------------------------------------------------------------

ifeq ("$(USR_LIBS)", "")

USR_LIBS_DEB  :=
USR_LIBS_REL  :=

else

DEPENDS_DEB   := $(COMP_OBJECT_DEB)/DEPEND
DEPENDS_REL   := $(COMP_OBJECT_REL)/DEPEND

USR_LIBS_DEB  := $(addprefix $(BUILD_EXPORT_DEB)/, $(USR_LIBS))
USR_LIBS_REL  := $(addprefix $(BUILD_EXPORT_REL)/, $(USR_LIBS))

ARGS_ARCH_DEB := $(COMP_OBJECT_DEB) $(DEPENDS_DEB)
ARGS_ARCH_REL := $(COMP_OBJECT_REL) $(DEPENDS_REL)

endif

CMD_LINK        := $(LINKER) $(EXE_LD_FLAGS) $(STD_LD_FLAGS) $(USR_LD_FLAGS) $(STD_LIB_PATH)

#   ----------------------------------------------------------------------------
#   @rule   exedeb
#
#   @desc   Builds the target executable for DEBUG variant.
#   ----------------------------------------------------------------------------

$(target_deb):
	$(CMDECHO)$(DISPLAY) Generating $(basename $@)...
	$(CMDECHO)$(CMD_LINK) $(LD_SW_OUT)$(EXECUTABLE_DEB) $(foreach dir, $(ARGS_ARCH_DEB), $(wildcard $(dir)/*.o)) $(USR_LIBS_DEB)
	$(CMDECHO)$(COPY) $(EXECUTABLE_DEB) $(BUILD_EXPORT_DEB)/.

exedeb  : $(target_deb)


#   ----------------------------------------------------------------------------
#   @rule   exerel
#
#   @desc   Builds the target executable for RELEASE variant.
#   ----------------------------------------------------------------------------

$(target_rel):
	$(CMDECHO)$(DISPLAY) Generating $(basename $@)...
	$(CMDECHO)$(CMD_LINK) $(LD_SW_OUT)$(EXECUTABLE_REL) $(foreach dir, $(ARGS_ARCH_REL), $(wildcard $(dir)/*.o)) $(USR_LIBS_REL)
	$(CMDECHO)$(COPY) $(EXECUTABLE_REL) $(BUILD_EXPORT_REL)/.

exerel  : $(target_rel)


endif   # ifeq ("$(COMP_TYPE)", "EXE")

endif   # ifndef LINK_MK
