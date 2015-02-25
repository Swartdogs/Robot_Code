#include "OI.h"
#include "Commands/AllCommands.h"

OI::OI() {
	driveJoystick = new Joystick(0);
	elevJoystick = 	new Joystick(1);
	buttonBox = 	new Joystick(2);

	jbDriveStrafe1= 		new JoystickButton(driveJoystick, 1);
	jbDriveStrafe2= 		new JoystickButton(driveJoystick, 11);
	jbDriveStrafeOnly1 =	new JoystickButton(driveJoystick, 3);
	jbDriveStrafeOnly2 =	new JoystickButton(driveJoystick, 10);
	jbDriveSetMark =		new JoystickButton(driveJoystick, 4);
	jbDriveRotateToMark =	new JoystickButton(driveJoystick, 5);

	jbElevDrive =  			new JoystickButton(elevJoystick, 1);
	jbElevStep =			new JoystickButton(elevJoystick, 6);
	jbElevStepUnload =		new JoystickButton(elevJoystick, 7);
	jbToteEject1 =			new JoystickButton(elevJoystick, 8);
	jbToteEject2 =			new JoystickButton(elevJoystick, 9);
	jbElevStepToteUnload =	new JoystickButton(elevJoystick, 10);
	jbElevStepTote =		new JoystickButton(elevJoystick, 11);

	jbElevFeederLoad = 		new JoystickButton(buttonBox, 1);
	jbElevLiftFromTote =	new JoystickButton(buttonBox, 2);
	jbElevLiftFromFloor =	new JoystickButton(buttonBox, 3);
	jbElevBinLoad =			new JoystickButton(buttonBox, 4);
	jbElevIncrementUp =		new JoystickButton(buttonBox, 5);
	jbElevIncrementDown = 	new JoystickButton(buttonBox, 6);
	jbElevBinRelease =		new JoystickButton(buttonBox, 7);
	jbElevCarry = 			new JoystickButton(buttonBox, 9);
	jbToteCenter =			new JoystickButton(buttonBox, 10);

	jbDriveStrafe1->WhileHeld		(new DriveStrafe());
	jbDriveStrafe2->WhileHeld		(new DriveStrafe());
	jbDriveStrafeOnly1->WhileHeld	(new DriveStrafeOnly());
	jbDriveStrafeOnly2->WhileHeld	(new DriveStrafeOnly());
//	jbDriveTest->WhenPressed(new AutoToteBin(0.0));
//	jbDriveTest->WhenPressed(new Auto3Tote());

	jbElevDrive->WhileHeld			(new ElevJoystick());
	jbElevStep->WhenPressed			(new ElevSetPosition(Elevator::pStep));
	jbElevStepUnload->WhenPressed	(new ElevSetPosition(Elevator::pStepUnload));
	jbToteEject1->WhileHeld			(new ElevToteEject());
	jbToteEject2->WhileHeld			(new ElevToteEject());
	jbElevStepToteUnload->WhenPressed(new ElevSetPosition(Elevator::pStepToteUnload));
	jbElevStepTote->WhenPressed		(new ElevSetPosition(Elevator::pStepTote));

    jbElevFeederLoad->WhenPressed	(new ElevSetPosition(Elevator::pFeederLoad));
	jbElevLiftFromTote->WhenPressed	(new ElevSetPosition(Elevator::pLiftFromTote));
	jbElevLiftFromFloor->WhenPressed(new ElevSetPosition(Elevator::pLiftFromFloor));
	jbElevBinLoad->WhenPressed		(new ElevSetPosition(Elevator::pBinLoad));
	jbElevIncrementUp->WhenPressed	(new ElevIncrement(Elevator::dUp));
	jbElevIncrementDown->WhenPressed(new ElevIncrement(Elevator::dDown));
	jbElevBinRelease->WhenPressed	(new ElevSetPosition(Elevator::pBinRelease));
	jbElevCarry->WhenPressed		(new ElevSetPosition(Elevator::pCarry));
	jbToteCenter->WhileHeld			(new ElevToteCenter());
}

bool OI::GetCameraButton() {
	return false;
//	return driveJoystick->GetRawButton(11);
}

float OI::GetStrafe() {
	return ApplyDeadband(driveJoystick->GetX(), 0.05);
}

float OI::GetDrive() {
	return -ApplyDeadband(driveJoystick->GetY(), 0.05);
}

float OI::GetRotate() {
	float joyValue = ApplyDeadband(driveJoystick->GetX(), 0.10);
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
