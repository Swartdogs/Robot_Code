#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

#include "../MyRobot.h"
#include "WPILib.h"
#include "../Subsystems/Drive.h"
#include "../Subsystems/Elevator.h"

class DriveDistance: public Command {
public:
	DriveDistance(double distance, float maxSpeed, bool resetEncoders, bool useBrake, float angle, Drive::AngleFrom angleFrom);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	double				m_distance;
	float				m_maxSpeed;
	bool				m_resetEncoders;
	bool                m_useBrake;
	float				m_angle;
	Drive::AngleFrom	m_angleFrom;
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

class DriveRotate: public Command {
public:
	DriveRotate(float angle, Drive::AngleFrom angleFrom);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	float				m_angle;
	Drive::AngleFrom	m_angleFrom;
};

class DriveStrafe: public Command {
public:
	DriveStrafe();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

class ElevIncrement: public Command {
public:
	ElevIncrement(Elevator::ElevDirection direction);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
private:
	Elevator::ElevDirection m_direction;
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

class ElevSetPosition: public Command {
public:
	ElevSetPosition(Elevator::ElevPosition position);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	Elevator::ElevPosition m_position;
};

class ElevToteCenter: public Command {
public:
	ElevToteCenter();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

class ElevToteEject: public Command {
public:
	ElevToteEject();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
