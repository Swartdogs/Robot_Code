#include "AllCommands.h"

DriveJoystick::DriveJoystick() {
	Requires(drive);
}

void DriveJoystick::Initialize() {
	drive->SetSafetyEnabled(true);
}

void DriveJoystick::Execute() {
	switch(drive->GetDriveMode()) {		// Get current Drive Type and call corresponding function
		case Drive::dTank: {		
			drive->DriveTank(oi->GetTankLeft(), oi->GetTankRight());
		} break;
	
		case Drive::dArcade: {
			drive->DriveArcade(oi->GetArcadeDrive(), oi->GetArcadeRotate());
		} break;
		
		case Drive::dMecanuum: {
			drive->DriveMecanuum(oi->GetMecanuumDrive(), oi->GetMecanuumStrafe(), oi->GetMecanuumRotate(), Drive::dAll);
		} break;
	}
}

bool DriveJoystick::IsFinished() {
	return false;
}

void DriveJoystick::End() {
}

void DriveJoystick::Interrupted() {
}
