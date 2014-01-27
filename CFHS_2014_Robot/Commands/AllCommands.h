#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

#include "../CommandBase.h"

////////////////
// BACKPICKUP //
////////////////

class BackPickupPeriodic: public CommandBase {
public:
	BackPickupPeriodic();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class BackPickupSetPosition: public CommandBase {
public:
	BackPickupSetPosition(int position);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	int m_position;
};

///////////
// DRIVE //
///////////

class DriveDistance: public CommandBase {
private:
	double m_targetDistance;
	float m_maxSpeed;
	float m_targetAngle;
	bool m_resetEncoders;
	bool m_resetGyro;
	bool m_useGyro;
public:
	DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders);
	DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle, bool resetGyro);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class DriveRotate: public CommandBase {
public:
	DriveRotate(float target_angle, bool reset_gyro);
	
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
	
	float GetTargetAngle(void);
	bool GetGyroReset(void);
	
private:
	float target_angle,reset_gyro;
};

class DriveWithJoystick: public CommandBase {
public:
	DriveWithJoystick();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

//////////////////////
// IMAGE PROCESSING //
//////////////////////

class FindHotTarget: public CommandBase {
public:
	FindHotTarget();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

/////////////////
// FRONTPICKUP //
/////////////////

class FrontPickupMoveArmsToPosition: public CommandBase {
public:
	FrontPickupMoveArmsToPosition();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class FrontPickupRun: public CommandBase {
public:
	FrontPickupRun();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class FrontPickupRunLeftWheels: public CommandBase {
public:
	FrontPickupRunLeftWheels(Relay::Value value);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	Relay::Value m_value;
};

class FrontPickupRunRightWheels: public CommandBase {
public:
	FrontPickupRunRightWheels(Relay::Value value);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	Relay::Value m_value;
};

#endif
