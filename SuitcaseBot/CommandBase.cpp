#include "CommandBase.h"
#include "Commands/Scheduler.h"

CommandBase::CommandBase(const char *name) : Command(name) {
}

CommandBase::CommandBase() : Command() {
}

// Initialize a single static instance of all of your subsystems to NULL

RobotMap*	CommandBase::robotMap = NULL;
Dial*		CommandBase::dial = NULL;
Drive* 		CommandBase::drive = NULL;
OI* 		CommandBase::oi = NULL;
Pneumatic*  CommandBase::pneumatic = NULL;

void CommandBase::Init(RobotLog* logDelegate) {
	robotMap = 	new RobotMap(logDelegate);
	
	dial =		new Dial(robotMap);
	drive = 	new Drive(robotMap);
	pneumatic = new Pneumatic(robotMap);
	
	oi = 		new OI();
}
