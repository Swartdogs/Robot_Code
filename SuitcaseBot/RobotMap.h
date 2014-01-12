#include "WPILib.h"
#include "RobotLog.h"

#ifndef ROBOTMAP_H
#define ROBOTMAP_H

typedef struct{
	INT32 driveGyro;
	INT32 meterPot;
}AImap;

typedef struct{
	INT32 driveEncoderLA;
	INT32 driveEncoderLB;
	INT32 driveEncoderRA;
	INT32 driveEncoderRB;
	INT32 stopSensorFront;
	INT32 stopSensorRear;
	INT32 pressureSwitch;
}DImap;

typedef struct{
	INT32 driveMotorLF;
	INT32 driveMotorLR;
	INT32 driveMotorRF;
	INT32 driveMotorRR;
	INT32 meterMotor;
}PWMmap;

typedef struct{
	INT32 airCompressor;
}Relaymap;

typedef struct{
	INT32 rodOut;
	INT32 rodIn;
}Solenoidmap;

class RobotMap {
public:
	RobotMap(RobotLog* logDelegate);
	~RobotMap();
	
	RobotLog*	robotLog;

	AImap		aiMap;
	DImap		diMap;
	PWMmap		pwmMap;
	Relaymap	relayMap;
	Solenoidmap	solenoidMap;
};

#endif
