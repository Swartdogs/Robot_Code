#ifndef BACKPICKUP_H
#define BACKPICKUP_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"

class BackPickup: public Subsystem {
public:
	typedef enum{bUnknown, bStore, bDeploy, bWaitToDeploy, bPass, bShoot} BackMode;
	typedef enum{rIn, rOut, rOff} RollerMode;
	typedef enum{aUp, aDown} AdjustMode;
	
	BackPickup(RobotLog* log);
	BackMode	GetBackPickupMode();
	INT32 		GetPosition();
	void 		InitDefaultCommand();
	bool 		OnTarget();
	void 		Periodic();
	void 		SetJoystickSpeed(float speed);
	void 		SetPickupMode(BackMode mode);
	void 		SetRollers(RollerMode mode);
	void 		SetUseJoystick(bool use);
	void		IncrementArm(AdjustMode mode);
	
	bool    HasBall();
	
private:
	char*	GetModeName(BackMode mode);
	void 	SetSetpoint(INT32 target);
	bool 	ShooterHasBall();
	INT32   LimitValue(INT32 position);
	
	Victor* 		m_baseMotor;
	Victor* 		m_rollers;
	
	DigitalInput*   m_ballSensor;
	AnalogChannel* 	m_baseMotorPot;
	PIDControl*    	m_baseMotorPID;
	RobotLog*      	m_robotLog;
	
	BackMode 	m_backMode;
	INT32 		m_baseTarget;
	float 		m_joySpeed;
	char		m_log[100];
	bool  		m_onTarget;
	bool		m_useJoystick;
};

#endif
