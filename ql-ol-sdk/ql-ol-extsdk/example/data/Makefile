#--------------------------------------------------------------
#               Quectel OpenLinux
#--------------------------------------------------------------
QL_SDK_PATH   ?= $(shell pwd)/../..

QL_EXP_TARGETS = example_data_call_v2 example_apn_v2 example_default_profile example_data_call_v2_cdma
QL_EXP_LDLIBS  = -lql_sys_log

#--------------------------------------------------------
# TOOLS BASIC CONFIG
# Note: No Need to change them
#--------------------------------------------------------
CPPFLAGS += -Wall -Werror -I./       \
            -I./inc                  \
            -I../../include          \
            -I$(QL_SDK_PATH)/lib/interface/inc        \
            -I$(SDKTARGETSYSROOT)/usr/include               \
            -I$(SDKTARGETSYSROOT)/usr/include/ql-manager    \



LDFLAGS += -L./                          \
           ${QL_EXP_LDLIBS}              \
           -L$(SDKTARGETSYSROOT)/usr/lib \
           -L$(QL_SDK_PATH)/lib          \
           -lpthread			 


STD_LIB= $(SDKTARGETSYSROOT)/usr/lib/libql_mgmt_client.so  \

DUAL_LIB=$(QL_SDK_PATH)/lib/libql_common_api.a
SINGLE_LIB=-lql_qcmap_client

$(QL_EXP_TARGETS): 
	$(COMPILE.c) $(CPPFLAGS) $(LDFLAGS) example_data_call_v2.c
	$(LINK.o) example_data_call_v2.o $(LDFLAGS) $(DUAL_LIB) $(STD_LIB) -o example_data_call_v2

	$(COMPILE.c) $(CPPFLAGS) $(LDFLAGS) example_apn_v2.c
	$(LINK.o) example_apn_v2.o $(LDFLAGS) $(DUAL_LIB) $(STD_LIB) -o example_apn_v2

	$(COMPILE.c) $(CPPFLAGS) $(LDFLAGS) example_default_profile.c
	$(LINK.o) example_default_profile.o $(LDFLAGS) $(DUAL_LIB) $(STD_LIB) -o example_default_profile

	$(COMPILE.c) $(CPPFLAGS) $(LDFLAGS) example_data_call_v2_cdma.c
	$(LINK.o) example_data_call_v2_cdma.o $(LDFLAGS) $(DUAL_LIB) $(STD_LIB) -o example_data_call_v2_cdma


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
