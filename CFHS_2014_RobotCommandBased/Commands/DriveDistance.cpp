#include "DriveDistance.h"

DriveDistance::DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders) {
	m_targetDistance = targetDistance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_targetAngle = 0;
	m_resetGyro = false;
}

DriveDistance::DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle, bool resetGyro) {
	m_targetDistance = targetDistance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_targetAngle = 0;
	m_resetGyro = false;	
}

// Called just before this Command runs the first time
void DriveDistance::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void DriveDistance::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool DriveDistance::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void DriveDistance::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveDistance::Interrupted() {
}
