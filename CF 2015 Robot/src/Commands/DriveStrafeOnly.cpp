#include "AllCommands.h"

DriveStrafeOnly::DriveStrafeOnly() {
	Requires(MyRobot::drive);
}

void DriveStrafeOnly::Initialize() {
	MyRobot::drive->InitStrafeDrive();
}

void DriveStrafeOnly::Execute() {
	MyRobot::drive->StrafeDrive(0, MyRobot::oi->GetStrafe());
}

bool DriveStrafeOnly::IsFinished() {
	return false;
}

void DriveStrafeOnly::End() {
}

void DriveStrafeOnly::Interrupted() {
}
