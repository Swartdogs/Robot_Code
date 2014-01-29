#include "BallShooter.h"
#include "../Robotmap.h"

INT32 const c_shootTriggerPosition = 0;
INT32 const c_shootDeadband = 0;


BallShooter::BallShooter() : Subsystem("BallShooter") {
	m_shootMotor = new Victor(MOD_SHOOT_MOTOR, PWM_SHOOT_MOTOR);
	m_shootPot = new AnalogChannel(AI_SHOOT_POT);
	m_shootPot->SetAverageBits(2);
	m_shootPot->SetOversampleBits(0);
	
	m_shootState = sIdle;
	m_shootPID = new PIDControl(0.003, 0, 0);
	m_shootPID->SetInputRange(0, 1000);       // NEED TO CHANGE
	m_shootPID->SetOutputRange(-0.7, 0.7);
}
    
void BallShooter::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void BallShooter::Periodic() {
	float shootSpeed = 0;
	INT32 curShootPosition = GetShooterPosition();
	switch(m_shootState) {
		case sIdle:
			break;
		case sStart:
			m_shootPID->SetPID(0.003, 0, 0);
			m_shootPID->Reset();
			m_shootPID->SetSetpoint((float) c_shootTriggerPosition);
			
			m_shootState = sLoad;
			break;
		case sLoad:
			if(curShootPosition > (c_shootTriggerPosition - 100)) {
				m_shootPID->SetPID(0.003, 0.0004, 0);
				m_shootState = sReady;
			}
		case sReady:
			shootSpeed = m_shootPID->Calculate(GetShooterPosition());
			m_shootMotor->Set(shootSpeed);
			break;
		case sFire:
			if(abs(curShootPosition - c_shootTriggerPosition) < 100) {
				shootSpeed = 1.0;
			} else {
				shootSpeed = 0.0;
				m_shootState = sIdle;
			}
			break;
	}
	m_shootMotor->Set(shootSpeed);
}

void BallShooter::Load() {
	if(m_shootState == sIdle) {
		m_shootState = sStart;
	}
}

void BallShooter::Fire() {
	if(m_shootState == sReady) {
		m_shootState = sFire;
	}
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

BallShooter::ShootState BallShooter::GetShootState() {
	return m_shootState;
}


