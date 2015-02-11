#include "AllCommands.h"

ElevToteCenter::ElevToteCenter() {
	Requires(MyRobot::elevator);
}

void ElevToteCenter::Initialize() {
	MyRobot::elevator->SetToteCenter(Elevator::sOn);
}

void ElevToteCenter::Execute() {
	MyRobot::elevator->StopMotors();
}

bool ElevToteCenter::IsFinished() {
	return false;
}

void ElevToteCenter::End() {
	MyRobot::elevator->SetToteCenter(Elevator::sOff);
}

void ElevToteCenter::Interrupted() {
	MyRobot::elevator->SetToteCenter(Elevator::sOff);
}
