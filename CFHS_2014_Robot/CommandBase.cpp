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
INIParser*      CommandBase::iniParser = NULL;

void CommandBase::Init(RobotLog* logDelegate) {
	iniParser =     new INIParser("525Init.ini");
	drive = 		new Drive(logDelegate);
	findTarget = 	new FindTarget(logDelegate);
	frontPickup = 	new FrontPickup(logDelegate);
	backPickup = 	new BackPickup(logDelegate);
	ballShooter = 	new BallShooter(logDelegate);
	oi = 			new OI(drive);
}

void CommandBase::Periodic() {
	frontPickup->Periodic();
	backPickup->Periodic();
	ballShooter->Periodic();
	oi->Periodic();
	
//	printf("Left Arm: %d, Right Arm: %d, Shooter: %d, Back: %d\n", frontPickup->GetPosition(FrontPickup::pLeft), frontPickup->GetPosition(FrontPickup::pRight), ballShooter->GetShooterPosition(), backPickup->GetPosition());
}

void CommandBase::StopMotors() {
	drive->StopMotors();
	frontPickup->StopMotors();
	backPickup->StopMotors();
	ballShooter->StopMotors();
}
