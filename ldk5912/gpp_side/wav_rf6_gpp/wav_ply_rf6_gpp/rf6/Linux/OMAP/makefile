#
#
# PATH to the root of the RF directory from this makefile
# Modify RF_ROOT_DIR to relative reference the base Rules.make

#
# RF6 root directory with include and lib directories
#
RF_ROOT_DIR     := ../../../..

#
# Include the system configurations
#
include $(RF_ROOT_DIR)/Rules.make

BASE_CC_FLAGS  = -DOS_LINUX			\
		-DMSGQ_COMPONENT                        \
		-DCHNL_COMPONENT                        \
		-DZCPY_LINK                     \
		-I.				\
		-I$(RF_INC_DIR)			\
		-I$(RF_INC_DIR)/Linux/OMAP \
		-Wall

DEB_CC_FLAGS  = $(BASE_CC_FLAGS) -g -DDSP_DEBUG

REL_CC_FLAGS  = $(BASE_CC_FLAGS)

LDFLAGS       = -lc -lpthread


all: debug release

.PHONY: debug release

debug:
	@mkdir -p debug
	@mkdir -p $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/debug
	@rm -rf $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/debug/wav_ply_rf6_gpp

	@echo "-------- Building rf6 debug application --------"
	$(CC) $(DEB_CC_FLAGS) -o debug/wav_ply_rf6_gpp wav_ply_rf6_gpp.c \
	    $(LDFLAGS) \
	    $(RF_LIB_DIR)/dsplink.lib
	@echo The build was successful

	@cp -rf debug/wav_ply_rf6_gpp $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/debug/.
	@echo Copied application to $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/debug dir

release:
	@mkdir -p release
	@mkdir -p $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/release
	@rm -rf $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/release/wav_ply_rf6_gpp

	@echo "-------- Building rf6 release application --------"
	$(CC) $(REL_CC_FLAGS) -o release/wav_ply_rf6_gpp wav_ply_rf6_gpp.c \
	    $(LDFLAGS) \
	    $(RF_LIB_DIR)/dsplink.lib
	@echo The build was successful

	@cp -rf release/wav_ply_rf6_gpp $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/release/.
	@echo Copied application to $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/release dir

clean:
	rm -f */*.o 
	rm -rf debug release
	@rm -rf $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/*/wav_ply_rf6_gpp
