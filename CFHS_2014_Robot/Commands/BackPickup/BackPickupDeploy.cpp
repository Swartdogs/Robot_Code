#include "../AllCommands.h"

BackPickupDeploy::BackPickupDeploy() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	
	Requires(backPickup);
}

// Called just before this Command runs the first time
void BackPickupDeploy::Initialize() {
	backPickup->SetPickupMode(BackPickup::bDeploy);
}

// Called repeatedly when this Command is scheduled to run
void BackPickupDeploy::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupDeploy::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void BackPickupDeploy::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupDeploy::Interrupted() {
	
}
