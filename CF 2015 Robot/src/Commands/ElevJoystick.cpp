#include "AllCommands.h"

ElevJoystick::ElevJoystick() {
	Requires(MyRobot::elevator);
}

void ElevJoystick::Initialize() {
	MyRobot::elevator->SetRunPID(false);
}

void ElevJoystick::Execute() {
	MyRobot::elevator->RunWithJoystick(MyRobot::oi->GetElevator());
}

bool ElevJoystick::IsFinished() {
	return false;
}

void ElevJoystick::End() {
	MyRobot::elevator->SetBrake(Elevator::sOn);
	MyRobot::elevator->SetRunPID(true);
}

void ElevJoystick::Interrupted() {
	MyRobot::elevator->SetRunPID(true);
}
