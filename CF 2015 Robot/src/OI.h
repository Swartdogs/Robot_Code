#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI {
public:
	OI();

	float GetDrive();
	float GetElevator();
	float GetRotate();
	float GetStrafe();

private:
	Joystick* 		driveJoystick1;
	Joystick*       driveJoystick2;
	Joystick*		elevJoystick;

	JoystickButton* jbDriveStrafe;

	JoystickButton*	jbElevDrive;
	JoystickButton* jbElevIncrementDown;
	JoystickButton* jbElevIncrementUp;
	JoystickButton* jbElevCarry;
	JoystickButton* jbElevBinLoad;
	JoystickButton* jbElevLiftFromFloor;
	JoystickButton* jbElevStep;
	JoystickButton* jbElevStepUnload;
	JoystickButton* jbElevStepTote;
	JoystickButton* jbElevStepToteUnload;
	JoystickButton* jbElevPlatform;
	JoystickButton* jbElevFeederLoad;
	JoystickButton* jbElevLiftFromTote;
	JoystickButton* jbToteEject;
	JoystickButton* jbToteCenter;

	float ApplyDeadband(float rawValue, float deadband);
	float Limit(float value);
};

#endif
