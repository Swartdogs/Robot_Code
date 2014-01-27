#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI {
private:
	Joystick* driveJoystick;
	Joystick* tiltJoystick;
	
	JoystickButton* tiltButton1;
	JoystickButton* tiltButton2;
	JoystickButton* tiltButton3;
	JoystickButton* tiltButton4;
	JoystickButton* tiltButton5;
	JoystickButton* tiltButton6;
	JoystickButton* tiltButton7;
	JoystickButton* tiltButton8;
	JoystickButton* tiltButton9;
	JoystickButton* tiltButton10;
	JoystickButton* tiltButton11;
	
	
public:
	OI();
	float GetDriveX();
	float GetDriveY();
	float GetDriveZ();
	float GetTiltX();
	float GetTiltY();
};

#endif
