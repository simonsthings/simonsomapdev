#
#
# PATH to the root of the RF directory from this makefile
# Modify RF_ROOT_DIR to relative reference the base Rules.make

#
# RF6 root directory with include and lib directories
#
#RF_ROOT_DIR     := ../../../..

#
# Include the system configurations
#
#include ../../../../Rules.make

#BASE_CC_FLAGS  = -DOS_LINUX -DMSGQ_COMPONENT -DCHNL_COMPONENT -DZCPY_LINK -I. -I../../../../include -I../../../../include/Linux/OMAP -Wall

#DEB_CC_FLAGS  = -DOS_LINUX -DMSGQ_COMPONENT -DCHNL_COMPONENT -DZCPY_LINK -I. -I../../../../include -I../../../../include/Linux/OMAP -Wall -g -DDSP_DEBUG

#REL_CC_FLAGS  = -DOS_LINUX -DMSGQ_COMPONENT -DCHNL_COMPONENT -DZCPY_LINK -I. -I../../../../include -I../../../../include/Linux/OMAP -Wall

#LDFLAGS       = -lc -lpthread


all: debug release

.PHONY: debug release

debug:
	@mkdir -p debug
	@mkdir -p ../../../../bin/rf6/Linux/OMAP/debug
	@rm -rf ../../../../bin/rf6/Linux/OMAP/debug/prog1c

	@echo "-------- Building rf6 debug application --------"
	arm-linux-gcc -I/home/LinuxDA/sdk/LinuxDA/include -c /home/aritra/Desktop/wav_rf6_gpp-in_subversion/wav_rec_rf6_gpp/rf6/Linux/OMAP/prog1c.c
	arm-linux-gcc -DOS_LINUX -DMSGQ_COMPONENT -DCHNL_COMPONENT -DZCPY_LINK -I. -I/home/aritra/Desktop/wav_rf6_gpp-in_subversion/include -I/home/aritra/Desktop/wav_rf6_gpp-in_subversion/include/Linux/OMAP -Wall -g -o debug/prog1c mainprog.c prog1c.o -L/home/LinuxDA/sdk/LinuxDA/lib -lgui -lminidb -lsqlite3 -lc -lpthread /home/aritra/Desktop/wav_rf6_gpp-in_subversion/lib/dsplink.lib

	@echo The build was successful

	@cp -rf debug/prog1c ../../../../bin/rf6/Linux/OMAP/debug/.
	@echo Copied application to ../../../../bin/rf6/Linux/OMAP/debug dir

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
	rm -rf debug release
	@rm -rf /home/aritra/Desktop/wav_rf6_gpp-in_subversion/bin/rf6/Linux/OMAP/*/prog1c
