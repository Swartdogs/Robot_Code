#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

#include "../CommandBase.h"

////////////////
// BACKPICKUP //
////////////////

class BackPickupJoystick: public CommandBase {
public:
	BackPickupJoystick();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class BackPickupSetMode: public CommandBase {
public:
	BackPickupSetMode(BackPickup::BackMode mode);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	BackPickup::BackMode m_pickupMode;
};

/////////////////
// BALLSHOOTER //
/////////////////

class BallShooterFire: public CommandBase {
public:
	BallShooterFire();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class BallShooterLoad: public CommandBase {
public:
	BallShooterLoad();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
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

class FrontPickupJoystickLeft: public CommandBase {
public:
	FrontPickupJoystickLeft();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class FrontPickupJoystickRight: public CommandBase {
public:
	FrontPickupJoystickRight();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
