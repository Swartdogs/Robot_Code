#include "AllCommands.h"

DriveRotate::DriveRotate(float angle, Drive::AngleFrom from) {
	Requires(MyRobot::drive);
	m_angle = angle;
	m_angleFrom = from;
}

void DriveRotate::Initialize() {
	MyRobot::drive->InitRotate(m_angle, m_angleFrom);
}


void DriveRotate::Execute() {
	MyRobot::drive->ExecuteRotate(false);
}

bool DriveRotate::IsFinished() {
	return MyRobot::drive->IsOnTarget();
}

void DriveRotate::End() {
	MyRobot::drive->StopMotors();
}

void DriveRotate::Interrupted() {
	MyRobot::drive->StopMotors();
}
