#include "DriveDistance.h"

DriveDistance::DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders) {
	m_targetDistance = targetDistance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_targetAngle = 0;
	m_resetGyro = false;
	m_useGyro = false;
	Requires(drive);
}

DriveDistance::DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle, bool resetGyro) {
	m_targetDistance = targetDistance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_targetAngle = targetAngle;
	m_resetGyro = resetGyro;
	m_useGyro = true;
	Requires(drive);
}

// Called just before this Command runs the first time
void DriveDistance::Initialize() {
	if(m_useGyro) {
		drive->InitDistance(m_targetDistance, m_maxSpeed, m_resetEncoders, m_targetAngle, m_resetGyro);
	} else {
		drive->InitDistance(m_targetDistance, m_maxSpeed, m_resetEncoders);
	}
}

// Called repeatedly when this Command is scheduled to run
void DriveDistance::Execute() {
	drive->ExecuteRotate();
}

// Make this return true when this Command no longer needs to run execute()
bool DriveDistance::IsFinished() {
	return drive->OnTarget();
}

// Called once after isFinished returns true
void DriveDistance::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveDistance::Interrupted() {
}
