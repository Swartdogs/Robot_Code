#include "../AllCommands.h"

DriveResetEncoders::DriveResetEncoders() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(drive);
}

// Called just before this Command runs the first time
void DriveResetEncoders::Initialize() {
	drive->EncoderTestEnabled(true);
}

// Called repeatedly when this Command is scheduled to run
void DriveResetEncoders::Execute() {
	drive->DriveArcade(oi->GetDriveY(), oi->GetDriveZ());
}

// Make this return true when this Command no longer needs to run execute()
bool DriveResetEncoders::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void DriveResetEncoders::End() {
	drive->EncoderTestEnabled(false);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveResetEncoders::Interrupted() {
	drive->EncoderTestEnabled(false);
}
