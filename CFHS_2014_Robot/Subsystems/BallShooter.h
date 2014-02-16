#ifndef BALLSHOOTER_H
#define BALLSHOOTER_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"

/**
 *
 *
 * @author Srinu
 */
class BallShooter: public Subsystem {

public:
	typedef enum {sIdle, sStart, sLoad, sReady, sFire, sRelease} ShootState;
	
	BallShooter(RobotLog* log);

	void  		Fire();
	INT32 		GetShooterPosition();
	ShootState	GetShootState();
	bool  		HasBall();
	void  		InitDefaultCommand();
	void  		Load();
	void  		Periodic();
	void		Release();
	void		StopMotors();
	
	void 		UpdateConstants();
	
private:
	Victor*        m_shootMotor;
	AnalogChannel* m_shootPot;
	
	PIDControl*    m_shootPID;
	ShootState     m_shootState;
	
	DigitalInput*  m_ballSensor;
	
	RobotLog*	   m_robotLog;
	char		   m_log[100];
	
	char*	GetStateName(ShootState state);
	
	INT32 f_triggerPosition;
	INT32 f_releasePosition;
};

#endif
