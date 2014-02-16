#include "../AllCommands.h"

DriveResetGyro::DriveResetGyro() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(drive);
}

// Called just before this Command runs the first time
void DriveResetGyro::Initialize() {
	drive->ResetGyro();
}

// Called repeatedly when this Command is scheduled to run
void DriveResetGyro::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool DriveResetGyro::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void DriveResetGyro::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveResetGyro::Interrupted() {
}
