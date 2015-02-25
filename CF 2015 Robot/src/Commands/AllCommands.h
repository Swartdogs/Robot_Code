#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

#include "../MyRobot.h"
#include "WPILib.h"
#include "../Subsystems/Drive.h"
#include "../Subsystems/Elevator.h"

class DriveAutoStrafe: public Command {
public:
	DriveAutoStrafe(double timeout, float strafe, float angle, Drive::AngleFrom angleFrom);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	double 				m_timeout;
	float  				m_strafe;
	float				m_angle;
	Drive::AngleFrom	m_angleFrom;
};

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

class DriveDistanceMoveElev: public Command {
public:
	DriveDistanceMoveElev(double distance, float maxSpeed, bool resetEncoders, bool useBrake, float angle, Drive::AngleFrom angleFrom,
						  double triggerDistance, Elevator::ElevPosition position);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	double					m_distance;
	float					m_maxSpeed;
	bool					m_resetEncoders;
	bool            	    m_useBrake;
	float					m_angle;
	Drive::AngleFrom		m_angleFrom;
	double					m_triggerDistance;
	Elevator::ElevPosition	m_elevPosition;
	bool					m_atTrigger;
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
	DriveRotate(float angle, Drive::AngleFrom angleFrom, float maxPWM);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	float				m_angle;
	Drive::AngleFrom	m_angleFrom;
	float				m_maxPWM;
};

class DriveSetMarkAngle: public Command {
public:
	DriveSetMarkAngle();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
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

class DriveStrafeOnly: public Command {
public:
	DriveStrafeOnly();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

class ElevAutoCenter: public Command {
public:
	ElevAutoCenter();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

class ElevCancel: public Command {
public:
	ElevCancel();
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

class ElevWaitForOnTarget: public Command {
public:
	ElevWaitForOnTarget();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

// ******************* Command Groups ********************

class Auto1Tote: public CommandGroup {
public:
	Auto1Tote(double wait);
};

class Auto3Tote: public CommandGroup {
public:
	Auto3Tote(double wait);
};

class AutoBinPosition: public CommandGroup {
public:
	AutoBinPosition(double wait);
};

class AutoBinToFeeder: public CommandGroup {
public:
	AutoBinToFeeder(double wait);
};

class AutoBinRobot: public CommandGroup {
public:
	AutoBinRobot(double wait);
};

class AutoRobot: public CommandGroup {
public:
	AutoRobot(double wait);
};

class AutoToteBinLeft: public CommandGroup {
public:
	AutoToteBinLeft(double wait);
};

class AutoToteBinRight: public CommandGroup {
public:
	AutoToteBinRight(double wait);
};

#endif
