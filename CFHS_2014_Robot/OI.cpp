#include "OI.h"
#include "Commands/AllCommands.h"
#include "Subsystems/FrontPickup.h"

OI::OI(Drive* drive) {
	m_drive = drive;
	
	driveJoystick =  new Joystick(1);
	mcJoystick[0]  = new Joystick(2);
	mcJoystick[1] =  new Joystick(3);
	
	driveButton1  = new JoystickButton(driveJoystick, 1);
	driveButton2  = new JoystickButton(driveJoystick, 2);
	driveButton3  = new JoystickButton(driveJoystick, 3);
	driveButton7  = new JoystickButton(driveJoystick, 7);
	driveButton11 = new JoystickButton(driveJoystick, 11);
	driveButton12 = new JoystickButton(driveJoystick, 12);
	
	mcJoy1Button1   = new JoystickButton(mcJoystick[0], 1);		// Left Front Pickup Enable
	mcJoy1Button2   = new JoystickButton(mcJoystick[0], 2);		// Right Front Pickup Enable
	mcJoy1Button3   = new JoystickButton(mcJoystick[0], 3);		// Back Pickup Enable
	mcJoy1Button4   = new JoystickButton(mcJoystick[0], 4);		// Ball Shooter Load
	mcJoy1Button5   = new JoystickButton(mcJoystick[0], 5);		// Ball Shooter Fire
	mcJoy1Button6   = new JoystickButton(mcJoystick[0], 6);		// Left Front Pickup Increment Up
	mcJoy1Button7   = new JoystickButton(mcJoystick[0], 7);		// Left Front Pickup Increment Down
	mcJoy1Button8   = new JoystickButton(mcJoystick[0], 8);		// Right Front Pickup Increment Up
	mcJoy1Button9   = new JoystickButton(mcJoystick[0], 9);		// Right Front Pickup Increment Down
	mcJoy1Button10  = new JoystickButton(mcJoystick[0], 10);	// AutoFire Arm Button
	mcJoy1Button11  = new JoystickButton(mcJoystick[0], 11);	// 

	mcJoy2Button1   = new JoystickButton(mcJoystick[1], 1);		// Left Front Pickup Enable
	mcJoy2Button2   = new JoystickButton(mcJoystick[1], 2);		// Right Front Pickup Enable
	mcJoy2Button3   = new JoystickButton(mcJoystick[1], 3);		// Back Pickup Enable
	mcJoy2Button4   = new JoystickButton(mcJoystick[1], 4);		// Ball Shooter Load
	mcJoy2Button5   = new JoystickButton(mcJoystick[1], 5);		// Ball Shooter Fire
	mcJoy2Button6   = new JoystickButton(mcJoystick[1], 6);		// Left Front Pickup Increment Up
	mcJoy2Button7   = new JoystickButton(mcJoystick[1], 7);		// Left Front Pickup Increment Down
	mcJoy2Button8   = new JoystickButton(mcJoystick[1], 8);		// Right Front Pickup Increment Up
	mcJoy2Button9   = new JoystickButton(mcJoystick[1], 9);		// Right Front Pickup Increment Down
	mcJoy2Button10  = new JoystickButton(mcJoystick[1], 10);	// AutoFire Arm Button
	mcJoy2Button11  = new JoystickButton(mcJoystick[1], 11);	// 
	
	comboButton1    = new InternalButton();
	comboButton2    = new InternalButton();
	comboButton3    = new InternalButton();
	joyLeft 		= new InternalButton();
	joyRight		= new InternalButton();
	joyBoth			= new InternalButton();
	
//----------------------------------------------- Testing Scheme ----------------------------------------------------//
	
//	driveButton1->WhileHeld(new DriveRangeDetect());
//	driveButton1->WhenPressed(new BallShooterFire());
//	driveButton11->WhileHeld(new DriveTapeDetect());
//	driveButton7->WhenPressed(new DriveResetGyro());
//	driveButton2->WhenPressed(new FindHotTarget());
//	driveButton3->WhileHeld(new DriveRangeDetect());
		
	
//---------------------------------------- One Joystick/Button Box Scheme -------------------------------------------//
	
	/*
	mcJoy1Button1->WhenPressed(new BackPickupSetMode(BackPickup::bPass));			// Pass From BackPickup
	mcJoy1Button2->WhenPressed(new BackPickupSetMode(BackPickup::bStore));			// Store BackPickup
	mcJoy1Button3->WhenPressed(new BackPickupSetMode(BackPickup::bDeploy));			// Deploy BackPickup
	mcJoy1Button4->WhenPressed(new FrontPickupSetMode(FrontPickup::fLoad));			// Load From FrontPickup
	mcJoy1Button6->WhenPressed(new FrontPickupSetMode(FrontPickup::fStore));		// Store FrontPickup
	mcJoy1Button7->WhenPressed(new FrontPickupSetMode(FrontPickup::fDeploy));		// Deploy FrontPickup
	mcJoy1Button8->WhenPressed(new BallShooterFire());								// Fire BallShooter
	mcJoy1Button9->WhileHeld(new BackPickupSetRollers(BackPickup::rIn));
	
	mcJoy1Button10->WhenPressed(new FrontPickupSetMode(FrontPickup::fLowDeploy));	// Low Deploy FrontPickup
	mcJoy1Button11->WhileHeld(new FrontPickupSetRollers(FrontPickup::wOut));		// Shoot FrontPickup
	*/
	
//--------------------------------------------- Competition Joystick Scheme -------------------------------------------------//
	
	driveButton1->WhenPressed(new BallShooterFire());
	driveButton2->WhileHeld(new DriveRangeDetect());
	driveButton7->WhenPressed(new DriveResetGyro());
	driveButton11->WhenPressed(new PickupCatch());
	driveButton12->WhileHeld(new DriveTapeDetect());
	
	mcJoy1Button1->WhenPressed(new BackPickupIncrement(BackPickup::aDown));
	mcJoy1Button2->WhenPressed(new BackPickupIncrement(BackPickup::aUp));
	mcJoy1Button4->WhenPressed(new FrontPickupIncrement(FrontPickup::pLeft, false));
	mcJoy1Button5->WhenPressed(new FrontPickupIncrement(FrontPickup::pLeft, true));
	mcJoy1Button6->WhenPressed(new BackPickupSetMode(BackPickup::bPass));
	mcJoy1Button7->WhenPressed(new BackPickupSetMode(BackPickup::bStore));
	mcJoy1Button8->WhenPressed(new BackPickupSetMode(BackPickup::bDeploy));
	mcJoy1Button10->WhenPressed(new FrontPickupSetMode(FrontPickup::fLoad));
//	mcJoy1Button11->WhenPressed();
	joyLeft->WhileHeld(new FrontPickupJoystickLeft());
	
//	mcJoy2Button1->WhenPressed(new BackPickupIncrement(BackPickup::aUp));
	mcJoy2Button2->WhileHeld(new BackPickupJoystick());
	mcJoy2Button4->WhenPressed(new FrontPickupIncrement(FrontPickup::pRight, false));
	mcJoy2Button5->WhenPressed(new FrontPickupIncrement(FrontPickup::pRight, true));
	mcJoy2Button6->WhileHeld(new FrontPickupSetRollers(FrontPickup::wOut));
	mcJoy2Button7->WhenPressed(new FrontPickupSetMode(FrontPickup::fLowDeploy));
	mcJoy2Button9->WhenPressed(new FrontPickupSetMode(FrontPickup::fDeploy));
	mcJoy2Button10->WhenPressed(new FrontPickupSetMode(FrontPickup::fStore));
	mcJoy2Button11->WhileHeld(new BackPickupSetRollers(BackPickup::rManualIn));
//	mcJoy1Button11->WhenPressed();
	joyRight->WhileHeld(new FrontPickupJoystickRight());
	
	comboButton2->WhenPressed(new BallShooterFire());
	//comboButton3->WhileHeld(new BackPickupJoystick());
	joyBoth->WhileHeld(new FrontPickupJoystickBoth());
	
}

void OI::Periodic() { 
//  comboButton1->SetPressed(mcJoy1Button10->Get() && m_drive->CrossedTape() );
	comboButton2->SetPressed(mcJoy1Button9->Get() && mcJoy2Button8->Get());
//  comboButton3->SetPressed(mcJoy1Button2->Get() || mcJoy2Button2->Get());
	joyLeft->SetPressed(mcJoy1Button3->Get() && !mcJoy2Button3->Get());
	joyRight->SetPressed(!mcJoy1Button3->Get() && mcJoy2Button3->Get());
	joyBoth->SetPressed(mcJoy1Button3->Get() && mcJoy2Button3->Get());
}

bool OI::GetButtonPress(UINT32 button) {
	static int pressedButtons = 0;
	
    int  buttonValue = 1 << (button - 1);
	int  vReturn = false;
	
    if (mcJoystick[0]->GetRawButton(button)) {
    	vReturn = ((pressedButtons & buttonValue) == 0);
    	pressedButtons |= buttonValue;
    
    } else if ((pressedButtons & buttonValue) != 0) {
    	pressedButtons ^= buttonValue;
    }
    
    return vReturn;
}

float OI::GetDriveX() {
	return driveJoystick->GetX();
}

float OI::GetDriveY() {
	return -driveJoystick->GetY();
}

float OI::GetDriveZ() {
	return -driveJoystick->GetZ();
}

float OI::GetMcX(bool left) {
	if(left) return mcJoystick[0]->GetX();
	return mcJoystick[1]->GetX();
}

float OI::GetMcY(bool left) {
	if(left) return -mcJoystick[0]->GetY();
	return -mcJoystick[1]->GetY();
}
