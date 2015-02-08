#include "AllCommands.h"

ElevPID::ElevPID() {
	Requires(MyRobot::elevator);
}

void ElevPID::Initialize() {

}

void ElevPID::Execute() {
	MyRobot::elevator->RunWithPID(false);
}

bool ElevPID::IsFinished() {
	return false;
}

void ElevPID::End() {

}

void ElevPID::Interrupted() {

}
