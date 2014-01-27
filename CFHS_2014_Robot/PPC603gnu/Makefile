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
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/CFHS_2014_Robot



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
PROJECT_TARGETS = CFHS_2014_Robot/$(MODE_DIR)/CFHS_2014_Robot.out \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/CFHS_2014_Robot_partialImage.o


#Rules

# CFHS_2014_Robot
ifeq ($(DEBUG_MODE),1)
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
CFHS_2014_Robot/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
CFHS_2014_Robot/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip 
CFHS_2014_Robot/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
CFHS_2014_Robot/$(MODE_DIR)/% : IDE_DEFINES = 
CFHS_2014_Robot/$(MODE_DIR)/% : PROJECT_TYPE = DKM
CFHS_2014_Robot/$(MODE_DIR)/% : DEFINES = 
CFHS_2014_Robot/$(MODE_DIR)/% : EXPAND_DBG = 0
CFHS_2014_Robot/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
CFHS_2014_Robot/$(MODE_DIR)/% : CPU = PPC603
CFHS_2014_Robot/$(MODE_DIR)/% : TOOL_FAMILY = gnu
CFHS_2014_Robot/$(MODE_DIR)/% : TOOL = gnu
CFHS_2014_Robot/$(MODE_DIR)/% : TOOL_PATH = 
CFHS_2014_Robot/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
CFHS_2014_Robot/$(MODE_DIR)/% : LIBPATH = 
CFHS_2014_Robot/$(MODE_DIR)/% : LIBS = 
CFHS_2014_Robot/$(MODE_DIR)/% : OBJ_DIR := CFHS_2014_Robot/$(MODE_DIR)

OBJECTS_CFHS_2014_Robot = CFHS_2014_Robot_partialImage/$(MODE_DIR)/CFHS_2014_Robot_partialImage.o

CFHS_2014_Robot/$(MODE_DIR)/CFHS_2014_Robot.out : $(OBJECTS_CFHS_2014_Robot)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";nmppc $(OBJECTS_CFHS_2014_Robot) $(IDE_LIBRARIES) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c ppc > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -fdollars-in-identifiers -Wall $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES)  -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X -T $(WIND_BASE)/target/h/tool/gnu/ldscripts/link.OUT -o "$@" $(OBJ_DIR)/ctdt.o $(OBJECTS_CFHS_2014_Robot) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS)  $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

CFHS_2014_Robot/$(MODE_DIR)/CFHS_2014_Robot_compile_file : $(FILE) ;

_clean :: CFHS_2014_Robot/$(MODE_DIR)/CFHS_2014_Robot_clean

CFHS_2014_Robot/$(MODE_DIR)/CFHS_2014_Robot_clean : 
	$(TRACE_FLAG)if [ -d "CFHS_2014_Robot" ]; then cd "CFHS_2014_Robot"; rm -rf $(MODE_DIR); fi


# CFHS_2014_Robot_partialImage
ifeq ($(DEBUG_MODE),1)
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : IDE_DEFINES = 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : DEFINES = 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : CPU = PPC603
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = gnu
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : TOOL = gnu
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : TOOL_PATH = 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : LIBPATH = 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : LIBS = 
CFHS_2014_Robot_partialImage/$(MODE_DIR)/% : OBJ_DIR := CFHS_2014_Robot_partialImage/$(MODE_DIR)

CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/CFHS_2014_RobotCommandBased.o : $(PRJ_ROOT_DIR)/CFHS_2014_RobotCommandBased.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/CommandBase.o : $(PRJ_ROOT_DIR)/CommandBase.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BackPickup/BackPickupPeriodic.o : $(PRJ_ROOT_DIR)/Commands/BackPickup/BackPickupPeriodic.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BackPickup/BackPickupSetPosition.o : $(PRJ_ROOT_DIR)/Commands/BackPickup/BackPickupSetPosition.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BallShooter/BallShooterFire.o : $(PRJ_ROOT_DIR)/Commands/BallShooter/BallShooterFire.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BallShooter/BallShooterLoad.o : $(PRJ_ROOT_DIR)/Commands/BallShooter/BallShooterLoad.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Drive/DriveDistance.o : $(PRJ_ROOT_DIR)/Commands/Drive/DriveDistance.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Drive/DriveRotate.o : $(PRJ_ROOT_DIR)/Commands/Drive/DriveRotate.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Drive/DriveWithJoystick.o : $(PRJ_ROOT_DIR)/Commands/Drive/DriveWithJoystick.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupManualLeftDrive.o : $(PRJ_ROOT_DIR)/Commands/FrontPickup/FrontPickupManualLeftDrive.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupManualRightDrive.o : $(PRJ_ROOT_DIR)/Commands/FrontPickup/FrontPickupManualRightDrive.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupMoveArmsToPosition.o : $(PRJ_ROOT_DIR)/Commands/FrontPickup/FrontPickupMoveArmsToPosition.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupRun.o : $(PRJ_ROOT_DIR)/Commands/FrontPickup/FrontPickupRun.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupRunLeftWheels.o : $(PRJ_ROOT_DIR)/Commands/FrontPickup/FrontPickupRunLeftWheels.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupRunRightWheels.o : $(PRJ_ROOT_DIR)/Commands/FrontPickup/FrontPickupRunRightWheels.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Misc/FindHotTarget.o : $(PRJ_ROOT_DIR)/Commands/Misc/FindHotTarget.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/IterativeRobot525.o : $(PRJ_ROOT_DIR)/IterativeRobot525.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/OI.o : $(PRJ_ROOT_DIR)/OI.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/BackPickup.o : $(PRJ_ROOT_DIR)/Subsystems/BackPickup.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/BallShooter.o : $(PRJ_ROOT_DIR)/Subsystems/BallShooter.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/Drive.o : $(PRJ_ROOT_DIR)/Subsystems/Drive.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/FindTarget.o : $(PRJ_ROOT_DIR)/Subsystems/FindTarget.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/FrontPickup.o : $(PRJ_ROOT_DIR)/Subsystems/FrontPickup.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/PIDControl.o : $(PRJ_ROOT_DIR)/Subsystems/PIDControl.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


OBJECTS_CFHS_2014_Robot_partialImage = CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/CFHS_2014_RobotCommandBased.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/CommandBase.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BackPickup/BackPickupPeriodic.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BackPickup/BackPickupSetPosition.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BallShooter/BallShooterFire.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BallShooter/BallShooterLoad.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Drive/DriveDistance.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Drive/DriveRotate.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Drive/DriveWithJoystick.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupManualLeftDrive.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupManualRightDrive.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupMoveArmsToPosition.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupRun.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupRunLeftWheels.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupRunRightWheels.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Misc/FindHotTarget.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/IterativeRobot525.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/OI.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/BackPickup.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/BallShooter.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/Drive.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/FindTarget.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/FrontPickup.o \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/PIDControl.o

CFHS_2014_Robot_partialImage/$(MODE_DIR)/CFHS_2014_Robot_partialImage.o : $(OBJECTS_CFHS_2014_Robot_partialImage)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X  -o "$@" $(OBJECTS_CFHS_2014_Robot_partialImage) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS)  && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

CFHS_2014_Robot_partialImage/$(MODE_DIR)/CFHS_2014_Robot_partialImage_compile_file : $(FILE) ;

_clean :: CFHS_2014_Robot_partialImage/$(MODE_DIR)/CFHS_2014_Robot_partialImage_clean

CFHS_2014_Robot_partialImage/$(MODE_DIR)/CFHS_2014_Robot_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "CFHS_2014_Robot_partialImage" ]; then cd "CFHS_2014_Robot_partialImage"; rm -rf $(MODE_DIR); fi

force : 

DEP_FILES := CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/CFHS_2014_RobotCommandBased.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/CommandBase.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BackPickup/BackPickupPeriodic.d \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BackPickup/BackPickupSetPosition.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BallShooter/BallShooterFire.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/BallShooter/BallShooterLoad.d \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Drive/DriveDistance.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Drive/DriveRotate.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Drive/DriveWithJoystick.d \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupManualLeftDrive.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupManualRightDrive.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupMoveArmsToPosition.d \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupRun.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupRunLeftWheels.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/FrontPickup/FrontPickupRunRightWheels.d \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Commands/Misc/FindHotTarget.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/IterativeRobot525.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/OI.d \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/BackPickup.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/BallShooter.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/Drive.d \
	 CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/FindTarget.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/FrontPickup.d CFHS_2014_Robot_partialImage/$(MODE_DIR)/Objects/CFHS_2014_Robot/Subsystems/PIDControl.d
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
