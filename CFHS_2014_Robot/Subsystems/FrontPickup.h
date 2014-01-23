#ifndef FRONTPICKUP_H
#define FRONTPICKUP_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"

/**
 *
 *
 * @author Collin
 */
class FrontPickup: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	Victor* m_rightArm;
	Victor* m_leftArm;
	Relay* m_rightWheels;
	Relay* m_leftWheels;
	
	AnalogChannel* m_leftArmPot;
	AnalogChannel* m_rightArmPot;

	PIDControl* m_leftArmPID;
	PIDControl* m_rightArmPID;
	
	int m_leftArmTarget;
	int m_rightArmTarget;
	int m_leftLastPosition;
	int m_rightLastPosition;
	
	bool m_leftOnTarget;
	bool m_rightOnTarget;
	
public:
	FrontPickup();
	void InitDefaultCommand();
	
	void Run();
	void MoveArmsWithJoystick(float leftArmPower, float rightArmPower);
	void SetArmsToPosition(int leftPosition, int rightPosition);
	void RunRightWheels(Relay::Value value);
	void RunLeftWheels(Relay::Value value);
};

#endif
