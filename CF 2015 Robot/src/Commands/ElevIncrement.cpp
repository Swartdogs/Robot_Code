#include "AllCommands.h"

ElevIncrement::ElevIncrement(Elevator::ElevDirection direction) {
	Requires(MyRobot::elevator);
	m_direction = direction;
}

void ElevIncrement::Initialize() {
	MyRobot::elevator->IncrementSetpoint(m_direction);
}

void ElevIncrement::Execute() {
	MyRobot::elevator->RunWithPID(false);
}

bool ElevIncrement::IsFinished() {
	return true;
}

void ElevIncrement::End() {
}

void ElevIncrement::Interrupted() {
}
