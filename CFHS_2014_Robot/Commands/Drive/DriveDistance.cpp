#include "../AllCommands.h"

DriveDistance::DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders) {
	m_targetDistance = targetDistance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_targetAngle = 0;
	m_resetGyro = false;
	m_currentInitMode = mRelNoGyro;
	Requires(drive);
}

DriveDistance::DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle, bool resetGyro) {
	m_targetDistance = targetDistance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_targetAngle = targetAngle;
	m_resetGyro = resetGyro;
	m_currentInitMode = mRelGyro;
	Requires(drive);
}

DriveDistance::DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders, float absoluteAngle) {
	m_targetDistance = targetDistance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_targetAngle = absoluteAngle;
	m_currentInitMode = mAbsolute;
	Requires(drive);
}

// Called just before this Command runs the first time
void DriveDistance::Initialize() {
	
	switch (m_currentInitMode) {
	case mRelGyro:
		drive->InitDistance(m_targetDistance, m_maxSpeed, m_resetEncoders, m_targetAngle, m_resetGyro);
		break;
	case mRelNoGyro:
		drive->InitDistance(m_targetDistance, m_maxSpeed, m_resetEncoders);
		break;
	case mAbsolute:
		drive->InitDistance(m_targetDistance, m_maxSpeed, m_resetEncoders, m_targetAngle);
		break;
	}
}

// Called repeatedly when this Command is scheduled to run
void DriveDistance::Execute() {
	drive->ExecuteDistance();
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
