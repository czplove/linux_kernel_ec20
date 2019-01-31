#--------------------------------------------------------------
#               Quectel OpenLinux
#--------------------------------------------------------------
QL_SDK_PATH   ?= $(shell pwd)/../..

QL_EXP_TARGETS = example_dual_apn example_single_apn 
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
            -I$(SDKTARGETSYSROOT)/usr/include/ql-manager    \



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
         $(SDKTARGETSYSROOT)/usr/lib/libql_mgmt_client.so  \

DUAL_LIB=$(QL_SDK_PATH)/lib/libql_common_api.a
SINGLE_LIB=-lql_qcmap_client
#---------------------
# Source code files
#---------------------
LOCAL_SRC_FILES = example_dual_apn.c example_single_apn.c


$(QL_EXP_TARGETS): 
#	$(CC) $(CPPFLAGS) $(CFLAGS) $(LOCAL_SRC_FILES)
	$(COMPILE.c) $(CPPFLAGS) $(LDFLAGS) example_dual_apn.c
	$(LINK.o) example_dual_apn.o $(LDFLAGS) $(DUAL_LIB) $(STD_LIB) -o example_dual_apn

	$(COMPILE.c) $(CPPFLAGS) $(LDFLAGS) example_single_apn.c
	$(LINK.o) example_single_apn.o $(LDFLAGS) $(SINGLE_LIB) $(STD_LIB) -o example_single_apn


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
