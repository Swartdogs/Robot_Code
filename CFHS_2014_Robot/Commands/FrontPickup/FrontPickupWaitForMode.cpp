#include "../AllCommands.h"

FrontPickupWaitForMode::FrontPickupWaitForMode(FrontPickup::FrontMode mode) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(frontPickup);
	
	m_mode = mode;
}

// Called just before this Command runs the first time
void FrontPickupWaitForMode::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupWaitForMode::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupWaitForMode::IsFinished() {
	return (frontPickup->GetFrontPickupMode() == m_mode && frontPickup->OnTarget());
}

// Called once after isFinished returns true
void FrontPickupWaitForMode::End() {
	printf("Wait For FrontPickup End\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupWaitForMode::Interrupted() {
}
