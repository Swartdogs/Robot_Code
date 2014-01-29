#include "../AllCommands.h"

BackPickupManualDrive::BackPickupManualDrive() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(backPickup);
}

// Called just before this Command runs the first time
void BackPickupManualDrive::Initialize() {
	backPickup->SetUseJoystick(true);
}

// Called repeatedly when this Command is scheduled to run
void BackPickupManualDrive::Execute() {
	backPickup->SetJoystickSpeed(oi->GetTiltY());
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupManualDrive::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void BackPickupManualDrive::End() {
	backPickup->SetUseJoystick(false);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupManualDrive::Interrupted() {
	backPickup->SetUseJoystick(false);
}
