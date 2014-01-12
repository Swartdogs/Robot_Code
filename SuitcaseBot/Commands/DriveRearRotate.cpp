#include "AllCommands.h"

DriveRearRotate::DriveRearRotate() {
	Requires(drive);
}

void DriveRearRotate::Initialize() {
	drive->SetSafetyEnabled(true);
}

void DriveRearRotate::Execute() {
	if (drive->GetDriveMode() == Drive::dMecanuum) {
		drive->MecanuumDrive(0, 0, oi->GetMecanuumRotate(), Drive::dRear);
	} else {
		drive->DriveArcade(0, 0);
	}
}

bool DriveRearRotate::IsFinished() {
	return false;
}

void DriveRearRotate::End() {
}

void DriveRearRotate::Interrupted() {
}
