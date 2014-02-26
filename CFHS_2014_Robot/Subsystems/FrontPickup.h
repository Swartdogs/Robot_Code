#ifndef FRONTPICKUP_H
#define FRONTPICKUP_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"

/**
 *
 *
 * @author Collin
 */
class FrontPickup: public Subsystem {
public:
	typedef enum {fUnknown, fDeploy, fWaitToStore, fStore, fWaitToLoad, fLoad, fDropInShooter, fShoot, fLowDeploy, fLowShoot, fAutoLoad, fCatch, fStart} FrontMode;
	typedef enum {pLeft, pRight} Pot;
	typedef enum {wIn, wOut, wOff} RollerMode;
	
	FrontPickup(RobotLog* log);
	
	FrontMode	GetFrontPickupMode();
	INT32 		GetPosition(Pot pot);
	bool 		HasBall();
	void 		IncrementArm(Pot arm, bool up);
	void 		InitDefaultCommand();
	bool 		OnTarget();
	void 		Periodic();
	void        SetConstant(const char* key, INT32 value);
	void 		SetJoystickLeft(float joyLeft);
	void 		SetJoystickRight(float joyRight);
	void 		SetPickupMode(FrontMode mode);
	void 		SetRollers(RollerMode mode);
	void 		SetUseJoystickLeft(bool use);
	void 		SetUseJoystickRight(bool use);
	void        StopMotors();
	
private:
	Victor* 		m_rightArm;
	Victor* 		m_leftArm;
	
	Relay* 			m_rightWheels;
	Relay* 			m_leftWheels;
	
	AnalogChannel*	m_leftArmPot;
	AnalogChannel* 	m_rightArmPot;

	PIDControl* 	m_leftArmPID;
	PIDControl* 	m_rightArmPID;
	
	DigitalInput*   m_ballLoadedSensor;
	
	RobotLog*       m_robotLog;

	FrontMode 		m_frontMode;
	float   		m_joyLeft;
	float   		m_joyRight;
	INT32 			m_leftArmTarget;
	bool 			m_leftOnTarget;
	char			m_log[100];
	INT32 			m_rightArmTarget;
	bool 			m_rightOnTarget;
	bool 			m_useJoystickLeft;
	bool 			m_useJoystickRight;
	
	INT32 			f_leftArmZeroOffset;
	INT32			f_leftArmMaxPosition;
	INT32			f_rightArmZeroOffset;
	INT32			f_rightArmMaxPosition;
	INT32			f_armTargetDeadband;
	INT32			f_incrementValue;
	
	INT32 			f_deploySetpoint;
	INT32			f_storeSetpoint;
	INT32			f_loadSetpoint;
	INT32			f_dropInShooterLeftSetpoint;
	INT32			f_dropInShooterRightSetpoint;
	INT32			f_shootSetpoint;
	INT32			f_lowDeploySetpoint;
	INT32			f_catchSetpoint;
	INT32			f_startSetpoint;
	
	char*	GetModeName(FrontMode mode);
	INT32 	LimitValue(Pot pot, INT32 position);
	void 	SetSetpoints(INT32 leftPosition, INT32 rightPosition);
};

#endif
