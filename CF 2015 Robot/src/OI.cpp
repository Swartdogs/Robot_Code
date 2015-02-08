#include "OI.h"
#include "Commands/AllCommands.h"

OI::OI() {
	driveJoystick = new Joystick(0);
	elevJoystick = 	new Joystick(1);

	jbDriveRotate = new JoystickButton(driveJoystick, 11);
	jbElevDrive =  	new JoystickButton(elevJoystick, 1);

	jbDriveRotate->WhenPressed(new DriveResetEncoder());
	jbElevDrive->WhileHeld(new ElevJoystick());
}

float OI::GetDriveX() {
	return ApplyDeadband(driveJoystick->GetX(), 0.05);
}

float OI::GetDriveY() {
	return -ApplyDeadband(driveJoystick->GetY(), 0.05);
}

float OI::GetDriveZ() {
	return ApplyDeadband(driveJoystick->GetZ(), 0.10);
}

float OI::GetElevY(){
	return ApplyDeadband(elevJoystick->GetY(), 0.05);
}

// ******************** PRIVATE ********************

float OI::ApplyDeadband(float rawValue, float deadband) {
	rawValue = Limit(rawValue);

	if(fabs(rawValue) < deadband) return 0.0;
	if(rawValue > 0)			  return (rawValue - deadband) / (1.0 - deadband);
								  return (rawValue + deadband) / (1.0 - deadband);
}

float OI::Limit(float value) {
	if(value > 1.0)  return 1.0;
	if(value < -1.0) return -1.0;
					 return value;
}
