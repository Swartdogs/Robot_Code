#include "CommandBase.h"
#include "Commands/Scheduler.h"

CommandBase::CommandBase(const char *name) : Command(name) {
}

CommandBase::CommandBase() : Command() {
}

// Initialize a single static instance of all of your subsystems to NULL
Drive* CommandBase::drive = NULL;
OI* CommandBase::oi = NULL;
FindTarget* CommandBase::findTarget = NULL;
FrontPickup* CommandBase::frontPickup = NULL;
BackPickup* CommandBase::backPickup = NULL;
BallShooter* CommandBase::ballShooter = NULL;

void CommandBase::Init(RobotLog* logDelegate) {
    // Create a single static instance of all of your subsystems. The following
	// line should be repeated for each subsystem in the project.
	drive = new Drive(logDelegate);
	oi = new OI();
	findTarget = new FindTarget();
	frontPickup = new FrontPickup(logDelegate);
	backPickup = new BackPickup(logDelegate);
	ballShooter = new BallShooter();
}

void CommandBase::Periodic() {
	frontPickup->Periodic();
	backPickup->Periodic();
	ballShooter->Periodic();
	oi->Periodic(drive->CrossedTape());
}
