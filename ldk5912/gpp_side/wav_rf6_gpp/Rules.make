# --------------------------------------------------------------------
# Master makefile for RF GPP side build
# --------------------------------------------------------------------
#
# The below paths need to be modified according to
# the location where file have been installed/placed
#

# RF_ROOT_DIR is defined in the local makefile for each module/application

# Path to Reference Frameworrk include and lib directory
RF_INC_DIR	:= $(RF_ROOT_DIR)/include
RF_LIB_DIR	:= $(RF_ROOT_DIR)/lib

# Path to Cross Complier for LDK5912
CROSS_COMPILER_DIR	:= /opt/arm-linux/bin


# Complier, linker and archiver for Cross Complier
CC	 	:= $(CROSS_COMPILER_DIR)/arm-linux-gcc
LD		:= $(CROSS_COMPILER_DIR)/arm-linux-ld
AR	 	:= $(CROSS_COMPILER_DIR)/arm-linux-ar
