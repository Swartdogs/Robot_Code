#include "AllCommands.h"

DriveDistanceMoveElev::DriveDistanceMoveElev(double distance, float maxSpeed, bool resetEncoders, bool useBrake, float angle,
											 Drive::AngleFrom angleFrom, double triggerDistance, Elevator::ElevPosition position) {
	Requires(MyRobot::drive);

	m_distance = distance;
	m_maxSpeed = maxSpeed;
	m_resetEncoders = resetEncoders;
	m_useBrake = useBrake;
	m_angle = angle;
	m_angleFrom = angleFrom;
	m_triggerDistance = triggerDistance;
	m_elevPosition = position;
	m_atTrigger = false;
}

void DriveDistanceMoveElev::Initialize() {
	MyRobot::drive->InitDistance(m_distance, m_maxSpeed, m_resetEncoders, m_useBrake, m_angle, m_angleFrom, m_triggerDistance);
}

void DriveDistanceMoveElev::Execute() {
	MyRobot::drive->ExecuteDistance(false);

	if (!m_atTrigger) {
		if (MyRobot::drive->IsAtTrigger()) {
			m_atTrigger = true;
			MyRobot::elevator->SetElevPosition(m_elevPosition);
		}
	}
}

bool DriveDistanceMoveElev::IsFinished() {
	return MyRobot::drive->IsOnTarget();
}

void DriveDistanceMoveElev::End() {
	MyRobot::drive->StopMotors();
}

void DriveDistanceMoveElev::Interrupted() {
	MyRobot::drive->StopMotors();
}
