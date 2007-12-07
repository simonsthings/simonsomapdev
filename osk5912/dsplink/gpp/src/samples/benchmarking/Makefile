#   ============================================================================
#   @file   Makefile
#
#   @path   $(PROJROOT)\src\samples\benchmarking
#
#   @desc   Makefile to build benchmarking application.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   This software is copyright to Texas Instruments Incorporated.
#   Its usage is governed by the terms and conditions stipulated in
#   the license agreement under which this software has been supplied.
#
#   The recipient of this software implicitly accepts the terms of
#   the license.
#   ============================================================================


PATH_X_COMPILER = /opt/hardhat/devkit/arm/920t_le

PATH_CGTOOL     = $(PATH_X_COMPILER)/bin

PATH_INCLUDE    = $(PATH_X_COMPILER)/target/usr/include

BASE_CC_FLAGS  = -DOS_LINUX                         \
				 -I$(PATH_INCLUDE)/usr/include      \
				 -I$(PROJROOT)/inc                  \
				 -I$(PROJROOT)/BUILD/INCLUDE        \
				 -I$(PROJROOT)/inc/$(GPPOS)         \
				 -I$(PROJROOT)/src/api              \
				 -I.                                \
				 -Wall


DEB_CC_FLAGS  = $(BASE_CC_FLAGS) -g -DDDSP_DEBUG

REL_CC_FLAGS  = $(BASE_CC_FLAGS)

LDFLAGS       = -L$(PATH_CGTOOL)/usr/lib -lc


CC = $(PATH_CGTOOL)/arm_920t_le-gcc
LD = $(PATH_CGTOOL)/arm_920t_le-ld

all: debug release

.PHONY: debug release

debug:
	-mkdir DEBUG
	@echo "Compiling Benchmarking DEBUG Application...."
	$(CC) $(LDFLAGS) $(DEB_CC_FLAGS) -o DEBUG/benchmarkinggpp benchmarking.c   \
		  $(PROJROOT)/export/BIN/Linux/OMAP/DEBUG/dsplink.lib
	@echo If the build was successful:
	@echo   Copy the generated executable to the target file system.

release:
	-mkdir RELEASE
	@echo "Compiling Benchmarking RELEASE Application...."
	$(CC) $(LDFLAGS) $(REL_CC_FLAGS) -o RELEASE/benchmarkinggpp benchmarking.c \
		  $(PROJROOT)/export/BIN/Linux/OMAP/RELEASE/dsplink.lib
	@echo If the build was successful:
	@echo   Copy the generated executable to the target file system.


clean:
	rm -f *.o *~ core DEBUG/benchmarkinggpp RELEASE/benchmarkinggpp
