#ifndef BALLSHOOTER_H
#define BALLSHOOTER_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"

/**
 *
 *
 * @author Srinu
 */
class BallShooter: public Subsystem {

public:
	typedef enum {sIdle, sStart, sLoad, sReady, sFire} ShootState;
	
	BallShooter();
	void InitDefaultCommand();
	
	void  Periodic();
	
	INT32 GetShooterPosition();
	void  Load();
	void  Fire();
	
	ShootState GetShootState();
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	
	
	Victor*        m_shootMotor;
	AnalogChannel* m_shootPot;
	
	PIDControl*    m_shootPID;
	ShootState     m_shootState;
		
};

#endif
