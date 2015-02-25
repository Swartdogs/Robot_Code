#include "AllCommands.h"

ElevCancel::ElevCancel() {
	Requires(MyRobot::elevator);
}

void ElevCancel::Initialize() {
	MyRobot::elevator->StopMotors();
}

void ElevCancel::Execute() {
}

bool ElevCancel::IsFinished() {
	return true;
}

void ElevCancel::End() {
}

void ElevCancel::Interrupted() {
}
