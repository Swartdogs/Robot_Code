#include "../AllCommands.h"

DriveTapeDetect::DriveTapeDetect() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(drive);
}

// Called just before this Command runs the first time
void DriveTapeDetect::Initialize() {
	drive->SetTapeMode(Drive::tArm);
}

// Called repeatedly when this Command is scheduled to run
void DriveTapeDetect::Execute() {
	drive->TapeDetectExecute(oi->GetDriveY(), oi->GetDriveZ());
}

// Make this return true when this Command no longer needs to run execute()
bool DriveTapeDetect::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void DriveTapeDetect::End() {
	drive->SetTapeMode(Drive::tOff);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveTapeDetect::Interrupted() {
	drive->SetTapeMode(Drive::tOff);
}
