#include "AllCommands.h"

DriveStrafe::DriveStrafe() {
	Requires(drive);
}

void DriveStrafe::Initialize() {
	drive->InitGyroStrafe();
}

void DriveStrafe::Execute() {
	drive->GyroStrafe(oi->GetMecanuumDrive(), oi->GetMecanuumStrafe());
}

bool DriveStrafe::IsFinished() {
	return false;
}

void DriveStrafe::End() {
	drive->EndGyroUse();
}

void DriveStrafe::Interrupted() {
	drive->EndGyroUse();
}
