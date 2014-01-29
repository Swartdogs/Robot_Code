#ifndef BACKPICKUP_H
#define BACKPICKUP_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"

/**
 *
 *
 * @author Neil
 */
class BackPickup: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	
	Victor* m_baseMotor;
	Victor* m_rollers;
	
	AnalogChannel* m_baseMotorPot;
	
	PIDControl* m_baseMotorPID;
	
	RobotLog* m_log;
	
	bool m_useJoystick;
	
	float m_joySpeed;
	
	INT32 m_baseTarget;
	bool  m_onTarget;
	
	INT32 GetPosition();
	
public:
	BackPickup(RobotLog* log);
	void InitDefaultCommand();
	void Periodic();
	void SetSetpoint(INT32 target);
	void SetRollers(float power);
	void SetUseJoystick(bool use);
	void SetJoystickSpeed(float speed);
	bool OnTarget();
};

#endif
