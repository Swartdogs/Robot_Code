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
	typedef enum {fDeployBoth, fDeployLeft, fDeployRight, fPass, fStore, fLowShoot, fLowDeploy, fMoveToLoad, fLoad} FrontMode;
	typedef enum {pLeft, pRight} Pot;
	
	FrontPickup(RobotLog* log);
	void InitDefaultCommand();
	
	void Periodic();
	void IncrementArm(Pot arm, bool up);
	void SetSetpoints(INT32 leftPosition, INT32 rightPosition);
	void SetSetpoint(INT32 position, Pot arm);
	void SetUseJoystickLeft(bool use);
	void SetUseJoystickRight(bool use);
	void SetJoystickLeft(float joyLeft);
	void SetJoystickRight(float joyRight);
	
	void SetPickupMode(FrontMode mode);
	FrontMode GetFrontPickupMode();
	
	bool HasBall();
	
private:
	
	Victor* 		m_rightArm;
	Victor* 		m_leftArm;
	Victor* 		m_rightWheels;
	Victor* 		m_leftWheels;
	
	AnalogChannel*	m_leftArmPot;
	AnalogChannel* 	m_rightArmPot;

	PIDControl* 	m_leftArmPID;
	PIDControl* 	m_rightArmPID;
	
	DigitalInput*   m_ballLoadedSensor;
	
	RobotLog*       m_log;

	INT32 	m_leftArmTarget;
	INT32 	m_rightArmTarget;
	
	FrontMode m_frontMode;
	
	float   m_joyLeft;
	float   m_joyRight;
	
	bool 	m_useJoystickLeft;
	bool 	m_useJoystickRight;
	bool 	m_leftOnTarget;
	bool 	m_rightOnTarget;
	
	INT32 GetPosition(Pot pot);
	INT32 LimitValue(Pot pot, INT32 position);
};

#endif
