#include "AllCommands.h"

ElevJoystick::ElevJoystick() {
	Requires(MyRobot::elevator);
}

void ElevJoystick::Initialize() {
	MyRobot::elevator->SetBrake(Elevator::bOff);
}

void ElevJoystick::Execute() {
	MyRobot::elevator->RunWithJoystick(MyRobot::oi->GetElevY());
}

bool ElevJoystick::IsFinished() {
	return false;
}

void ElevJoystick::End() {
	MyRobot::elevator->SetBrake(Elevator::bOn);
}

void ElevJoystick::Interrupted() {
	MyRobot::elevator->SetBrake(Elevator::bOn);
}
