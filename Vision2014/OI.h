#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI {
private:
	Joystick*		joystick;
	
	JoystickButton* joyButton1;
	JoystickButton*	joyButton11;
	JoystickButton*	joyButton12;
	
public:
	OI();
	
	float GetJoyX();
	float GetJoyY();
};

#endif
