#include "AllCommands.h"

DriveRotate::DriveRotate(float angle, bool resetGyro) {
	Requires(drive);
	m_targetAngle = angle;
	m_resetGyro = resetGyro;
}

void DriveRotate::Initialize() {
	drive->InitRotate(m_targetAngle, m_resetGyro);
}

void DriveRotate::Execute() {
	drive->ExecuteRotate();
}

bool DriveRotate::IsFinished() {
	return drive->OnTarget();
}

void DriveRotate::End() {
	drive->EndGyroUse();
}

void DriveRotate::Interrupted() {
	drive->EndGyroUse();
}
