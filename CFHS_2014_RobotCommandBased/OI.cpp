#include "OI.h"

OI::OI() {
	// Process operator interface input here.
	driveJoystick = new Joystick(1);
}

float OI::GetDriveX() {
	return driveJoystick->GetX();
}

float OI::GetDriveY() {
	return -driveJoystick->GetY();
}

float OI::GetDriveZ() {
	return driveJoystick->GetZ();
}
