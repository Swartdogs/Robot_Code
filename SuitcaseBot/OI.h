#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI {
public:
	OI();
	
	float GetArcadeDrive();
	float GetArcadeRotate();
	bool  GetButton(UINT32 button);
	bool  GetButtonPress(UINT32 button);
	float GetMecanuumDrive();
	float GetMecanuumStrafe();
	float GetMecanuumRotate();
	float GetTankLeft();
	float GetTankRight();
	void  Periodic();

private:
	Joystick*		joystick;
	
	JoystickButton*	joyButton1;
	JoystickButton*	joyButton2;
	JoystickButton* joyButton3;
	JoystickButton* joyButton4;
	JoystickButton* joyButton5;
	JoystickButton* joyButton6;
	JoystickButton* joyButton7;
	JoystickButton* joyButton8;
	JoystickButton* joyButton9;
	JoystickButton* joyButton10;
	
	InternalButton* comboButton1;
	InternalButton* comboButton2;
	
	float ApplyDeadband(float rawValue, float Deadband);
};

#endif
