#include "AllCommands.h"

DriveStopSensor::DriveStopSensor(float maxSpeed) {
	Requires(drive);
	m_maxSpeed = maxSpeed;
}

void DriveStopSensor::Initialize() {
	drive->InitStopSensor(m_maxSpeed);
}

void DriveStopSensor::Execute() {
	drive->ExecuteStopSensor();
}

bool DriveStopSensor::IsFinished() {
	return false;
}

void DriveStopSensor::End() {
	drive->EndGyroUse();
}

void DriveStopSensor::Interrupted() {
	drive->EndGyroUse();
}
