#include "AllCommands.h"

DriveDistance::DriveDistance(double distance, float maxSpeed, bool resetEncoders, bool useBrake, float angle, Drive::AngleFrom angleFrom) {
	Requires(MyRobot::drive);

	m_distance = distance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_useBrake = useBrake;
	m_angle = angle;
	m_angleFrom = angleFrom;
}

void DriveDistance::Initialize() {
	MyRobot::drive->InitDistance(m_distance, m_maxSpeed, m_resetEncoders, m_useBrake, m_angle, m_angleFrom);
}

void DriveDistance::Execute() {
	MyRobot::drive->ExecuteDistance(false);
}

bool DriveDistance::IsFinished() {
	return MyRobot::drive->IsOnTarget();
}

void DriveDistance::End() {
	MyRobot::drive->StopMotors();
}

void DriveDistance::Interrupted() {
	MyRobot::drive->StopMotors();
}
