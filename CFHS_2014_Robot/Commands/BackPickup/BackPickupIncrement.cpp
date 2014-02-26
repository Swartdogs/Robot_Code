#include "../AllCommands.h"

BackPickupIncrement::BackPickupIncrement(BackPickup::AdjustMode mode) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	
	m_mode = mode;
	
	Requires(backPickup);
}

// Called just before this Command runs the first time
void BackPickupIncrement::Initialize() {
	printf("BackPickup Increment Command %d\n", m_mode);
	backPickup->IncrementArm(m_mode);
}

// Called repeatedly when this Command is scheduled to run
void BackPickupIncrement::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupIncrement::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void BackPickupIncrement::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupIncrement::Interrupted() {
}
