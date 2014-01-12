#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

#include "../CommandBase.h"

class DialSetTarget: public CommandBase {
public:
	DialSetTarget(float target);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	float m_target;
};

class DriveDistance: public CommandBase {
public:
	DriveDistance(double distance, float maxSpeed, bool resetEncoders);
	DriveDistance(double distance, float maxSpeed, bool resetEncoders, float angle, bool resetGyro);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	float	m_maxSpeed;
	bool	m_resetEncoders;
	bool	m_resetGyro;
	float	m_targetAngle;
	double 	m_targetDistance;
	bool	m_useGyro;
};

class DriveJoystick: public CommandBase {
public:
	DriveJoystick();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class DriveRearRotate: public CommandBase {
public:
	DriveRearRotate();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class DriveRotate: public CommandBase {
public:
	DriveRotate(float angle, bool resetGyro);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	float	m_targetAngle;
	bool	m_resetGyro;
};

class DriveStopSensor: public CommandBase {
public:
	DriveStopSensor(float maxSpeed);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	float 	m_maxSpeed;
};

class DriveStrafe: public CommandBase {
public:
	DriveStrafe();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class ResetGyro: public CommandBase {
public:
	ResetGyro();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

//  ********** COMMAND GROUPS **********

class AutoSquare: public CommandGroup {
public:	
	AutoSquare(double wait, double distance);
};

class AutoSquareDial: public CommandGroup {
public:	
	AutoSquareDial(double wait, double distance);
};

#endif
