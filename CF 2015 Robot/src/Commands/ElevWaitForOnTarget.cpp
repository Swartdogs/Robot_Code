#include "AllCommands.h"

ElevWaitForOnTarget::ElevWaitForOnTarget() {
	Requires(MyRobot::elevator);
}

void ElevWaitForOnTarget::Initialize() {
}

void ElevWaitForOnTarget::Execute() {
}

bool ElevWaitForOnTarget::IsFinished() {
	return MyRobot::elevator->IsOnTarget();
}

void ElevWaitForOnTarget::End() {
}

void ElevWaitForOnTarget::Interrupted() {
}
