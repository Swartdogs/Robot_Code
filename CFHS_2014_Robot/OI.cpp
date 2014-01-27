#include "OI.h"
#include "Commands/AllCommands.h"

OI::OI() {
	// Process operator interface input here.
	driveJoystick = new Joystick(1);
	tiltJoystick  = new Joystick(2);
	
	// Forgot most of the mappings, so make sure to change!
	// Also didn't want to mess around with light sensor stuff
	
	tiltButton1   = new JoystickButton(tiltJoystick, 1);	// Left Front Pickup Enable
	tiltButton2   = new JoystickButton(tiltJoystick, 2);	// Right Front Pickup Enable
	tiltButton3   = new JoystickButton(tiltJoystick, 3);	// Back Pickup Enable
	tiltButton4   = new JoystickButton(tiltJoystick, 4);	// Ball Shooter Load
	tiltButton5   = new JoystickButton(tiltJoystick, 5);	// Ball Shooter Fire
	tiltButton6   = new JoystickButton(tiltJoystick, 6);	// 
	tiltButton7   = new JoystickButton(tiltJoystick, 7);	// 
	tiltButton8   = new JoystickButton(tiltJoystick, 8);	// 
	tiltButton9   = new JoystickButton(tiltJoystick, 9);	// 
	tiltButton10  = new JoystickButton(tiltJoystick, 10);	// 
	tiltButton11  = new JoystickButton(tiltJoystick, 11);	// 
	
	tiltButton1->WhileHeld(new FrontPickupManualLeftDrive());
	tiltButton2->WhileHeld(new FrontPickupManualRightDrive());
	
	tiltButton4->WhenPressed(new BallShooterLoad());  // Will be replaced by Command Group to include actually loading a ball from either Pickup
	tiltButton5->WhenPressed(new BallShooterFire());
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

float OI::GetTiltX() {
	return tiltJoystick->GetX();
}

float OI::GetTiltY() {
	return tiltJoystick->GetY();
}
