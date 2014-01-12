#include "RobotMap.h"

RobotMap::RobotMap(RobotLog* logDelegate) {
	robotLog = logDelegate;
	
	aiMap.driveGyro 		= 1;
	aiMap.meterPot			= 2;
	
	diMap.driveEncoderLA	= 1;
	diMap.driveEncoderLB	= 2;
	diMap.driveEncoderRA	= 3;
	diMap.driveEncoderRB	= 4;
	diMap.stopSensorRear	= 5;
	diMap.stopSensorFront	= 6;
	diMap.pressureSwitch    = 7;
	
	pwmMap.driveMotorLF		= 1;
	pwmMap.driveMotorLR		= 2;
	pwmMap.driveMotorRF		= 3;
	pwmMap.driveMotorRR		= 4;
	pwmMap.meterMotor		= 5;
	
	relayMap.airCompressor	= 1;
	
	solenoidMap.rodIn		= 1;
	solenoidMap.rodOut		= 2;
}

RobotMap::~RobotMap() {
	delete robotLog;
}

//	JOYSTICK MAP
//
//  Axis 1 LX:		Mecanuum Rotate				(Teleop)
//	Axis 2 LY:		Tank Left					(Teleop)
//	Axis 3 RX:		Arcade Rotate				(Teleop)
//					Mecanuum Strafe				(Teleop)
//	Axis 4 RY:		Tank Right					(Teleop)
//					Arcade Drive				(Teleop)
//					Mecanuum Drive				(Teleop)
//
//	Button 1:		Dial Preset 270 degrees		(Teleop)
//	Button 2:		Dial Preset 180 degrees		(Teleop)
//					Pneumatic Rod in			(Test)
//	Button 3:		Dial Preset  90 degrees		(Teleop)
//	Button 4:		Dial Preset   0 degrees		(Teleop)
//					Pneumatic Rod Out			(Test)
//					Dial Mode 					(Disabled)
//  Button 5:		Drive 48"  (w/Axis 6=+1)	(Teleop)
//	Button 6:		Rotate 180 (w/Axis 6=+1)	(Teleop)
//	Button 7:		Gyro-assisted strafing		(Teleop)
//  Button 8:
//  Button 9:		Drive with Stop Sensors		(Teleop)
//					Compressor Off				(Test)
//					Autonomous Selection		(Disabled)
//  Button 10:		Reset Gyro					(Teleop)
//					Compressor On				(Test)
//					Autonomous Delay			(Disabled)
//	Button 11:		Drive Mode					(Disabled)
//  Button 12:		Drive Power Level			(Disabled)

