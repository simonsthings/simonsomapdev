# Generated by Code Composer Studio

# Makefile for W:\Work\project data\Inomed\OMAP5912 Infos\OMAP5912 McBSP Reference - spru762c\SPRU762\Mcbsp2_two_codecs\TWO_AIC33s.pjt
# This makefile is intended to be compatible with any version of make.
#
# USAGE
# Step 1: Set up proper environment variables, by running DosRun.bat
#         (Windows platform) or set them up by yourself.
# Step 2: Change directory to the directory of this makefile, which is stored
#         in the macro MAKEFILE_DIR
# Step 3: In the makefile directory, you can perform one of these operations:
#         To build the configuration <config>, type make <config>
#         To clean the configuration <config>, type make <config>clean
#         To rebuild the configuration <config>, type make <config>rebuild
#         If <config> is not specified, the operations apply to the active
#         configuration, which is stored in the macro ACTIVE_CONFIG.
# For CCS gmake users: In the makefile directory, you can perform one of these operations:
#         To build the configuration <config>, type gmake projectName.mak <config>
#         To clean the configuration <config>, type gmake <config>clean
#         To rebuild the configuration <config>, type gmake <config>rebuild
#         If <config> is not specified, the operations apply to the active
#         configuration, which is stored in the macro ACTIVE_CONFIG.
#
# DETAILS
# Step 1:
#   In order for this makefile to work, you must have the environment variables
# set properly.  Specifically, the paths to build programs and the system-wide
# header files and library files must be set.
#   In Windows, a batch file "DosRun.bat", located at the root directory of
# the Code Composer Studio installation, is provided to set up these variables.
#   In UNIX, you can set up these environment variables yourself.  The "PATH"
# variable must include the paths to the build programs, and the "C_DIR" and
# "A_DIR" variables must include the paths to the system-wide header files and
# library files.
#
# Step 2:
#   System files referenced by this project are specified in full path names,
# while other files in this project are specified in path names relative to the
# directory of this makefile.  These directory names are stored in DIR_x macros.
# You can modify them to reflect the locations of the files on this system.
#   It is important that the command to make this makefile is issued from the
# directory of this makefile, which is stored in the macro MAKEFILE_DIR.
#
# Step 3:
#   There are three operations that can be performed on a build configuration:
# make, clean, and rebuild. A rebuild forces all files in the configuration to
# be rebuilt.
# To build the configuration <config>, type make <config>
# To clean the configuration <config>, type make <config>clean
# To rebuild the configuration <config>, type make <config>rebuild
#   If <config> is not specified, the operations apply to the active
# configuration. The active configuration can be any valid build configuration
# (including "all") and is specified in the macro ACTIVE_CONFIG.
# To build the active configuration, type make
# To clean the active configuration, type make clean
# To rebuild the active configuration, type make rebuild
#
# For CCS gmake users:
#   There are three operations that can be performed on a build configuration:
# make, clean, and rebuild. A rebuild forces all files in the configuration to
# be rebuilt.
# To build the configuration <config>, type gmake projectName.mak <config>
# To clean the configuration <config>, type gmake <config>clean
# To rebuild the configuration <config>, type gmake <config>rebuild
#   If <config> is not specified, the operations apply to the active
# configuration. The active configuration can be any valid build configuration
# (including "all") and is specified in the macro ACTIVE_CONFIG.
# To build the active configuration, type gmake
# To clean the active configuration, type gmake clean
# To rebuild the active configuration, type gmake rebuild

# The MKFLAGS macro takes the place of the MFLAGS or MAKEFLAGS macro. If it
# gives you any trouble, just delete the macro. It is used when recursively
# calling make (i.e. when rebuilding)
MKFLAGS= $(MFLAGS)$(MAKEFLAGS)

MAKEFILE_DIR= W:/Work/project data/Inomed/OMAP5912 Infos/OMAP5912 McBSP Reference - spru762c/SPRU762/Mcbsp2_two_codecs

# The active configuration can be one of these valid build configurations:
# all, Debug
ACTIVE_CONFIG= Debug

# These DIR_x macros store the directories of the files used in this project.
# There must be no trailing spaces after these macros.

DIR_1= Debug

DIR_2= C:/CCStudio_v3.1/C5500/cgtools/bin

DIR_3= C:/CCStudio_v3.1/csl_OMAP5912/dsp/inc

DIR_4= C:/CCStudio_v3.1/csl_OMAP5912/dsp/lib

SOURCE=$(DIR_4)/csl_OMAP5912.lib
SOURCE=cvectors.asm
SOURCE=main.c
SOURCE=lnk.cmd

active_config: $(ACTIVE_CONFIG)

norecurse: $(ACTIVE_CONFIG)_norecurse

clean: $(ACTIVE_CONFIG)clean

clean_norecurse: $(ACTIVE_CONFIG)clean_norecurse

rebuild: $(ACTIVE_CONFIG)rebuild

force_rebuild: 

all: Debug 

allclean: Debugclean 

allrebuild: Debugrebuild 


Debugclean: Debugclean_norecurse

Debugclean_norecurse: 
	-@rm -f $(DIR_1)/cvectors.obj
	-@rm -f $(DIR_1)/main.obj
	-@rm -f $(DIR_1)/mcbsp2_2codecs.out

Debugrebuild: 
	$(MAKE) $(MFLAGS) -f TWO_AIC33s.mak Debug FRC=force_rebuild

Debug: Debug_norecurse

Debug_norecurse: $(DIR_1)/mcbsp2_2codecs.out



$(DIR_1)/cvectors.obj: $(FRC) cvectors.asm 
	"$(DIR_2)/cl55" -g -as -fr"./Debug" -i"C:/CCStudio_v3.1/csl_OMAP5912/dsp/inc" -d"_DEBUG" -d"CSL_CHIP_5905PG1_0" -d"CSL_CHIP_5912PG1_0" -ml -v5510:2 "cvectors.asm" 

$(DIR_1)/main.obj: $(FRC) main.c $(DIR_3)/csl.h $(DIR_3)/csl_chip.h $(DIR_3)/csl_dma.h $(DIR_3)/csl_error.h $(DIR_3)/csl_intc.h $(DIR_3)/csl_mcbsp.h $(DIR_3)/csl_ssw.h $(DIR_3)/soc.h $(DIR_3)/csl_types.h $(DIR_3)/cslr.h $(DIR_3)/cslr_chip.h $(DIR_3)/cslr_dma.h $(DIR_3)/cslr_intc20.h $(DIR_3)/cslr_intc21.h $(DIR_3)/cslr_mcbsp.h $(DIR_3)/cslr_ssw.h $(DIR_3)/tistdtypes.h 
	"$(DIR_2)/cl55" -g -as -fr"./Debug" -i"C:/CCStudio_v3.1/csl_OMAP5912/dsp/inc" -d"_DEBUG" -d"CSL_CHIP_5905PG1_0" -d"CSL_CHIP_5912PG1_0" -ml -v5510:2 "main.c" 

$(DIR_1)/mcbsp2_2codecs.out: $(FRC) $(DIR_1)/cvectors.obj $(DIR_1)/main.obj $(DIR_4)/csl_OMAP5912.lib lnk.cmd 
	-@echo -z -q -c -m"./Debug/mcbsp2_2codecs.map" -o"./Debug/mcbsp2_2codecs.out" -x -l"rts55x.lib"> TWO_AIC33s.Debug.lkf
	-@echo "$(DIR_1)/cvectors.obj">> TWO_AIC33s.Debug.lkf
	-@echo "$(DIR_1)/main.obj">> TWO_AIC33s.Debug.lkf
	-@echo "$(DIR_4)/csl_OMAP5912.lib">> TWO_AIC33s.Debug.lkf
	-@echo "lnk.cmd">> TWO_AIC33s.Debug.lkf
	"$(DIR_2)/cl55" -@"TWO_AIC33s.Debug.lkf"
	-@rm -f TWO_AIC33s.Debug.lkf