#--------------------------------------------------------------
#               Quectel OpenLinux
#--------------------------------------------------------------
QL_SDK_PATH   ?= $(shell pwd)/../../..

QL_EXP_TARGETS = 
QL_EXP_LDLIBS  =

#--------------------------------------------------------
# TOOLS BASIC CONFIG
# Note: No Need to change them
#--------------------------------------------------------
CPPFLAGS += -I./                     \
            -I./include          \
            -I../../../include          \
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
           -L./lib          \
           -lpthread -lstdc++ -lasound -lrt -lbluegate

STD_LIB = $(SDKTARGETSYSROOT)/usr/lib/libqcmap_client.so \
          $(SDKTARGETSYSROOT)/usr/lib/libqmi_cci.so

USR_LIB=$(QL_SDK_PATH)/lib/libql_common_api.a
USR_LIB+=$(QL_SDK_PATH)/lib/libql_peripheral.a
USR_LIB+=./lib/libbluegate.so

ql_nf3303_ble_server = demo_UI
ql_nf3303_ble_server_obj = ./source/demo_UI.o \
			./source/ql_nf3303_ble_common.o \
			./source/ql_nf3303_ble_sleep.o

%.o: %.c
	$(COMPILE.c) $(CPPFLAGS) -c $< -o $@

all: $(ql_nf3303_ble_server)

$(ql_nf3303_ble_server): $(ql_nf3303_ble_server_obj)
	$(LINK.cc) $(ql_nf3303_ble_server_obj) -o $@ $(LDFLAGS) $(USR_LIB) $(STD_LIB)

clean:
	@rm -rf $(ql_nf3303_ble_server) ./source/*.o

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
