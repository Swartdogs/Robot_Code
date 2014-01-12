#include "AllCommands.h"

DriveDistance::DriveDistance(double distance, float maxSpeed, bool resetEncoders) {
	Requires(drive);
	m_targetDistance = distance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_targetAngle = 0;
	m_resetGyro = m_useGyro = false;
}

DriveDistance::DriveDistance(double distance, float maxSpeed, bool resetEncoders, float angle, bool resetGyro) {
	Requires(drive);
	m_targetDistance = distance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_targetAngle = angle;
	m_resetGyro = resetGyro;
	m_useGyro = true;
}

void DriveDistance::Initialize() {
	if (m_useGyro) {
		drive->InitDistance(m_targetDistance, m_maxSpeed, m_resetEncoders, m_targetAngle, m_resetGyro);
	} else {
		drive->InitDistance(m_targetDistance, m_maxSpeed, m_resetEncoders);
	}
}

void DriveDistance::Execute() {
	drive->ExecuteDistance();
}

bool DriveDistance::IsFinished() {
	return drive->OnTarget();
}

void DriveDistance::End() {
	drive->StopEncoders();
	drive->EndGyroUse();
}

void DriveDistance::Interrupted() {
	drive->StopEncoders();
	drive->EndGyroUse();
}
