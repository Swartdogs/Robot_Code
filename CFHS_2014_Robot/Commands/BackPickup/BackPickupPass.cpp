#include "../AllCommands.h"

BackPickupPass::BackPickupPass() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	
	Requires(backPickup);
}

// Called just before this Command runs the first time
void BackPickupPass::Initialize() {
	backPickup->SetPickupMode(BackPickup::bPass);
}

// Called repeatedly when this Command is scheduled to run
void BackPickupPass::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupPass::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void BackPickupPass::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupPass::Interrupted() {
}
