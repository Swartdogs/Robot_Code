#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI {
private:
	Joystick* driveJoystick;
public:
	OI();
	float GetDriveX();
	float GetDriveY();
	float GetDriveZ();
};

#endif
