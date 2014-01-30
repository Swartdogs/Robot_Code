#include "../AllCommands.h"

BackPickupStore::BackPickupStore() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);

	Requires(backPickup);
}

// Called just before this Command runs the first time
void BackPickupStore::Initialize() {
	backPickup->SetPickupMode(BackPickup::bStore);
}

// Called repeatedly when this Command is scheduled to run
void BackPickupStore::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupStore::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void BackPickupStore::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupStore::Interrupted() {
}
