# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))
WIND_USR := $(subst \,/,$(WIND_USR))

all : pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = PPC603gnu
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
WS_ROOT_DIR := C:/WindRiver/workspace
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/CFHS_2013_Robot++



#Global Build Macros
PROJECT_TYPE = DKM
DEFINES = 
EXPAND_DBG = 0


#BuildSpec specific Build Macros
VX_CPU_FAMILY = ppc
CPU = PPC603
TOOL_FAMILY = gnu
TOOL = gnu
TOOL_PATH = 
CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
LIBPATH = 
LIBS = 

IDE_INCLUDES = -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 

IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 

IDE_DEFINES = 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C-Compiler = -g
DEBUGFLAGS_C++-Compiler = -g
DEBUGFLAGS_Linker = -g
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -g
else
DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif


#Project Targets
PROJECT_TARGETS = CF2013Code/$(MODE_DIR)/CF2013Code.out \
	 CF2013Code_partialImage/$(MODE_DIR)/CF2013Code_partialImage.o


#Rules

# CF2013Code
ifeq ($(DEBUG_MODE),1)
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
CF2013Code/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
CF2013Code/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 
CF2013Code/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
CF2013Code/$(MODE_DIR)/% : IDE_DEFINES = 
CF2013Code/$(MODE_DIR)/% : PROJECT_TYPE = DKM
CF2013Code/$(MODE_DIR)/% : DEFINES = 
CF2013Code/$(MODE_DIR)/% : EXPAND_DBG = 0
CF2013Code/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
CF2013Code/$(MODE_DIR)/% : CPU = PPC603
CF2013Code/$(MODE_DIR)/% : TOOL_FAMILY = gnu
CF2013Code/$(MODE_DIR)/% : TOOL = gnu
CF2013Code/$(MODE_DIR)/% : TOOL_PATH = 
CF2013Code/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
CF2013Code/$(MODE_DIR)/% : LIBPATH = 
CF2013Code/$(MODE_DIR)/% : LIBS = 
CF2013Code/$(MODE_DIR)/% : OBJ_DIR := CF2013Code/$(MODE_DIR)

OBJECTS_CF2013Code = CF2013Code_partialImage/$(MODE_DIR)/CF2013Code_partialImage.o

CF2013Code/$(MODE_DIR)/CF2013Code.out : $(OBJECTS_CF2013Code)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";nmppc $(OBJECTS_CF2013Code) $(IDE_LIBRARIES) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c ppc > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -fdollars-in-identifiers -Wall $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES)  -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X -T $(WIND_BASE)/target/h/tool/gnu/ldscripts/link.OUT -o "$@" $(OBJ_DIR)/ctdt.o $(OBJECTS_CF2013Code) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS)  $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

CF2013Code/$(MODE_DIR)/CF2013Code_compile_file : $(FILE) ;

_clean :: CF2013Code/$(MODE_DIR)/CF2013Code_clean

CF2013Code/$(MODE_DIR)/CF2013Code_clean : 
	$(TRACE_FLAG)if [ -d "CF2013Code" ]; then cd "CF2013Code"; rm -rf $(MODE_DIR); fi


# CF2013Code_partialImage
ifeq ($(DEBUG_MODE),1)
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
CF2013Code_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
CF2013Code_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 
CF2013Code_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
CF2013Code_partialImage/$(MODE_DIR)/% : IDE_DEFINES = 
CF2013Code_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
CF2013Code_partialImage/$(MODE_DIR)/% : DEFINES = 
CF2013Code_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
CF2013Code_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
CF2013Code_partialImage/$(MODE_DIR)/% : CPU = PPC603
CF2013Code_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = gnu
CF2013Code_partialImage/$(MODE_DIR)/% : TOOL = gnu
CF2013Code_partialImage/$(MODE_DIR)/% : TOOL_PATH = 
CF2013Code_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
CF2013Code_partialImage/$(MODE_DIR)/% : LIBPATH = 
CF2013Code_partialImage/$(MODE_DIR)/% : LIBS = 
CF2013Code_partialImage/$(MODE_DIR)/% : OBJ_DIR := CF2013Code_partialImage/$(MODE_DIR)

CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/CFCode2013++.o : $(PRJ_ROOT_DIR)/CFCode2013++.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/DiskPickup.o : $(PRJ_ROOT_DIR)/DiskPickup.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/DiskShooter.o : $(PRJ_ROOT_DIR)/DiskShooter.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/Drive.o : $(PRJ_ROOT_DIR)/Drive.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/FindGoals.o : $(PRJ_ROOT_DIR)/FindGoals.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/Hopper.o : $(PRJ_ROOT_DIR)/Hopper.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/PIDLoop.o : $(PRJ_ROOT_DIR)/PIDLoop.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


OBJECTS_CF2013Code_partialImage = CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/CFCode2013++.o \
	 CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/DiskPickup.o \
	 CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/DiskShooter.o \
	 CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/Drive.o \
	 CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/FindGoals.o \
	 CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/Hopper.o \
	 CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/PIDLoop.o

CF2013Code_partialImage/$(MODE_DIR)/CF2013Code_partialImage.o : $(OBJECTS_CF2013Code_partialImage)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X  -o "$@" $(OBJECTS_CF2013Code_partialImage) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS)  && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

CF2013Code_partialImage/$(MODE_DIR)/CF2013Code_partialImage_compile_file : $(FILE) ;

_clean :: CF2013Code_partialImage/$(MODE_DIR)/CF2013Code_partialImage_clean

CF2013Code_partialImage/$(MODE_DIR)/CF2013Code_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "CF2013Code_partialImage" ]; then cd "CF2013Code_partialImage"; rm -rf $(MODE_DIR); fi

force : 

DEP_FILES := CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/CFCode2013++.d CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/DiskPickup.d CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/DiskShooter.d \
	 CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/Drive.d CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/FindGoals.d CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/Hopper.d \
	 CF2013Code_partialImage/$(MODE_DIR)/Objects/CFHS_2013_Robot++/PIDLoop.d
-include $(DEP_FILES)



-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

main_all : external_build $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""
