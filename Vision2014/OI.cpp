#include "OI.h"
#include "Commands/VisionButton.h"
#include "Commands/FindButton.h"
#include "Commands/DriveJoystick.h"


OI::OI() {
	// Process operator interface input here.
	joystick = new Joystick(1);
	
	joyButton1 = new JoystickButton(joystick, 1);
	joyButton11 = new JoystickButton(joystick, 11);
	joyButton12 = new JoystickButton(joystick, 12);
	
	joyButton1->WhileHeld(new DriveJoystick());
	joyButton11->WhenPressed(new VisionButton());
	joyButton12->WhenPressed(new FindButton());
	
	
}

float OI::GetJoyX() {
	return joystick->GetX();
}

float OI::GetJoyY() {
	return joystick->GetY();
}
