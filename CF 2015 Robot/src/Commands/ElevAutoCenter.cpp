#include "AllCommands.h"

ElevAutoCenter::ElevAutoCenter() {
	Requires(MyRobot::elevator);
}

void ElevAutoCenter::Initialize() {
	MyRobot::elevator->SetToteCenter(Elevator::sOn);
	SetTimeout(0.15);
}

void ElevAutoCenter::Execute() {
}

bool ElevAutoCenter::IsFinished() {
	return IsTimedOut();
}

void ElevAutoCenter::End() {
	MyRobot::elevator->SetToteCenter(Elevator::sOff);
}

void ElevAutoCenter::Interrupted() {
	MyRobot::elevator->SetToteCenter(Elevator::sOff);
}
