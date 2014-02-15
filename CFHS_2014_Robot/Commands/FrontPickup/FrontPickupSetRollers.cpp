#include "../AllCommands.h"

FrontPickupSetRollers::FrontPickupSetRollers(FrontPickup::RollerMode mode) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	m_rollerMode = mode;
	Requires(frontPickup);
}

// Called just before this Command runs the first time
void FrontPickupSetRollers::Initialize() {
	frontPickup->SetRollers(m_rollerMode);
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupSetRollers::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupSetRollers::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void FrontPickupSetRollers::End() {
	frontPickup->SetRollers(FrontPickup::wOff);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupSetRollers::Interrupted() {
	frontPickup->SetRollers(FrontPickup::wOff);
}
