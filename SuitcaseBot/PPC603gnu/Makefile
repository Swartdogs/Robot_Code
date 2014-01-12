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
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/SuitcaseBot



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

IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip 

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
PROJECT_TARGETS = SuitcaseBot/$(MODE_DIR)/SuitcaseBot.out \
	 SuitcaseBot_partialImage/$(MODE_DIR)/SuitcaseBot_partialImage.o


#Rules

# SuitcaseBot
ifeq ($(DEBUG_MODE),1)
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
SuitcaseBot/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
SuitcaseBot/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip 
SuitcaseBot/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
SuitcaseBot/$(MODE_DIR)/% : IDE_DEFINES = 
SuitcaseBot/$(MODE_DIR)/% : PROJECT_TYPE = DKM
SuitcaseBot/$(MODE_DIR)/% : DEFINES = 
SuitcaseBot/$(MODE_DIR)/% : EXPAND_DBG = 0
SuitcaseBot/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
SuitcaseBot/$(MODE_DIR)/% : CPU = PPC603
SuitcaseBot/$(MODE_DIR)/% : TOOL_FAMILY = gnu
SuitcaseBot/$(MODE_DIR)/% : TOOL = gnu
SuitcaseBot/$(MODE_DIR)/% : TOOL_PATH = 
SuitcaseBot/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
SuitcaseBot/$(MODE_DIR)/% : LIBPATH = 
SuitcaseBot/$(MODE_DIR)/% : LIBS = 
SuitcaseBot/$(MODE_DIR)/% : OBJ_DIR := SuitcaseBot/$(MODE_DIR)

OBJECTS_SuitcaseBot = SuitcaseBot_partialImage/$(MODE_DIR)/SuitcaseBot_partialImage.o

SuitcaseBot/$(MODE_DIR)/SuitcaseBot.out : $(OBJECTS_SuitcaseBot)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";nmppc $(OBJECTS_SuitcaseBot) $(IDE_LIBRARIES) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c ppc > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -fdollars-in-identifiers -Wall $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES)  -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X -T $(WIND_BASE)/target/h/tool/gnu/ldscripts/link.OUT -o "$@" $(OBJ_DIR)/ctdt.o $(OBJECTS_SuitcaseBot) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS)  $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

SuitcaseBot/$(MODE_DIR)/SuitcaseBot_compile_file : $(FILE) ;

_clean :: SuitcaseBot/$(MODE_DIR)/SuitcaseBot_clean

SuitcaseBot/$(MODE_DIR)/SuitcaseBot_clean : 
	$(TRACE_FLAG)if [ -d "SuitcaseBot" ]; then cd "SuitcaseBot"; rm -rf $(MODE_DIR); fi


# SuitcaseBot_partialImage
ifeq ($(DEBUG_MODE),1)
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
SuitcaseBot_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip 
SuitcaseBot_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
SuitcaseBot_partialImage/$(MODE_DIR)/% : IDE_DEFINES = 
SuitcaseBot_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
SuitcaseBot_partialImage/$(MODE_DIR)/% : DEFINES = 
SuitcaseBot_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
SuitcaseBot_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
SuitcaseBot_partialImage/$(MODE_DIR)/% : CPU = PPC603
SuitcaseBot_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = gnu
SuitcaseBot_partialImage/$(MODE_DIR)/% : TOOL = gnu
SuitcaseBot_partialImage/$(MODE_DIR)/% : TOOL_PATH = 
SuitcaseBot_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
SuitcaseBot_partialImage/$(MODE_DIR)/% : LIBPATH = 
SuitcaseBot_partialImage/$(MODE_DIR)/% : LIBS = 
SuitcaseBot_partialImage/$(MODE_DIR)/% : OBJ_DIR := SuitcaseBot_partialImage/$(MODE_DIR)

SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/CommandBase.o : $(PRJ_ROOT_DIR)/CommandBase.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/AutoSquare.o : $(PRJ_ROOT_DIR)/Commands/AutoSquare.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/AutoSquareDial.o : $(PRJ_ROOT_DIR)/Commands/AutoSquareDial.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DialSetTarget.o : $(PRJ_ROOT_DIR)/Commands/DialSetTarget.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveDistance.o : $(PRJ_ROOT_DIR)/Commands/DriveDistance.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveJoystick.o : $(PRJ_ROOT_DIR)/Commands/DriveJoystick.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveRearRotate.o : $(PRJ_ROOT_DIR)/Commands/DriveRearRotate.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveRotate.o : $(PRJ_ROOT_DIR)/Commands/DriveRotate.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveStopSensor.o : $(PRJ_ROOT_DIR)/Commands/DriveStopSensor.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveStrafe.o : $(PRJ_ROOT_DIR)/Commands/DriveStrafe.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/ResetGyro.o : $(PRJ_ROOT_DIR)/Commands/ResetGyro.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/IterativeRobot525.o : $(PRJ_ROOT_DIR)/IterativeRobot525.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/OI.o : $(PRJ_ROOT_DIR)/OI.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/RobotMap.o : $(PRJ_ROOT_DIR)/RobotMap.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/Dail.o : $(PRJ_ROOT_DIR)/Subsystems/Dail.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/Drive.o : $(PRJ_ROOT_DIR)/Subsystems/Drive.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/PIDControl.o : $(PRJ_ROOT_DIR)/Subsystems/PIDControl.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/Pneumatic.o : $(PRJ_ROOT_DIR)/Subsystems/Pneumatic.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/SuitcaseBot.o : $(PRJ_ROOT_DIR)/SuitcaseBot.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


OBJECTS_SuitcaseBot_partialImage = SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/CommandBase.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/AutoSquare.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/AutoSquareDial.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DialSetTarget.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveDistance.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveJoystick.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveRearRotate.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveRotate.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveStopSensor.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveStrafe.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/ResetGyro.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/IterativeRobot525.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/OI.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/RobotMap.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/Dail.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/Drive.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/PIDControl.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/Pneumatic.o \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/SuitcaseBot.o

SuitcaseBot_partialImage/$(MODE_DIR)/SuitcaseBot_partialImage.o : $(OBJECTS_SuitcaseBot_partialImage)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X  -o "$@" $(OBJECTS_SuitcaseBot_partialImage) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS)  && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

SuitcaseBot_partialImage/$(MODE_DIR)/SuitcaseBot_partialImage_compile_file : $(FILE) ;

_clean :: SuitcaseBot_partialImage/$(MODE_DIR)/SuitcaseBot_partialImage_clean

SuitcaseBot_partialImage/$(MODE_DIR)/SuitcaseBot_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "SuitcaseBot_partialImage" ]; then cd "SuitcaseBot_partialImage"; rm -rf $(MODE_DIR); fi

force : 

DEP_FILES := SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/CommandBase.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/AutoSquare.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/AutoSquareDial.d \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DialSetTarget.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveDistance.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveJoystick.d \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveRearRotate.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveRotate.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveStopSensor.d \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/DriveStrafe.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Commands/ResetGyro.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/IterativeRobot525.d \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/OI.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/RobotMap.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/Dail.d \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/Drive.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/PIDControl.d SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/Subsystems/Pneumatic.d \
	 SuitcaseBot_partialImage/$(MODE_DIR)/Objects/SuitcaseBot/SuitcaseBot.d
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
