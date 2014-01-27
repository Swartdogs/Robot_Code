#include "../AllCommands.h"

FrontPickupManualLeftDrive::FrontPickupManualLeftDrive() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(frontPickup);
}

// Called just before this Command runs the first time
void FrontPickupManualLeftDrive::Initialize() {
	frontPickup->SetUseJoystickLeft(true);
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupManualLeftDrive::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupManualLeftDrive::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void FrontPickupManualLeftDrive::End() {
	frontPickup->SetUseJoystickRight(false);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupManualLeftDrive::Interrupted() {
}
