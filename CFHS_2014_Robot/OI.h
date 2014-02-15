#ifndef OI_H
#define OI_H

#include "WPILib.h"
#include "Subsystems/Drive.h"

class OI {
private:
	Drive* m_drive;
	
	Joystick* driveJoystick;
	Joystick* mcJoystick[2];
	
	JoystickButton* driveButton1;
	JoystickButton* driveButton2;
	JoystickButton* driveButton3;
	JoystickButton* driveButton11;
	
	JoystickButton* mcJoy1Button1;
	JoystickButton* mcJoy1Button2;
	JoystickButton* mcJoy1Button3;
	JoystickButton* mcJoy1Button4;
	JoystickButton* mcJoy1Button5;
	JoystickButton* mcJoy1Button6;
	JoystickButton* mcJoy1Button7;
	JoystickButton* mcJoy1Button8;
	JoystickButton* mcJoy1Button9;
	JoystickButton* mcJoy1Button10;
	JoystickButton* mcJoy1Button11;
	
	JoystickButton* mcJoy2Button1;
	JoystickButton* mcJoy2Button2;
	JoystickButton* mcJoy2Button3;
	JoystickButton* mcJoy2Button4;
	JoystickButton* mcJoy2Button5;
	JoystickButton* mcJoy2Button6;
	JoystickButton* mcJoy2Button7;
	JoystickButton* mcJoy2Button8;
	JoystickButton* mcJoy2Button9;
	JoystickButton* mcJoy2Button10;
	JoystickButton* mcJoy2Button11;
	
	InternalButton* comboButton1;
	InternalButton* comboButton2;
	InternalButton* comboButton3;
	
public:
	OI(Drive* drive);
	
	void  Periodic();
	bool  GetButtonPress(UINT32 button);
	float GetDriveX();
	float GetDriveY();
	float GetDriveZ();
	float GetMcX(bool left);
	float GetMcY(bool left);
};

#endif
