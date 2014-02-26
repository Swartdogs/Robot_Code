#include "../AllCommands.h"

PickupCatch::PickupCatch() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(frontPickup);
	Requires(backPickup);
}

// Called just before this Command runs the first time
void PickupCatch::Initialize() {
	frontPickup->SetPickupMode(FrontPickup::fCatch);
	backPickup->SetPickupMode(BackPickup::bCatch);
}

// Called repeatedly when this Command is scheduled to run
void PickupCatch::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool PickupCatch::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void PickupCatch::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void PickupCatch::Interrupted() {
}
