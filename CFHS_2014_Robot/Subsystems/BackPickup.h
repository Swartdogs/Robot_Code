#ifndef BACKPICKUP_H
#define BACKPICKUP_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"

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
	
	int m_lastPosition;
	int m_baseTarget;
	bool m_onTarget;
	
public:
	BackPickup();
	void InitDefaultCommand();
	void Periodic();
	void SetToPosition(int target);
	void SetRollers(float power);
	bool OnTarget();
};

#endif
