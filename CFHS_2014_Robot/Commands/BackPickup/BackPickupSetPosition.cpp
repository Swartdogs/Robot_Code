#include "../AllCommands.h"

BackPickupSetPosition::BackPickupSetPosition(int position) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(backPickup);
	m_position = position;
}

// Called just before this Command runs the first time
void BackPickupSetPosition::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void BackPickupSetPosition::Execute() {
	backPickup->SetToPosition(m_position);
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupSetPosition::IsFinished() {
	return backPickup->OnTarget();
}

// Called once after isFinished returns true
void BackPickupSetPosition::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupSetPosition::Interrupted() {
}
