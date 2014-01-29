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
private:
	
	typedef enum {pLeft, pRight}Pot;
	
	Victor* 		m_rightArm;
	Victor* 		m_leftArm;
	Relay* 			m_rightWheels;
	Relay* 			m_leftWheels;
	
	AnalogChannel*	m_leftArmPot;
	AnalogChannel* 	m_rightArmPot;

	PIDControl* 	m_leftArmPID;
	PIDControl* 	m_rightArmPID;
	
	RobotLog*       m_log;

	INT32 	m_leftArmTarget;
	INT32 	m_rightArmTarget;
	
	float   m_joyLeft;
	float   m_joyRight;
	
	bool 	m_useJoystickLeft;
	bool 	m_useJoystickRight;
	bool 	m_leftOnTarget;
	bool 	m_rightOnTarget;
	
	INT32 GetPosition(Pot pot);
	
public:
	FrontPickup(RobotLog* log);
	void InitDefaultCommand();
	
	void Periodic();
	void SetSetpoint(INT32 leftPosition, INT32 rightPosition);
	void RunRightWheels(Relay::Value value);
	void RunLeftWheels(Relay::Value value);
	void SetUseJoystickLeft(bool use);
	void SetUseJoystickRight(bool use);
	void SetJoystickLeft(float joyLeft);
	void SetJoystickRight(float joyRight);
};

#endif
