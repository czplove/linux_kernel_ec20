#--------------------------------------------------------------
#               Quectel OpenLinux
#--------------------------------------------------------------
QL_SDK_PATH   ?= $(shell pwd)/../../..

QL_EXP_TARGETS = 
QL_EXP_LDLIBS  =

# This flag is used to remove the debug information in the app trace
GLOBAL_DEFINE += -DAPP_TRACE_NODEBUG

# This flag is used to add timestamp in the app traces
#GLOBAL_DEFINE += -DAPP_TRACE_TIMESTAMP

# This flag is used to add color in the app traces
GLOBAL_DEFINE += -DAPP_TRACE_COLOR -D_FORTIFY_SOURCE=2 -DBUILDCFG

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
           -lpthread -lstdc++ -lrt -L./libbsa/lib -lbsa \

STD_LIB = $(SDKTARGETSYSROOT)/usr/lib/libqcmap_client.so \
          $(SDKTARGETSYSROOT)/usr/lib/libqmi_cci.so

USR_LIB=$(QL_SDK_PATH)/lib/libql_common_api.a
USR_LIB+=$(QL_SDK_PATH)/lib/libql_peripheral.a

bsa_client = bsa_client 
bsa_client_obj = ./source/app_ble_test_main.o \
	./source/app_ble_test_server.o \
	./source/app_ble_test_client.o \
	./source/app_dm.o \
	./source/app_ble.o \
	./source/app_ble_client.o \
	./source/app_ble_server.o \
	./source/app_ble_client_xml.o \
	./source/app_ble_client_db.o \
	./source/app_disc.o \
	./source/app_utils.o \
	./source/app_mgt.o \
	./source/app_xml_param.o \
	./source/app_xml_utils.o \
	./source/app_services.o \
	./source/app_thread.o \
	./source/app_manager.o \
	./source/nanoxml.o \
	./source/ql_cm256sm_ble_server.o \
	./source/ql_cm256sm_ble_sleep.o

%.o: %.c
	$(COMPILE.cc) $(GLOBAL_DEFINE) $(CPPFLAGS) -c $< -o $@ -w

all: $(bsa_client)

$(bsa_client): $(bsa_client_obj)
	$(LINK.o) $(GLOBAL_DEFINE) $(bsa_client_obj) -o $@ $(LDFLAGS) $(USR_LIB) $(STD_LIB)

clean:
	@rm -rf $(bsa_client) ./source/*.o

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
