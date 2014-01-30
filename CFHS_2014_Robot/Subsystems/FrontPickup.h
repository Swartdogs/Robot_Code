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
	typedef enum {fDeployBoth,fDeployLeft,fDeployRight,fPass,fStore,fLowShoot,fLowDeploy} FrontMode;
	
	FrontPickup(RobotLog* log);
	void InitDefaultCommand();
	
	void Periodic();
	void SetSetpoints(INT32 leftPosition, INT32 rightPosition);
	void SetUseJoystickLeft(bool use);
	void SetUseJoystickRight(bool use);
	void SetJoystickLeft(float joyLeft);
	void SetJoystickRight(float joyRight);
	
	void SetPickupMode(FrontMode mode);
	FrontMode GetFrontPickupMode();
	
private:
	
	typedef enum {pLeft, pRight}Pot;
	
	Victor* 		m_rightArm;
	Victor* 		m_leftArm;
	Victor* 		m_rightWheels;
	Victor* 		m_leftWheels;
	
	AnalogChannel*	m_leftArmPot;
	AnalogChannel* 	m_rightArmPot;

	PIDControl* 	m_leftArmPID;
	PIDControl* 	m_rightArmPID;
	
	DigitalInput*   m_rightArmSensor;
	
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
};

#endif
