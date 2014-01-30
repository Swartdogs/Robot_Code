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
public:
	typedef enum{bDeploy, bStore, bPass}BackMode;
	
	BackPickup(RobotLog* log);
	void InitDefaultCommand();
	void Periodic();
	void SetUseJoystick(bool use);
	void SetJoystickSpeed(float speed);
	bool OnTarget();
	void SetPickupMode(BackMode mode);
	
	BackMode  GetBackPickupMode();
	
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	
	void SetSetpoint(INT32 target);
	void SetRollers(float power);
	
	Victor* m_baseMotor;
	Victor* m_rollers;
	
	DigitalInput* m_lightSensor;
	
	AnalogChannel* m_baseMotorPot;
	
	PIDControl* m_baseMotorPID;
	
	RobotLog* m_log;
	
	bool m_useJoystick;
	
	float m_joySpeed;
	
	INT32 m_baseTarget;
	bool  m_onTarget;
	
	BackMode m_backMode;
	
	INT32 GetPosition();
};

#endif
