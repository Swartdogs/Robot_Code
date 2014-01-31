#include "../AllCommands.h"

FrontPickupIncrement::FrontPickupIncrement(FrontPickup::Pot arm, bool up) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	m_arm = arm;
	m_up = up;
	
	Requires(frontPickup);
}

// Called just before this Command runs the first time
void FrontPickupIncrement::Initialize() {
	frontPickup->IncrementArm(m_arm, m_up);
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupIncrement::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupIncrement::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void FrontPickupIncrement::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupIncrement::Interrupted() {
}
