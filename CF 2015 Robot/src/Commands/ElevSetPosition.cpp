#include "AllCommands.h"

ElevSetPosition::ElevSetPosition(Elevator::ElevPosition position) {
	Requires(MyRobot::elevator);
	m_position = position;

}

void ElevSetPosition::Initialize() {
	MyRobot::elevator->SetElevPosition(m_position);
}

void ElevSetPosition::Execute() {
	MyRobot::elevator->RunWithPID(false);
}

bool ElevSetPosition::IsFinished() {
	return true;
}

void ElevSetPosition::End() {
}

void ElevSetPosition::Interrupted() {
}
