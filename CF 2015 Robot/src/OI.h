#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI {
public:
	OI();

	float GetDriveX();
	float GetDriveY();
	float GetDriveZ();
	float GetElevY();

private:
	Joystick* 		driveJoystick;
	Joystick*		elevJoystick;

	JoystickButton*	jbDriveRotate;
	JoystickButton*	jbElevDrive;

	float ApplyDeadband(float rawValue, float deadband);
	float Limit(float value);
};

#endif
