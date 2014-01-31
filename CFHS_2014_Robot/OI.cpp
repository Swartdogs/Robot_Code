#include "OI.h"
#include "Commands/AllCommands.h"
#include "Subsystems/FrontPickup.h"

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
	tiltButton6   = new JoystickButton(tiltJoystick, 6);	// Left Front Pickup Increment Up
	tiltButton7   = new JoystickButton(tiltJoystick, 7);	// Left Front Pickup Increment Down
	tiltButton8   = new JoystickButton(tiltJoystick, 8);	// Right Front Pickup Increment Up
	tiltButton9   = new JoystickButton(tiltJoystick, 9);	// Right Front Pickup Increment Down
	tiltButton10  = new JoystickButton(tiltJoystick, 10);	// AutoFire Arm Button
	tiltButton11  = new JoystickButton(tiltJoystick, 11);	// 
	
	comboButton1  = new InternalButton();
	
	tiltButton1->WhileHeld(new FrontPickupJoystickLeft());
	tiltButton2->WhileHeld(new FrontPickupJoystickRight());
	tiltButton3->WhileHeld(new BackPickupJoystick());
	tiltButton4->WhenPressed(new BallShooterLoad());  // Will be replaced by Command Group to include actually loading a ball from either Pickup
	tiltButton5->WhenPressed(new BallShooterFire());
	
	// Front Pickup Increment Buttons
	tiltButton6->WhenPressed(new FrontPickupIncrement(FrontPickup::pLeft, true));
	tiltButton7->WhenPressed(new FrontPickupIncrement(FrontPickup::pLeft, false));
	tiltButton8->WhenPressed(new FrontPickupIncrement(FrontPickup::pRight, true));
	tiltButton9->WhenPressed(new FrontPickupIncrement(FrontPickup::pRight, false));
	
	comboButton1->WhenPressed(new AutoFire());
}

void OI::Periodic(bool autoFireArm) { // Is there a better way to do this?
	comboButton1->SetPressed(tiltButton10->Get() && autoFireArm);
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
