#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

#include "../CommandBase.h"

////////////////
// BACKPICKUP //
////////////////

class BackPickupIncrement: public CommandBase {
public:
	BackPickupIncrement(BackPickup::AdjustMode mode);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	BackPickup::AdjustMode m_mode;
};

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

class BackPickupSetRollers: public CommandBase {
public:
	BackPickupSetRollers(BackPickup::RollerMode mode);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	BackPickup::RollerMode m_mode;
};

class BackPickupWaitForMode: public CommandBase {
public:
	BackPickupWaitForMode(BackPickup::BackMode mode);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	BackPickup::BackMode m_mode;
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

class BallShooterReset: public CommandBase {
public:
	BallShooterReset();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class BallShooterWaitForMode: public CommandBase {
public:
	BallShooterWaitForMode(BallShooter::ShootState mode);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	BallShooter::ShootState m_mode;
};

///////////
// DRIVE //
///////////

class DriveDistance: public CommandBase {
private:
	typedef enum {mRelNoGyro,mRelGyro,mAbsolute} InitMode;
	
	InitMode m_currentInitMode;
	
	double	m_targetDistance;
	double 	m_fireDistance;
	float 	m_maxSpeed;
	float 	m_targetAngle;
	bool 	m_resetEncoders;
	bool 	m_resetGyro;
	bool 	m_useGyro;
	
public:
	DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders, double fireDistance);
	DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders, double fireDistance, float relativeAngle, bool resetGyro);
	DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders, double fireDistance, float absoluteAngle);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class DriveRangeDetect: public CommandBase {
public:
	DriveRangeDetect();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class DriveResetEncoders: public CommandBase {
public:
	DriveResetEncoders();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class DriveResetGyro: public CommandBase {
public:
	DriveResetGyro();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

class DriveRotate: public CommandBase {
public:
	DriveRotate(float absoluteAngle);
	DriveRotate(float relativeAngle, bool setRelativeZero);
	
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
	
private:
	typedef enum {rAbsolute, rRelative} RotateMode;
	
	RotateMode m_rotateMode;
	float m_angle;
	float m_setRelativeZero;
};

class DriveTapeDetect: public CommandBase {
public:
	DriveTapeDetect();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
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

class TwoBallHotDecision: public CommandBase {
public:
	TwoBallHotDecision();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	typedef enum {dLeft, dRight} Decision;
	
	Decision m_decision;
};

//////////////////////
// IMAGE PROCESSING //
//////////////////////

class CheckForHot: public CommandBase {
public:
	CheckForHot();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

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



class FrontPickupIncrement: public CommandBase {
public:
	FrontPickupIncrement(FrontPickup::Pot arm, bool up);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
	
private:
	FrontPickup::Pot m_arm;
	bool m_up;
};

class FrontPickupJoystickBoth: public CommandBase {
public:
	FrontPickupJoystickBoth();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

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

class FrontPickupSetMode: public CommandBase {
public:
	FrontPickupSetMode(FrontPickup::FrontMode mode);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	FrontPickup::FrontMode m_pickupMode;
};

class FrontPickupSetRollers: public CommandBase {
public:
	FrontPickupSetRollers(FrontPickup::RollerMode mode);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	FrontPickup::RollerMode m_rollerMode;
};

class FrontPickupWaitForMode: public CommandBase {
public:
	FrontPickupWaitForMode(FrontPickup::FrontMode mode);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
	
private:
	FrontPickup::FrontMode m_mode;
};

////////////
// PICKUP //
////////////

class PickupCatch: public CommandBase {
public:
	PickupCatch();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

////////////////////
// COMMAND GROUPS //
////////////////////

class OneBallAuto: public CommandGroup {
public:	
	OneBallAuto();
};

class TwoBallAuto: public CommandGroup {
public:	
	TwoBallAuto();
};

class TwoBallHotAuto: public CommandGroup {
public:	
	TwoBallHotAuto();
};

#endif
