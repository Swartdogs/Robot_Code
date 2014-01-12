#ifndef DIAL_H
#define DIAL_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"
#include "../RobotMap.h"

class Dial: public Subsystem {
public:
	typedef enum {mPresets, mGyro}DialMode;

	Dial(RobotMap* robotMap);
	~Dial();
	
	void InitDefaultCommand();

	void  	DriveMotor(float value);
	float 	GetAngle();
	char*	GetDialModeName();
	bool	OnTarget();
	void    Periodic();
	void	SetTarget(float target);
	void	SwitchDialMode();
	
private:
	Victor*			m_dialMotor;
	PIDControl*		m_dialPID;
	AnalogChannel*	m_dialPot;
	RobotLog*		m_robotLog;
	
	DialMode		m_dialMode;
	char      	  	m_log[100];
	bool			m_onTarget;
	float			m_targetAngle;
	
	float GetError(float targetAngle);
};

#endif
