#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

#include "../CommandBase.h"

////////////////
// BACKPICKUP //
////////////////

class BackPickupManualDrive: public CommandBase {
public:
	BackPickupManualDrive();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class BackPickupDeploy: public CommandBase {
public:
	BackPickupDeploy();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class BackPickupPass: public CommandBase {
public:
	BackPickupPass();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class BackPickupStore: public CommandBase {
public:
	BackPickupStore();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
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

class FrontPickupManualLeftDrive: public CommandBase {
public:
	FrontPickupManualLeftDrive();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class FrontPickupManualRightDrive: public CommandBase {
public:
	FrontPickupManualRightDrive();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class FrontPickupMoveArmsToPosition: public CommandBase {
public:
	FrontPickupMoveArmsToPosition();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
