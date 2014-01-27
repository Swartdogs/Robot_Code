#include "../AllCommands.h"

FrontPickupRun::FrontPickupRun() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(frontPickup);
}

// Called just before this Command runs the first time
void FrontPickupRun::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupRun::Execute() {
	frontPickup->Run();
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupRun::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void FrontPickupRun::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupRun::Interrupted() {
}
