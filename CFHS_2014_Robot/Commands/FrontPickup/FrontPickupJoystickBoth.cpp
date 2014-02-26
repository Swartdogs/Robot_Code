#include "../AllCommands.h"

FrontPickupJoystickBoth::FrontPickupJoystickBoth() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(frontPickup);
}

// Called just before this Command runs the first time
void FrontPickupJoystickBoth::Initialize() {
	frontPickup->SetUseJoystickLeft(true);
	frontPickup->SetUseJoystickRight(true);
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupJoystickBoth::Execute() {
	frontPickup->SetJoystickLeft(-oi->GetMcY(true));
	frontPickup->SetJoystickRight(-oi->GetMcY(false));
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupJoystickBoth::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void FrontPickupJoystickBoth::End() {
	frontPickup->SetUseJoystickLeft(false);
	frontPickup->SetUseJoystickRight(false);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupJoystickBoth::Interrupted() {
	frontPickup->SetUseJoystickLeft(false);
	frontPickup->SetUseJoystickRight(false);
}
