#include "AllCommands.h"

DriveStrafe::DriveStrafe() {
	Requires(MyRobot::drive);
}

void DriveStrafe::Initialize() {
	MyRobot::drive->InitStrafeDrive();
}

void DriveStrafe::Execute() {
	MyRobot::drive->StrafeDrive(MyRobot::oi->GetDrive(),
								MyRobot::oi->GetStrafe());
}

bool DriveStrafe::IsFinished() {
	return false;
}

void DriveStrafe::End() {
}

void DriveStrafe::Interrupted() {
}
