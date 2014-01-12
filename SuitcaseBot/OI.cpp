#include "OI.h"
#include <math.h>
#include "Commands/AllCommands.h"

OI::OI() {
	joystick = new Joystick(1);

	joyButton1 =  new JoystickButton(joystick, 1);			
	joyButton2 =  new JoystickButton(joystick, 2);
	joyButton3 =  new JoystickButton(joystick, 3);
	joyButton4 =  new JoystickButton(joystick, 4);			// Disabled: Dial Mode
	joyButton5 =  new JoystickButton(joystick, 5);
	joyButton6 =  new JoystickButton(joystick, 6);
	
	joyButton7 =  new JoystickButton(joystick, 7);	
	joyButton8 =  new JoystickButton(joystick, 8);
	joyButton9 =  new JoystickButton(joystick, 9);			// Disabled: Autonomous Select
	joyButton10 = new JoystickButton(joystick, 10);			// Disabled: Autonomous Delay
	
	comboButton1 = new InternalButton();
	comboButton2 = new InternalButton();
		
	joyButton1->WhenPressed(new DialSetTarget(270));
	joyButton2->WhenPressed(new DialSetTarget(180));
	joyButton3->WhenPressed((new DialSetTarget(85)));
	joyButton4->WhenPressed(new DialSetTarget(0));
	
	joyButton7->WhileHeld(new DriveStrafe());
	joyButton8->WhileHeld(new DriveRearRotate());
	joyButton9->WhileHeld(new DriveStopSensor(0.4));
	joyButton10->WhenPressed(new ResetGyro());
	
	comboButton1->WhenPressed(new DriveDistance(48, 0.5, true, 0.0, true));
	comboButton2->WhenPressed(new DriveRotate(180, true));
}

float OI::GetArcadeDrive() {				// Right Y Axis
	return ApplyDeadband(-joystick->GetRawAxis(4), 0.05);
}

float OI::GetArcadeRotate() {				// Right X Axis
	return ApplyDeadband(-joystick->GetRawAxis(3), 0.05);
}

bool OI::GetButton(UINT32 button) {
	return joystick->GetRawButton(button);
}

bool OI::GetButtonPress(UINT32 button) {
	static int pressedButtons = 0;
	
    int  buttonValue = 1 << (button - 1);
	int  vReturn = false;
	
    if (joystick->GetRawButton(button)) {
    	vReturn = ((pressedButtons & buttonValue) == 0);
    	pressedButtons |= buttonValue;
    
    } else if ((pressedButtons & buttonValue) != 0) {
    	pressedButtons ^= buttonValue;
    }
    
    return vReturn;
}

float OI::GetMecanuumDrive() {				// Right Y Axis
	return ApplyDeadband(-joystick->GetRawAxis(4), 0.05);
}

float OI::GetMecanuumStrafe() {				// Right X Axis
	return ApplyDeadband(joystick->GetRawAxis(3), 0.05);
}

float OI::GetMecanuumRotate() {				// Left X Axis
	return ApplyDeadband(joystick->GetRawAxis(1), 0.10);
}

float OI::GetTankLeft() {					// Left Y Axis
	return ApplyDeadband(-joystick->GetRawAxis(2), 0.05);
}

float OI::GetTankRight() {					// Right Y Axis
	return ApplyDeadband(-joystick->GetRawAxis(4), 0.05);
}

void OI::Periodic() {
	comboButton1->SetPressed(joystick->GetRawAxis(6) > 0.5 && fabs(joystick->GetRawAxis(5)) < 0.5 && joyButton5->Get());
	comboButton2->SetPressed(joystick->GetRawAxis(6) > 0.5 && fabs(joystick->GetRawAxis(5)) < 0.5 && joyButton6->Get());
}

// PRIVATE FUNCTIONS

float OI::ApplyDeadband(float rawValue, float deadband) {
	if (rawValue > 1.0) {
		rawValue = 1.0;
	} else if (rawValue < -1.0) {
		rawValue = -1.0;
	}
	
	if(fabs(rawValue) < deadband) return 0;
	if(rawValue > 0)			  return (rawValue - deadband) / (1.0 - deadband);
								  return (rawValue + deadband) / (1.0 - deadband);
}

