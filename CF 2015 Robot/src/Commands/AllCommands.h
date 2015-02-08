#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

#include "../MyRobot.h"
#include "WPILib.h"
#include "../Subsystems/Drive.h"

class DriveDistance: public Command {
private:
	double				m_distance;
	float				m_maxSpeed;
	bool				m_resetEncoders;
	bool                m_useBrake;
	float				m_angle;
	Drive::AngleFrom	m_angleFrom;

public:
	DriveDistance(double distance, float maxSpeed, bool resetEncoders, bool useBrake, float angle, Drive::AngleFrom angleFrom);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

class DriveJoystick: public Command {
public:
	DriveJoystick();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

class DriveResetEncoder: public Command {
public:
	DriveResetEncoder();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

class ElevJoystick: public Command {
public:
	ElevJoystick();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

class ElevPID: public Command {
public:
	ElevPID();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

};

class DriveRotate: public Command {
private:
	float				m_angle;
	Drive::AngleFrom	m_angleFrom;

public:
	DriveRotate(float angle, Drive::AngleFrom angleFrom);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};


#endif
