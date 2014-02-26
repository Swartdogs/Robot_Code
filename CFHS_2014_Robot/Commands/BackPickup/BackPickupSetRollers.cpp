#include "../AllCommands.h"

BackPickupSetRollers::BackPickupSetRollers(BackPickup::RollerMode mode) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(backPickup);
	m_mode = mode;
}

// Called just before this Command runs the first time
void BackPickupSetRollers::Initialize() {
	backPickup->SetRollerMode(m_mode);
}

// Called repeatedly when this Command is scheduled to run
void BackPickupSetRollers::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupSetRollers::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void BackPickupSetRollers::End() {
	backPickup->SetRollerMode(BackPickup::rOff);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupSetRollers::Interrupted() {
	backPickup->SetRollerMode(BackPickup::rOff);
}
