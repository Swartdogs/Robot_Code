#include "../AllCommands.h"

DriveRangeDetect::DriveRangeDetect() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(drive);
}

// Called just before this Command runs the first time
void DriveRangeDetect::Initialize() {
	drive->SetRangeMode(Drive::rOn);
}

// Called repeatedly when this Command is scheduled to run
void DriveRangeDetect::Execute() {
	drive->RangeDetectExecute(oi->GetDriveY(), oi->GetDriveZ());
}

// Make this return true when this Command no longer needs to run execute()
bool DriveRangeDetect::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void DriveRangeDetect::End() {
	drive->SetRangeMode(Drive::rOff);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveRangeDetect::Interrupted() {
	drive->SetRangeMode(Drive::rOff);
}
