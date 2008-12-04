#
#
# PATH to the root of the RF directory from this makefile
# Modify RF_ROOT_DIR to relative reference the base Rules.make

#
# RF6 root directory with include and lib directories
#
#RF_ROOT_DIR     := /home/aritra/workspace/daqGUIsubversion

#
# Include the system configurations
#
#include /home/aritra/Desktop/wav_rf6_gpp-in_subversion/Rules.make

#BASE_CC_FLAGS  = -DOS_LINUX -DMSGQ_COMPONENT -DCHNL_COMPONENT -DZCPY_LINK -I. -I/home/aritra/workspace/daqGUIsubversion/include -I/home/aritra/workspace/daqGUIsubversion/include/Linux/OMAP -Wall

#DEB_CC_FLAGS  = -DOS_LINUX -DMSGQ_COMPONENT -DCHNL_COMPONENT -DZCPY_LINK -I. -I/home/aritra/workspace/daqGUIsubversion/include -I/home/aritra/workspace/daqGUIsubversion/include/Linux/OMAP -Wall -g -DDSP_DEBUG

#REL_CC_FLAGS  = -DOS_LINUX -DMSGQ_COMPONENT -DCHNL_COMPONENT -DZCPY_LINK -I. -I/home/aritra/workspace/daqGUIsubversion/include -I/home/aritra/workspace/daqGUIsubversion/include/Linux/OMAP -Wall

#LDFLAGS       = -lc -lpthread


all: debug release

.PHONY: debug release

debug:
	@mkdir -p debug
#	@mkdir -p /home/aritra/Desktop/wav_rf6_gpp-in_subversion/bin/rf6/Linux/OMAP/debug
#	@rm -rf debug/daqGUI

	@echo "-------- Building rf6 debug application --------"
	/opt/arm-linux/bin/arm-linux-gcc -I/home/LinuxDA/sdk/LinuxDA/include -c daqGUI.c
	/opt/arm-linux/bin/arm-linux-gcc -DOS_LINUX -DMSGQ_COMPONENT -DCHNL_COMPONENT -DZCPY_LINK -I. -I/home/LinuxDA/wav_rf6_gpp/include -I/home/LinuxDA/wav_rf6_gpp/include/Linux/OMAP -Wall -g -o debug/daqGUI wav_rec_rf6_gpp.c daqGUI.o -L/home/LinuxDA/sdk/LinuxDA/lib -lgui -lminidb -lsqlite3 -lc -lpthread /home/LinuxDA/wav_rf6_gpp/lib/dsplink.lib

	@echo The build was successful

	@cp -rf debug/daqGUI /data/target/xbin/.
	@echo Copied application to "/data/target/xbin" directory.

#release:
#	@mkdir -p release
#	@mkdir -p $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/release
#	@rm -rf $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/release/wav_rec_rf6_gpp
#
#	@echo "-------- Building rf6 release application --------"
#	$(CC) $(REL_CC_FLAGS) -o release/wav_rec_rf6_gpp wav_rec_rf6_gpp.c \
#	    $(LDFLAGS) \
#	    $(RF_LIB_DIR)/dsplink.lib
#	@echo The build was successful
#
#	@cp -rf release/wav_rec_rf6_gpp $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/release/.
#	@echo Copied application to $(RF_ROOT_DIR)/bin/rf6/Linux/OMAP/release dir

clean:
	rm -f */*.o 
	rm -f debug/daqGUI 
	@rm -f /home/aritra/workspace/daqGUIsubversion/bin/rf6/Linux/OMAP/*/daqGUI
