#ifndef BACKPICKUP_H
#define BACKPICKUP_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"

class BackPickup: public Subsystem {
public:
	typedef enum{bUnknown, bStore, bDeploy, bWaitToDeploy, bPass, bShoot, bCatch, bStart} BackMode;
	typedef enum{rIn, rManualIn, rOut, rOff} RollerMode;
	typedef enum{aUp, aDown} AdjustMode;
	
	BackPickup(RobotLog* log);
	BackMode	GetBackPickupMode();
	INT32 		GetPosition();
	bool    	HasBall();
	void		IncrementArm(AdjustMode mode);
	void 		InitDefaultCommand();
	bool 		OnTarget();
	void 		Periodic();
	void        SetConstant(const char* key, INT32 value);
	void 		SetJoystickSpeed(float speed);
	void 		SetPickupMode(BackMode mode);
	void 		SetRollerMode(RollerMode mode);
	void 		SetUseJoystick(bool use);
	void        StopMotors();
	
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
	RollerMode  m_rollerMode;
	INT32 		m_baseTarget;
	float 		m_joySpeed;
	char		m_log[100];
	bool  		m_onTarget;
	bool		m_useJoystick;
	
	INT32 f_baseMotorDeadband;
	INT32 f_baseZeroOffset;	
	INT32 f_baseMaxPosition;
	INT32 f_incrementValue;
	
	INT32 f_storeSetpoint;
	INT32 f_deploySetpoint;
	INT32 f_passSetpoint;
	INT32 f_shootSetpoint;
	INT32 f_catchSetpoint;
	INT32 f_startSetpoint;
};

#endif
