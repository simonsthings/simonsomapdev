#   ============================================================================
#   @file   systools.mk
#
#   @path   $(DSPLINK)\gpp\make\Linux
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


ifndef SYSTOOLS_MK

define SYSTOOLS_MK
endef

#   ============================================================================
#   Base directory for the project
#   ============================================================================
BASE_PROJ  := $(GPPROOT)


#   ============================================================================
#   Base directory for the tools
#   ============================================================================
BASE_TOOLS := /usr/bin


#   ============================================================================
#   Base directory for other tools
#   ============================================================================
BASE_PERL := /usr/bin


#   ============================================================================
#   File for redirecting unwanted output
#   ============================================================================
TRASH := /dev/null


#   ============================================================================
#   Define macros to handle VERBOSE option
#
#   CMDHIDE     : Token to prevent a command from echoing on the console
#   CHMECHO     : Token to allow/prevent a command to echoing on the console
#                 This macro is controlled by value of VERBOSE
#   ============================================================================

CMDHIDE := @

ifneq ("$(VERBOSE)", "1")
VERBOSE := 0
endif

ifeq ("$(VERBOSE)", "1")
CMDECHO :=
else
CMDECHO :=
endif

#   ============================================================================
#   Define macros for common shell commands
#
#   DISPLAY     : Display on screen
#   SETENV      : Set environment variable
#   MKDIR       : Make a directory
#   RMDIR       : Remove a directory
#   CHDIR       : Change to a directory
#   COPY        : Copy a file
#   XCOPY       : Copy files and directories
#   DELETE      : delete a file
#   RMTREE      : Remove a directory tree
#   IFF         : conditional execution
#   APPENDTO    : Shell token representing 'append' operator
#   SENDTO      : Shell token representing 'redirect' operator
#   ============================================================================

DISPLAY  := echo
SETENV   := set
MKDIR    := mkdir
RMDIR    := rmdir
CHDIR    := cd
COPY     := cp
XCOPY    := xcopy
DELETE   := rm -f
RMTREE   := rm -r
IFF      := if
APPENDTO := >>
SENDTO   := >


#   ============================================================================
#   Define macros for other tools
#
#   BANNER      : Display banner on screen
#   ============================================================================

PERL      := $(BASE_PERL)$(DIRSEP)perl

BANNER    := $(PERL) $(MAKEROOT)$(DIRSEP)bin$(DIRSEP)banner.pl

RUNIF     := $(PERL) $(MAKEROOT)$(DIRSEP)bin$(DIRSEP)runif.pl LINUX

CFG2C     := $(PERL) $(DSPLINK)$(DIRSEP)config$(DIRSEP)bin$(DIRSEP)cfg2c.pl

endif   # ifndef SYSTOOLS_MK
