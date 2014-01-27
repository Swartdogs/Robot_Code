#include "../AllCommands.h"

FrontPickupRunLeftWheels::FrontPickupRunLeftWheels(Relay::Value value) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(frontPickup);
	
	m_value = value;
}

// Called just before this Command runs the first time
void FrontPickupRunLeftWheels::Initialize() {
	frontPickup->RunLeftWheels(m_value);
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupRunLeftWheels::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupRunLeftWheels::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void FrontPickupRunLeftWheels::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupRunLeftWheels::Interrupted() {

}
