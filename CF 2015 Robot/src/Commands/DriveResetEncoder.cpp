#include "AllCommands.h"

DriveResetEncoder::DriveResetEncoder() {
	Requires(MyRobot::drive);
}

void DriveResetEncoder::Initialize() {
	MyRobot::drive->ResetEncoders();
}

void DriveResetEncoder::Execute() {
}

bool DriveResetEncoder::IsFinished() {
	return true;
}

void DriveResetEncoder::End() {
}

void DriveResetEncoder::Interrupted() {
}
