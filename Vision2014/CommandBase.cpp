#include "CommandBase.h"
#include "Subsystems/VisionTarget.h"
#include "Commands/Scheduler.h"

CommandBase::CommandBase(const char *name) : Command(name) {
}

CommandBase::CommandBase() : Command() {
}

// Initialize a single static instance of all of your subsystems to NULL
FindTarget*	  CommandBase::findTarget = NULL;
VisionTarget* CommandBase::visionTarget = NULL;
OI* 		  CommandBase::oi = NULL;

void CommandBase::init() {
    // Create a single static instance of all of your subsystems. The following
	// line should be repeated for each subsystem in the project.
	findTarget = new FindTarget();
	visionTarget = new VisionTarget();
	
	oi = new OI();
}
