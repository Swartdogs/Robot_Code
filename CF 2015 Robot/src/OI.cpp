#include "OI.h"
#include "Commands/AllCommands.h"

OI::OI() {
	driveJoystick1 = new Joystick(0);
	driveJoystick2 = new Joystick(1);
	elevJoystick = 	 new Joystick(2);

	jbDriveStrafe = 		new JoystickButton(driveJoystick1, 1);

	jbElevDrive =  			new JoystickButton(elevJoystick, 1);
	jbElevIncrementDown = 	new JoystickButton(elevJoystick, 8);
	jbElevIncrementUp =		new JoystickButton(elevJoystick, 9);
	jbElevBinLoad =			new JoystickButton(elevJoystick, 4);
	jbElevLiftFromFloor =	new JoystickButton(elevJoystick, 3);
//	jbElevStep =			new JoystickButton(elevJoystick, 7);
//	jbElevStepUnload =		new JoystickButton(elevJoystick, 6);
//	jbElevStepTote =		new JoystickButton(elevJoystick, 10);
//	jbElevStepToteUnload =	new JoystickButton(elevJoystick, 11);
	jbElevPlatform = 		new JoystickButton(elevJoystick, 5);
	jbElevLiftFromTote =	new JoystickButton(elevJoystick, 11);
	jbElevFeederLoad = 		new JoystickButton(elevJoystick, 6);
	jbElevCarry = 			new JoystickButton(elevJoystick, 2);
	jbToteEject =			new JoystickButton(elevJoystick, 7);
	jbToteCenter =			new JoystickButton(elevJoystick, 10);

	jbDriveStrafe->WhileHeld(new DriveStrafe());

	jbElevDrive->WhileHeld(new ElevJoystick());
	jbElevIncrementDown->WhenPressed(new ElevIncrement(Elevator::dDown));
	jbElevIncrementUp->WhenPressed(new ElevIncrement(Elevator::dUp));
	jbElevLiftFromTote->WhenPressed(new ElevSetPosition(Elevator::pLiftFromTote));
    jbElevFeederLoad->WhenPressed(new ElevSetPosition(Elevator::pFeederLoad));
	jbElevCarry->WhenPressed(new ElevSetPosition(Elevator::pCarry));
	jbElevBinLoad->WhenPressed(new ElevSetPosition(Elevator::pBinLoad));
	jbElevLiftFromFloor->WhenPressed(new ElevSetPosition(Elevator::pLiftFromFloor));
//	jbElevStep->WhenPressed(new ElevSetPosition(Elevator::pStep));
//	jbElevStepUnload->WhenPressed(new ElevSetPosition(Elevator::pStepUnload));
//	jbElevStepTote->WhenPressed(new ElevSetPosition(Elevator::pStepTote));
//	jbElevStepToteUnload->WhenPressed(new ElevSetPosition(Elevator::pStepToteUnload));
	jbElevPlatform->WhenPressed(new ElevSetPosition(Elevator::pPlatform));

	jbToteEject->WhileHeld(new ElevToteEject());
	jbToteCenter->WhileHeld(new ElevToteCenter());
}

float OI::GetStrafe() {
	return ApplyDeadband(driveJoystick1->GetX(), 0.05);
}

float OI::GetDrive() {
	return -ApplyDeadband(driveJoystick1->GetY(), 0.05);
}

float OI::GetRotate() {
	float joyValue = ApplyDeadband(driveJoystick1->GetX(), 0.10);
	joyValue = (joyValue >= 0) ? joyValue * joyValue : -joyValue * joyValue;
	return joyValue;
}

float OI::GetElevator(){
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
