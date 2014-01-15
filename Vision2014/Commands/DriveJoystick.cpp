#include "DriveJoystick.h"

DriveJoystick::DriveJoystick() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(findTarget);
}

// Called just before this Command runs the first time
void DriveJoystick::Initialize() {
	printf("Drive Init\n");
}

// Called repeatedly when this Command is scheduled to run
void DriveJoystick::Execute() {
	findTarget->DriveTest(oi->GetJoyY(), oi->GetJoyX());
}

// Make this return true when this Command no longer needs to run execute()
bool DriveJoystick::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void DriveJoystick::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveJoystick::Interrupted() {
}
