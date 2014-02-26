#include "../AllCommands.h"

FrontPickupJoystickRight::FrontPickupJoystickRight() {
	Requires(frontPickup);
}

// Called just before this Command runs the first time
void FrontPickupJoystickRight::Initialize() {
	frontPickup->SetUseJoystickRight(true);
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupJoystickRight::Execute() {
	frontPickup->SetJoystickRight(-oi->GetMcY(false));
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupJoystickRight::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void FrontPickupJoystickRight::End() {
	frontPickup->SetUseJoystickRight(false);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupJoystickRight::Interrupted() {
	frontPickup->SetUseJoystickRight(false);
}
