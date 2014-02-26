#include "CommandBase.h"
#include "Commands/Scheduler.h"

CommandBase::CommandBase(const char *name) : Command(name) {
}

CommandBase::CommandBase() : Command() {
}

Drive* 			CommandBase::drive = NULL;
FindTarget* 	CommandBase::findTarget = NULL;
FrontPickup*	CommandBase::frontPickup = NULL;
BackPickup*		CommandBase::backPickup = NULL;
BallShooter* 	CommandBase::ballShooter = NULL;
OI* 			CommandBase::oi = NULL;

void CommandBase::Init(RobotLog* logDelegate) {
	drive = 		new Drive(logDelegate);
	findTarget = 	new FindTarget(logDelegate);
	ballShooter = 	new BallShooter(logDelegate);
	frontPickup = 	new FrontPickup(logDelegate);
	backPickup = 	new BackPickup(logDelegate);
	oi = 			new OI(drive);
}

void CommandBase::Periodic() {
	frontPickup->Periodic();
	backPickup->Periodic();
	ballShooter->Periodic();
	oi->Periodic();
}

void CommandBase::StopMotors() {
	drive->StopMotors();
	frontPickup->StopMotors();
	backPickup->StopMotors();
	ballShooter->StopMotors();
}
