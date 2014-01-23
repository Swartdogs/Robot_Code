#include "BallShooter.h"
#include "../Robotmap.h"

INT32 const c_shootTriggerPosition = 0;
INT32 const c_shootDeadband = 0;


BallShooter::BallShooter() : Subsystem("BallShooter") {
	
}
    
void BallShooter::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void BallShooter::Load() {
	if(m_shootState == sIdle) {
		m_shootPID->SetPID(0.003, 0, 0);
		m_shootPID->Reset();
		m_shootPID->SetSetpoint((float) c_shootTriggerPosition);
		
		m_shootState = sStart;
	}
}

void BallShooter::Fire() {
	
	INT32 curShootPosition = GetShooterPosition();
	float shootSpeed = 0.0;
	
	if(m_shootState == sReady) {
		m_shootState = sFire;
		
		if(abs(curShootPosition - c_shootTriggerPosition) < 50) {
			shootSpeed = 1.0;
		} else {
			shootSpeed = 0.0;
			m_shootState = sIdle;
		}
	}
	
	m_shootMotor->Set(shootSpeed);
}

INT32 BallShooter::GetShooterPosition() {
	INT32 curReading = 0;
	static INT32 lastReading = 0;
	INT32 returnValue = 0;
	
	curReading = m_shootPot->GetAverageValue();
	
	if(curReading > lastReading) {
		returnValue = curReading;
	} else if(abs(curReading - lastReading) < 200) {
		returnValue = curReading;
	} else {
		returnValue = lastReading;
	}
	
	lastReading = curReading;
	
	return returnValue;
}


