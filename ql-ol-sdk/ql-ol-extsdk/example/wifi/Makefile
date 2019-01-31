#--------------------------------------------------------------
#               Quectel OpenLinux
#--------------------------------------------------------------
QL_SDK_PATH   ?= $(shell pwd)/../..

QL_EXP_TARGETS = 
QL_EXP_LDLIBS  = -lql_sys_log

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
            -I$(SDKTARGETSYSROOT)/usr/include/data          

LDFLAGS += -L./                          \
           ${QL_EXP_LDLIBS}              \
           -L$(SDKTARGETSYSROOT)/usr/lib \
           -L$(QL_SDK_PATH)/lib          \
           -lpthread -lstdc++


STD_LIB = $(SDKTARGETSYSROOT)/usr/lib/libqcmap_client.so \
          $(SDKTARGETSYSROOT)/usr/lib/libqmi_cci.so



USR_LIB=$(QL_SDK_PATH)/lib/libql_common_api.a

wifi_api_test = wifi_api_test
wifi_api_test_obj = example_wifi.o

wifi_ap_mode = wifi_ap_mode
wifi_ap_mode_obj = ap_mode.o

wifi_sta_mode = wifi_sta_mode
wifi_sta_mode_obj = sta_mode.o

wifi_ap_sta_mode = wifi_ap_sta_mode
wifi_ap_sta_mode_obj = ap_sta_mode.o

wifi_ap_ap_mode = wifi_ap_ap_mode
wifi_ap_ap_mode_obj = ap_ap_mode.o

%.o: %.c
	$(COMPILE.c) $(CPPFLAGS) -c $< -o $@

all: $(wifi_api_test) $(wifi_ap_mode) $(wifi_sta_mode) $(wifi_ap_sta_mode) $(wifi_ap_ap_mode)

$(wifi_api_test): $(wifi_api_test_obj)
	$(LINK.cc) $(wifi_api_test_obj) -o $@ $(LDFLAGS) $(USR_LIB) $(STD_LIB)

$(wifi_ap_mode): $(wifi_ap_mode_obj)
	$(LINK.cc) $(wifi_ap_mode_obj) -o $@ $(LDFLAGS) $(USR_LIB) $(STD_LIB)

$(wifi_sta_mode): $(wifi_sta_mode_obj)
	$(LINK.cc) $(wifi_sta_mode_obj) -o $@ $(LDFLAGS) $(USR_LIB) $(STD_LIB)

$(wifi_ap_sta_mode): $(wifi_ap_sta_mode_obj)
	$(LINK.cc) $(wifi_ap_sta_mode_obj) -o $@ $(LDFLAGS) $(USR_LIB) $(STD_LIB)

$(wifi_ap_ap_mode): $(wifi_ap_ap_mode_obj)
	$(LINK.cc) $(wifi_ap_ap_mode_obj) -o $@ $(LDFLAGS) $(USR_LIB) $(STD_LIB)

clean:
	@rm -rf $(wifi_api_test) $(wifi_ap_mode) $(wifi_sta_mode) $(wifi_ap_sta_mode) $(wifi_ap_ap_mode) *.o

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
