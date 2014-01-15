#include "FindButton.h"

FindButton::FindButton() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(findTarget);
}

// Called just before this Command runs the first time
void FindButton::Initialize() {
	printf("\nFind Target\n");
	findTarget->FindOne();
}

// Called repeatedly when this Command is scheduled to run
void FindButton::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool FindButton::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void FindButton::End() {
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FindButton::Interrupted() {
}
