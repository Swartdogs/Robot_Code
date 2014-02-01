#include "../AllCommands.h"

DriveToLowGoal::DriveToLowGoal() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(drive);
	m_distanceToLowGoal = 7.0;
}

// Called just before this Command runs the first time
void DriveToLowGoal::Initialize() {
	m_distanceToLowGoal = drive->GetRange() - m_distanceToLowGoal;
	drive->InitDistance(m_distanceToLowGoal, 0.5, true, 0);
}

// Called repeatedly when this Command is scheduled to run
void DriveToLowGoal::Execute() {
	drive->ExecuteDistance();
}

// Make this return true when this Command no longer needs to run execute()
bool DriveToLowGoal::IsFinished() {
	return drive->OnTarget();
}

// Called once after isFinished returns true
void DriveToLowGoal::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveToLowGoal::Interrupted() {
}
