#--------------------------------------------------------------
#               Quectel OpenLinux
#--------------------------------------------------------------
QL_SDK_PATH   ?= $(shell pwd)/../..

QL_EXP_TARGETS = example_parse_json example_create_json
QL_EXP_LDLIBS  = -ljson-c

#--------------------------------------------------------
# TOOLS BASIC CONFIG
# Note: No Need to change them
#--------------------------------------------------------
#CPPFLAGS = -I./ -I$(SDKTARGETSYSROOT)/usr/include/ -I$(QL_SDK_PATH)/include
CPPFLAGS = -I./ -I$(QL_SDK_PATH)/include -I$(SDKTARGETSYSROOT)/usr/include/json-c
LDFLAGS = -L./ -L$(QL_SDK_PATH)/lib -lrt ${QL_EXP_LDLIBS}


all: $(QL_EXP_TARGETS)
.PHPNY: all


%:%.c
#	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $@
	$(COMPILE.c) $^
	$(LINK.o) $@.o -o $@

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
