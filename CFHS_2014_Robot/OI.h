#ifndef OI_H
#define OI_H

#include "WPILib.h"
#include "Subsystems/Drive.h"

class OI {
private:
	Drive* m_drive;
	
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
	
	InternalButton* comboButton1;
	
public:
	OI(Drive* drive);
	void  Periodic();
	float GetDriveX();
	float GetDriveY();
	float GetDriveZ();
	float GetTiltX();
	float GetTiltY();
};

#endif
