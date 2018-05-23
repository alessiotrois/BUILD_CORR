#############################################################################
# Makefile for building: AGILE ASDC CORRECTION
# Last modification: 
# Project:  AGILE ASDC CORRECTION
# Template: system
# Use make variable_name=' options ' to override the variables or make -e to
# override the file variables with the environment variables
# 		make CFLAGS='-g' or make prefix='/usr'
# Instructions:
# - if you want, modify the sections 1) and 2) but it is not necessary
# - modify the variables of the section 3): CFLAGS
# - in section 4), modify the following action:
#		* checkout: modify the project and option -r
#			cvs checkout -r TAG_NAME prj_name
#		* all: and or remove exe and lib prerequisite
#		* clean: add or remove the files and directories that should be cleaned
#		* install: add or remove the files and directories that should be installed
#		* uninstall: add or remove the files and directories that should be uninstalled
#############################################################################

SHELL = /bin/sh

####### 0) Common definition

####### 1) Directories for the installation
# Prefix for each installed program. Only ABSOLUTE PATH
prefix=$(HOME)/ADC
exec_prefix=$(prefix)
# The directory to install the binary files in.
bindir=$(exec_prefix)/bin
# The directory to install the local configuration file.
datadir=$(exec_prefix)/share
# The directory to install the libraries in.
libdir=$(exec_prefix)/lib
# The directory to install the info files in.
infodir=$(exec_prefix)/info
# The directory to install the include files in.
includedir=$(exec_prefix)/include

####### 2) Directories
PRJ_DIR=Projects

INSTALL_SUBDIR=

export AGILE_LIBPIL_INCLUDE=$(exec_prefix)/include
export AGILE_LIBPIL_LIB=$(exec_prefix)/lib
export AGILE_LIBWCS_INCLUDE=$(exec_prefix)/include
export AGILE_LIBWCS_LIB=$(exec_prefix)/lib
export AGILE_LIBCOR_INCLUDE=$(exec_prefix)/include
export AGILE_LIBCOR_LIB=$(exec_prefix)/lib

export SASOA_LIB=$(exec_prefix)/lib
export SASOA_INCLUDE=$(exec_prefix)/include


####### 3) Compiler, tools and options

#Insert the optional parameter to the compiler. The CFLAGS could be changed externally by the user
CFLAGS   =
AR       = ar cqs
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rm -rf
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p
CVS = cvs checkout

BUILD = 
#BUILD = 

####### 4) Build rules
all:
	$(MKDIR) $(exec_prefix)/$(INSTALL_SUBDIR)/share
	test -d $(exec_prefix)/L1 || $(MKDIR) $(exec_prefix)/LV1
	test -d $(exec_prefix)/CORR || $(MKDIR) $(exec_prefix)/COR
	cd ./$(PRJ_DIR)/agile_wcs_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/agile_pil_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/agile_cor_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_0101	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3201	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3300	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3401	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_35XY	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3503	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3504	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3505	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3506	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3601	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3901	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3902	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3903	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3904	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3905	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3908	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3909	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3910	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3911	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3913	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3914	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3914_bis	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3916	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3916_bis    &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3917	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_3918	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	mkdir -p $(prefix)/Kalman && tar -C $(prefix)/Kalman -xvzf ./$(PRJ_DIR)/kalmanFilterCGS_AGILE/Kalman.tgz  && mkdir -p $(prefix)/correction/bin && ln -f -s $(prefix)/Kalman/Kalman $(prefix)/correction/bin
	cd ./$(PRJ_DIR)/cor_obtutc      &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install       
	cd ./$(PRJ_DIR)/cor_tcs         &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_drift       &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install
	cd ./$(PRJ_DIR)/cor_33XY_bis    &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) install

exe:
	cd ./$(PRJ_DIR)/agile_wcs_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/agile_pil_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/agile_cor_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_0101	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3201	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3300	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3401	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_35XY	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3503	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3504	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3505	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3506	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3601	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3901	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3902	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3903	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3904	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3905	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3908	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3909	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3910	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3911	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3913	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3914	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3914_bis	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3916	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3916_bis    &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3917	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_3918	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_obtutc      &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_tcs         &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_drift       &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe
	cd ./$(PRJ_DIR)/cor_33XY_bis    &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) exe

checkout:
	test -d $(PRJ_DIR) || mkdir -p $(PRJ_DIR)
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) agile_wcs_lib
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) agile_pil_lib
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) agile_cor_lib
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_0101
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3201
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3300
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3401
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_35XY
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3503
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3504
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3505
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3506
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3601
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3901
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3902
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3903
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3904
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3905
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3908
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3909
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3910
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3911
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3913
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3914
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3914_bis
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3916
	cd ./$(PRJ_DIR)         &&      $(CVS)  $(BUILD) cor_3916_bis
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3917
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) cor_3918
	cd ./$(PRJ_DIR) 	&&	$(CVS)  $(BUILD) kalmanFilterCGS_AGILE
	cd ./$(PRJ_DIR)         &&      $(CVS)  $(BUILD) cor_obtutc
	cd ./$(PRJ_DIR)         &&      $(CVS)  $(BUILD) cor_tcs
	cd ./$(PRJ_DIR)         &&      $(CVS)  $(BUILD) cor_drift
	cd ./$(PRJ_DIR)         &&      $(CVS)  $(BUILD) cor_33XY_bis

#clean: delete all files from the current directory that are normally created by building the program.
clean:
	cd ./$(PRJ_DIR)/agile_wcs_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/agile_pil_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/agile_cor_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_0101	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3201	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3300	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3401	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_35XY	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3503	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3504	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3505	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3506	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3601	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3901	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3902	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3903	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3904	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3905	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3908	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3909	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3910	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3911	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3913	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3914	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3914_bis	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3916	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3916_bis    &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3917	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_3918	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_obtutc      &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_tcs         &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_drift       &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean
	cd ./$(PRJ_DIR)/cor_33XY_bis    &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) clean

#Delete all files from the current directory that are created by configuring or building the program.
distclean: 
	test $(PRJ_DIR) = . || $(DEL_DIR) $(PRJ_DIR)

#install: compile the program and copy the executables, libraries,
#and so on to the file names where they should reside for actual use.
install: all


#uninstall: delete all the installed files--the copies that the `install' target creates.
uninstall:

	cd ./$(PRJ_DIR)/agile_wcs_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/agile_pil_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/agile_cor_lib	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_0101	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3201	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3300	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3401	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_35XY	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3503	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3504	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3505	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3506	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3901	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3902	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3903	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3904	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3905	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3906	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3909	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3910	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3911	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3913	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3914	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3914_bis	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3916	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3916_bis    &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3917	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_3918	&&	$(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_obtutc      &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_tcs         &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_drift       &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall
	cd ./$(PRJ_DIR)/cor_33XY_bis    &&      $(MAKE) CFLAGS=$(CFLAGS) prefix=$(prefix) uninstall

