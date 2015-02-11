#include "AllCommands.h"

ElevToteEject::ElevToteEject() {
	Requires(MyRobot::elevator);
}

void ElevToteEject::Initialize() {
	MyRobot::elevator->SetToteEject(Elevator::sOn);
}

void ElevToteEject::Execute() {
	MyRobot::elevator->StopMotors();
}

bool ElevToteEject::IsFinished() {
	return false;
}

void ElevToteEject::End() {
	MyRobot::elevator->SetToteEject(Elevator::sOff);
}

void ElevToteEject::Interrupted() {
	MyRobot::elevator->SetToteEject(Elevator::sOff);
}
