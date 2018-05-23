#############################################################################
# Makefile for building: correction
# Last modification: 2007-02-14 20:36 (Andrew Chen)
# Project:  correction
# Template: lib and exe
# Use make variable_name=' options ' to override the variables or make -e to
# override the file variables with the environment variables
# 		make CFLAGS='-g' or make prefix='/usr'
# Instructions:
# - modify the section 1)
# - if you want, modify the section 2) and 3), but it is not necessary
# - modify the variables of the section 4): CFLAGS INCPATH ALL_CFLAGS CPPFLAGS LIBS
# - in section 10), modify the following action:
#		* all: and or remove exe and lib prerequisite
#		* lib: and or remove staticlib and dynamiclib prerequisite
#		* clean: add or remove the files and directories that should be cleaned
#		* install: add or remove the files and directories that should be installed
#		* uninstall: add or remove the files and directories that should be uninstalled
#############################################################################

SHELL = /bin/sh

####### 1) Project names and system

#SYSTEM: linux or QNX
SYSTEM = linux
PROJECT=correction
EXE_NAME6 = Cor_obtutc


LIB_NAME =
VER_FILE_NAME = version.h
LIB_NAME = libpil
LIB_NAME1 = libwcs-4.0
####### 2) Directories for the installation

# Prefix for each installed program. Only ABSOLUTE PATH
prefix=~/ADC
adc_prefix=$(prefix)

exec_prefix=$(prefix)/correction
# The directory to install the binary files in.
bindir=$(exec_prefix)/bin
# The directory to install the libraries in.
libdir=$(exec_prefix)/lib
# The directory to install the info files in.
infodir=$(exec_prefix)/doc
# The directory to install the include files in.
includedir=$(exec_prefix)/include
# The directory to install the ut files in.
utdir=$(exec_prefix)/template
# The directory to install the data files in.
datadir=$(adc_prefix)/LV1
# The directory to install the cor files out.
dataoutdir=$(adc_prefix)/COR
# The directory to install the evt files out.
evtoutdir=$(adc_prefix)/EVT

includedir=$(exec_prefix)/include
# The directory to install the include files in.

pfilesdir=$(adc_prefix)/PFILES
# The directory to install the parameters files in.

indexdir=$(prefix)/INDEX
# The directory to install the index files in.

####### 3) Directories for the compiler
BIN_DIR = bin
OBJECTS_DIR = obj
SOURCE_DIR = src
INCLUDE_DIR = include
DOC_DIR = doc
UT_DIR = template
CONF_DIR = conf
DOXY_SOURCE_DIR = code_filtered
EXE_DESTDIR  = $(BIN_DIR)
LIB_DESTDIR = lib
DATA_DIR = data

####### 4) Compiler, tools and options

CC       = gcc -Wno-write-strings
CXX      = g++ -Wno-write-strings
#Insert the optional parameter to the compiler. The CFLAGS could be changed externally by the user
CFLAGS   = -Wall

#Set INCPATH to add the inclusion paths
INCPATH = -I./include -I$(AGILE_LIBWCS_INCLUDE) -I$(AGILE_LIBPIL_INCLUDE) -I$(AGILE_LIBCOR_INCLUDE) -I$(AGILE_CFITSIO_INCLUDE)
LIBPATH =  -L$(AGILE_LIBWCS_LIB)  -L$(AGILE_LIBPIL_LIB) -L$(AGILE_LIBCOR_LIB) -L$(AGILE_CFITSIO_LIB)



#Insert the implicit parameter to the compiler:
ALL_CFLAGS =  $(INCPATH) $(LIBPATH) $(CFLAGS)

ifeq ($(SYSTEM), QNX)
	ALL_CFLAGS += -Vgcc_ntox86_gpp -lang-c++
endif
#Use CPPFLAGS for the preprocessor
CPPFLAGS =
#Set LIBS for addition library
# LIBS =  -lcfitsio  -L. -lm -lnsl -lpil
LIBS =  -lm -lnsl -lcor -lwcs-4.0 -lpil -$(AGILE_CFITSIO_LIBNAME)

ifeq ($(SYSTEM), QNX)
	LIBS += -lsocket
endif
LINK     = g++
#for link
LFLAGS = -shared -Wl,-soname,$(TARGET1) -Wl,-rpath,$(DESTDIR)
AR       = ar cqs
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f -r
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rm -rf
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p


####### 5) VPATH

VPATH=$(SOURCE_DIR):$(INCLUDE_DIR):
vpath %.o $(OBJECTS_DIR)

####### 6) Files of the project

INCLUDE=$(foreach dir,$(INCLUDE_DIR), $(wildcard $(dir)/*.h))
SOURCE=$(foreach dir,$(SOURCE_DIR), $(wildcard $(dir)/*.cpp))
SOURCE+=$(foreach dir,$(SOURCE_DIR), $(wildcard $(dir)/*.c))
#Objects to build
OBJECTS=$(addsuffix .o, $(basename $(notdir $(SOURCE))))
#only for documentation generation
DOC_INCLUDE= $(addprefix $(DOXY_SOURCE_DIR)/, $(notdir $(INCLUDE)))
DOC_SOURCE= $(addprefix $(DOXY_SOURCE_DIR)/, $(notdir $(SOURCE)))

####### 7) Only for library generation

TARGET   = $(LIB_NAME).so.$(shell cat version)
TARGETA	= $(LIB_NAME).a
TARGETB	= $(LIB_NAME1).a
TARGETD	= $(LIB_NAME).so.$(shell cat version)
TARGET0	= $(LIB_NAME).so
TARGET1	= $(LIB_NAME).so.$(shell cut version -f 1 -d '.')
TARGET2	= $(LIB_NAME).so.$(shell cut version -f 1 -d '.').$(shell cut version -f 2 -d '.')



####### 9) Pattern rules
%.o : %.cpp
	$(CXX) $(ALL_CFLAGS) -c $< -o $(OBJECTS_DIR)/$@
%.o : %.c
	$(CC) $(ALL_CFLAGS) -c $< -o $(OBJECTS_DIR)/$@

#only for documentation generation
$(DOXY_SOURCE_DIR)/%.h : %.h
	doxyfilter < $< > $@

$(DOXY_SOURCE_DIR)/%.cpp : %.cpp
	doxyfilter < $< > $@




#all: compile the entire program.
all: exe

lib: staticlib

staticlib: makelibdir makeobjdir $(OBJECTS)	
		test -d $(LIB_DESTDIR) || mkdir -p $(LIB_DESTDIR)
	
exe: makeobjdir makebindir $(OBJECTS)	
	test -d $(EXE_DESTDIR) || mkdir -p $(EXE_DESTDIR)

	$(CXX)  $(ALL_CFLAGS) -o $(EXE_DESTDIR)/$(EXE_NAME6)  $(OBJECTS_DIR)/$(EXE_NAME6).o $(LIBS)
	
				
makeobjdir:
	test -d $(OBJECTS_DIR) || mkdir -p $(OBJECTS_DIR)	
	
	
makebindir:
#	test -d $(bindir) || mkdir -p $(bindir)
	test -d $(BIN_DIR) || mkdir -p $(BIN_DIR)

makelibdir:
	test -d $(LIB_DESTDIR) || mkdir -p $(LIB_DESTDIR)

#clean: delete all files from the current directory that are normally created by building the program. 
clean:
	$(DEL_DIR) $(OBJECTS_DIR)
	$(DEL_DIR) $(BIN_DIR)	

#Delete all files from the current directory that are created by configuring or building the program. 
distclean: clean

#install: compile the program and copy the executables, libraries, 
#and so on to the file names where they should reside for actual use. 


#uninstall: delete all the installed files--the copies that the `install' target creates. 
uninstall:

# For exe uninstall
	$(DEL_DIR) $(exec_prefix)
	$(DEL_DIR) $(exec_prefix)
#dist: create a distribution tar file for this program
dist: all


#info: generate any Info files needed.
info:
	test -d $(infodir) || mkdir -p $(infodir)
	$(COPY_DIR) $(DOC_DIR)/*.* $(infodir)


install: all
# For exe installation
	test -d $(bindir) || mkdir -p $(bindir)	
	test -d $(utdir)  || mkdir -p $(utdir)
	test -d $(confdir) || mkdir -p $(confdir)	
	test -d $(infodir) || mkdir -p $(infodir)
	test -d $(datadir) || mkdir -p $(datadir)
	test -d $(indexdir) || mkdir -p $(indexdir)

	test -d $(dataoutdir) || mkdir -p $(dataoutdir)
	test -d $(evtoutdir) || mkdir -p $(evtoutdir)	
	test -d $(includedir) || mkdir -p $(includedir)	
	test -d $(pfilesdir) || mkdir -p $(pfilesdir)	
		
	test ! -d $(BIN_DIR) || $(COPY_DIR) $(BIN_DIR) $(exec_prefix)
	test ! -f $(CONF_DIR)/*.par || $(COPY_FILE) $(CONF_DIR)/*.par $(pfilesdir)
	
	test ! -f $(DOC_DIR)/*.* ||  $(COPY_DIR) $(DOC_DIR)/*.* $(infodir)
	test ! -d $(SOURCE_DIR) || $(COPY_DIR) $(SOURCE_DIR) $(exec_prefix)
	
	
# 	if test -d $(UT_DIR); \
# 	   then $(COPY_FILE) $(UT_DIR)/*.* $(utdir)/;\
# 	fi
# 
# 	if test -d $(DATA_DIR); \
# 	   then $(COPY_FILE) $(DATA_DIR)/*.* $(datadir)/;\
# 	fi	
		
# 	if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i;	
	
# 	if test -d $(UT_DIR) then echo $(UT_DIR)
# 	$(COPY_FILE) $(DATA_DIR)/*.lv1 $(datadir)
	test ! -f $(INCLUDE_DIR)/*.h  || $(COPY_FILE) $(INCLUDE_DIR)/*.h $(includedir)




