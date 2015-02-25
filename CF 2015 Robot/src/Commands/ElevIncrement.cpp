#include "AllCommands.h"

ElevIncrement::ElevIncrement(Elevator::ElevDirection direction) {
	Requires(MyRobot::elevator);
	m_direction = direction;
}

void ElevIncrement::Initialize() {
	MyRobot::elevator->IncrementSetpoint(m_direction);
}

void ElevIncrement::Execute() {
}

bool ElevIncrement::IsFinished() {
	return true;
}

void ElevIncrement::End() {
}

void ElevIncrement::Interrupted() {
}
