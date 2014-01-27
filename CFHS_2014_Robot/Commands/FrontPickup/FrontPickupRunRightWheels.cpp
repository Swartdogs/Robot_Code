#include "../AllCommands.h"

FrontPickupRunRightWheels::FrontPickupRunRightWheels(Relay::Value value) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(frontPickup);
	
	m_value = value;
}

// Called just before this Command runs the first time
void FrontPickupRunRightWheels::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupRunRightWheels::Execute() {
	frontPickup->RunRightWheels(m_value);
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupRunRightWheels::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void FrontPickupRunRightWheels::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupRunRightWheels::Interrupted() {

}
