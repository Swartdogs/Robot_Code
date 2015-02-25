#include "AllCommands.h"

DriveAutoStrafe::DriveAutoStrafe(double timeout, float strafe, float angle, Drive::AngleFrom angleFrom) {
	Requires(MyRobot::drive);
	m_timeout = timeout;
	m_strafe = strafe;
	m_angle = angle;
	m_angleFrom = angleFrom;
}

void DriveAutoStrafe::Initialize() {
	MyRobot::drive->InitRotate(m_angle, m_angleFrom, 0.7);
	SetTimeout(m_timeout);
}


void DriveAutoStrafe::Execute() {
	MyRobot::drive->StrafeDrive(0, m_strafe);
}

bool DriveAutoStrafe::IsFinished() {
	return IsTimedOut();
}

void DriveAutoStrafe::End() {
	MyRobot::drive->StopMotors();
}

void DriveAutoStrafe::Interrupted() {
	MyRobot::drive->StopMotors();
}
