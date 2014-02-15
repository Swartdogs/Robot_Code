#include "../AllCommands.h"

DriveRotate::DriveRotate(float absoluteAngle) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	
	Requires(drive);
	
	m_angle = absoluteAngle;
	m_setRelativeZero = false;
	m_rotateMode = rAbsolute;
}

DriveRotate::DriveRotate(float relativeAngle, bool setRelativeZero) {
	Requires(drive);
	
	m_angle = relativeAngle;
	m_setRelativeZero = setRelativeZero;
	m_rotateMode = rRelative;
}

// Called just before this Command runs the first time
void DriveRotate::Initialize() {
	switch(m_rotateMode) {
		case rAbsolute:
			drive->InitRotate(m_angle);
			break;
		case rRelative:
			drive->InitRotate(m_angle, m_setRelativeZero);
			break;
		default:;
	}
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
