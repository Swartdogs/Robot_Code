#include "../AllCommands.h"

FrontPickupJoystickLeft::FrontPickupJoystickLeft() {
	Requires(frontPickup);
}

// Called just before this Command runs the first time
void FrontPickupJoystickLeft::Initialize() {
	frontPickup->SetUseJoystickLeft(true);
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupJoystickLeft::Execute() {
	frontPickup->SetJoystickLeft(-oi->GetMcY(true));
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupJoystickLeft::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void FrontPickupJoystickLeft::End() {
	frontPickup->SetUseJoystickLeft(false);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupJoystickLeft::Interrupted() {
	frontPickup->SetUseJoystickLeft(false);
}
