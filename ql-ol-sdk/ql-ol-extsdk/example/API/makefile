#--------------------------------------------------------------
#               Quectel OpenLinux
#--------------------------------------------------------------
QL_SDK_PATH   ?= $(shell pwd)/../..

QL_EXP_TARGETS = testapi
QL_EXP_LDLIBS  =

#--------------------------------------------------------
# TOOLS BASIC CONFIG
# Note: No Need to change them
#--------------------------------------------------------
CPPFLAGS += -I./                     \
            -I./inc                  \
            -I../../include          \
            -I$(QL_SDK_PATH)/lib/interface/inc        \
            -I$(SDKTARGETSYSROOT)/usr/include               \
            -I$(SDKTARGETSYSROOT)/usr/include               \
            -I$(SDKTARGETSYSROOT)/usr/include/data          \
            -I$(SDKTARGETSYSROOT)/usr/include/dsutils       \
            -I$(SDKTARGETSYSROOT)/usr/include/qmi           \
            -I$(SDKTARGETSYSROOT)/usr/include/qmi-framework \



LDFLAGS += -L./                          \
           ${QL_EXP_LDLIBS}              \
           -L$(SDKTARGETSYSROOT)/usr/lib \
           -L$(QL_SDK_PATH)/lib          \
           -lpthread 


STD_LIB= $(SDKTARGETSYSROOT)/usr/lib/libdsi_netctrl.so     \
         $(SDKTARGETSYSROOT)/usr/lib/libdsutils.so         \
         $(SDKTARGETSYSROOT)/usr/lib/libqmiservices.so     \
         $(SDKTARGETSYSROOT)/usr/lib/libqmi_cci.so         \
         $(SDKTARGETSYSROOT)/usr/lib/libqmi_common_so.so   \
         $(SDKTARGETSYSROOT)/usr/lib/libqmi.so             \
         $(SDKTARGETSYSROOT)/usr/lib/libmcm.so             \


USR_LIB=$(QL_SDK_PATH)/lib/libql_common_api.a

#---------------------
# Source code files
#---------------------
LOCAL_SRC_FILES = api_test_main.c




testapi: 
#	$(CC) $(CPPFLAGS) $(CFLAGS) $(LOCAL_SRC_FILES)
	$(COMPILE.c) $(CPPFLAGS) $(LDFLAGS) $(LOCAL_SRC_FILES)
	$(LINK.o) *.o $(LDFLAGS) $(USR_LIB) $(STD_LIB) -o $@


all: $(QL_EXP_TARGETS)
.PHPNY: all

clean:
	rm -rf $(QL_EXP_TARGETS) *.o

.PHONY:checkmake
checkmake:  
	@echo -e "CURDIR =		\n	${CURDIR}"  
	@echo -e "\nMAKE_VERSION =	\n	${MAKE_VERSION}"  
	@echo -e "\nMAKEFILE_LIST =	\n	${MAKEFILE_LIST}"  
	@echo -e "\nCOMPILE.c =		\n	${COMPILE.c}"
	@echo -e "\nCOMPILE.cc =	\n	${COMPILE.cc}"
	@echo -e "\nCOMPILE.cpp =	\n	${COMPILE.cpp}"
	@echo -e "\nLINK.cc =		\n	${LINK.cc}"
	@echo -e "\nLINK.o =		\n	${LINK.o}"
	@echo -e "\nCPPFLAGS =		\n	${CPPFLAGS}"
	@echo -e "\nCFLAGS =		\n	${CFLAGS}"
	@echo -e "\nCXXFLAGS =		\n	${CXXFLAGS}"
	@echo -e "\nLDFLAGS =		\n	${LDFLAGS}"
	@echo -e "\nLDLIBS =		\n	${LDLIBS}"
