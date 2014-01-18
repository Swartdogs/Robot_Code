#include "OI.h"

OI::OI() {
	// Process operator interface input here.
	stick = new Joystick(1);
}

float OI::GetMove() {
	return stick->GetY();
}

float OI::GetRotate() {
	return -(stick->GetZ());
}
