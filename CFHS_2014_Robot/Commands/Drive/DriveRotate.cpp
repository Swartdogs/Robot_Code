#include "../AllCommands.h"

DriveRotate::DriveRotate(float target_angle, bool reset_gyro) : target_angle(target_angle) , reset_gyro(reset_gyro) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	
	Requires(drive);
}

// Quick member accessor functions.

float DriveRotate::GetTargetAngle(void) {
	return target_angle;
}

bool DriveRotate::GetGyroReset(void) {
	return reset_gyro;
}

// Called just before this Command runs the first time
void DriveRotate::Initialize() {
	drive->InitRotate(target_angle,reset_gyro);
}

// Called repeatedly when this Command is scheduled to run
void DriveRotate::Execute() {
	drive->ExecuteRotate();
}
	
// Make this return true when this Command no longer needs to run execute()
bool DriveRotate::IsFinished() {
	return drive->OnTarget();
}

// Called once after isFinished returns true
void DriveRotate::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveRotate::Interrupted() {
	
}
