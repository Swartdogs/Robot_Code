#include "../AllCommands.h"

BackPickupJoystick::BackPickupJoystick() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(backPickup);
}

// Called just before this Command runs the first time
void BackPickupJoystick::Initialize() {
	backPickup->SetUseJoystick(true);
}

// Called repeatedly when this Command is scheduled to run
void BackPickupJoystick::Execute() {
	backPickup->SetJoystickSpeed(oi->GetMcY(false));
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupJoystick::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void BackPickupJoystick::End() {
	backPickup->SetUseJoystick(false);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupJoystick::Interrupted() {
	backPickup->SetUseJoystick(false);
}
