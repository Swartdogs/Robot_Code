#include "BackPickupPeriodic.h"

BackPickupPeriodic::BackPickupPeriodic() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(backPickup);
}

// Called just before this Command runs the first time
void BackPickupPeriodic::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void BackPickupPeriodic::Execute() {
	backPickup->Periodic();
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupPeriodic::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void BackPickupPeriodic::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupPeriodic::Interrupted() {
}
