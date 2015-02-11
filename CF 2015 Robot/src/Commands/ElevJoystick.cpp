#include "AllCommands.h"

ElevJoystick::ElevJoystick() {
	Requires(MyRobot::elevator);
}

void ElevJoystick::Initialize() {
}

void ElevJoystick::Execute() {
	MyRobot::elevator->RunWithJoystick(MyRobot::oi->GetElevator());
}

bool ElevJoystick::IsFinished() {
	return false;
}

void ElevJoystick::End() {
	MyRobot::elevator->SetBrake(Elevator::sOn);
}

void ElevJoystick::Interrupted() {
	MyRobot::elevator->SetBrake(Elevator::sOn);
}
