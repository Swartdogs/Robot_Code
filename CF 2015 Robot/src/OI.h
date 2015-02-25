#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI {
public:
	OI();

	bool	GetCameraButton();
	float 	GetDrive();
	float 	GetElevator();
	float 	GetRotate();
	float 	GetStrafe();

private:
	Joystick* 		driveJoystick;
	Joystick*		elevJoystick;
	Joystick*       buttonBox;

	JoystickButton* jbDriveStrafe1;
	JoystickButton* jbDriveStrafe2;
	JoystickButton* jbDriveStrafeOnly1;
	JoystickButton* jbDriveStrafeOnly2;
	JoystickButton* jbDriveSetMark;
	JoystickButton* jbDriveRotateToMark;

	JoystickButton*	jbElevDrive;
	JoystickButton* jbElevStep;
	JoystickButton* jbElevStepUnload;
	JoystickButton* jbToteEject1;
	JoystickButton* jbToteEject2;
	JoystickButton* jbElevStepToteUnload;
	JoystickButton* jbElevStepTote;

	JoystickButton* jbElevFeederLoad;
	JoystickButton* jbElevLiftFromTote;
	JoystickButton* jbElevLiftFromFloor;
	JoystickButton* jbElevBinLoad;
	JoystickButton* jbElevIncrementUp;
	JoystickButton* jbElevIncrementDown;
	JoystickButton* jbElevBinRelease;
	JoystickButton* jbElevCarry;
	JoystickButton* jbToteCenter;

	float ApplyDeadband(float rawValue, float deadband);
	float Limit(float value);
};

#endif
