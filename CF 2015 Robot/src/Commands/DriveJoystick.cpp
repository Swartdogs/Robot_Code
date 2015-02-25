#include "AllCommands.h"

DriveJoystick::DriveJoystick() {
	Requires(MyRobot::drive);
}

void DriveJoystick::Initialize() {
}

void DriveJoystick::Execute() {
	MyRobot::drive->DriveWithJoystick(MyRobot::oi->GetDrive(),
									  0,
									  MyRobot::oi->GetRotate(),
									  Drive::wAll);
}

bool DriveJoystick::IsFinished() {
	return false;
}

void DriveJoystick::End() {
}

void DriveJoystick::Interrupted() {
}
