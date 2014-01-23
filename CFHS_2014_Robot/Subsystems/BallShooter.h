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
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	typedef enum {sIdle, sStart, sLoad, sReady, sFire} ShootState;
	
	Victor*        m_shootMotor;
	AnalogChannel* m_shootPot;
	PIDControl*    m_shootPID;
	INT32          m_shootReadyPosition;
	ShootState     m_shootState;
	INT32          m_releaseBallPosition;
	
public:
	BallShooter();
	void InitDefaultCommand();
	
	INT32 GetShooterPosition();
	void  Load();
	void  Fire();
	
};

#endif
