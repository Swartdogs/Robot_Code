#include "BallShooter.h"
#include "../Robotmap.h"
#include "../CommandBase.h"

BallShooter::BallShooter(RobotLog* log) : Subsystem("BallShooter") {
	f_triggerPosition = 900;
	f_releasePosition = 160;

	m_robotLog = log;
	
	m_shootMotor = new Victor(PWM_SHOOT_MOTOR);
	
	m_shootPot = new AnalogChannel(AI_SHOOT_POT);
	m_shootPot->SetAverageBits(2);
	m_shootPot->SetOversampleBits(0);
	
	m_shootState = sIdle;
	m_maxReadyPosition = 0;
	
	m_shootPID = new PIDControl(0.015, 0.0, 0);
	m_shootPID->SetInputRange(0, 1000);       
	m_shootPID->SetOutputRange(-1.0, 1.0);
	
	m_ballSensor = new DigitalInput(DI_BALL_SHOOTER_SENSOR);
}
    
void BallShooter::Fire() {
	if(HasBall()) {
		m_shootState = sFire;
		sprintf(m_log, "Shooter: State=%s  MaxReady=%d", GetStateName(sFire), m_maxReadyPosition);
		m_robotLog->LogWrite(m_log);
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

bool BallShooter::HasBall() {
	return (!m_ballSensor->Get() && m_shootState == sReady);
}

void BallShooter::InitDefaultCommand() {
}

void BallShooter::Load() {
	if(m_shootState == sIdle) {
		m_shootState = sStart;
		sprintf(m_log, "Shooter: State=%s", GetStateName(sStart));
		m_robotLog->LogWrite(m_log);
	}
}

void BallShooter::Periodic() {
	ShootState startState = m_shootState;
	float shootSpeed = 0;
	INT32 curPosition = GetShooterPosition();

	switch(m_shootState) {
	case sStart:
		m_shootPID->SetPID(0.010, 0, 0); // 0.015, 0, 0
		m_shootPID->Reset();
		m_shootPID->SetSetpoint((float) f_triggerPosition);
		m_shootState = sLoad;
		shootSpeed = 1.0;
		break;
	
	case sLoad:
		if(curPosition > f_triggerPosition || curPosition < 100) {
			shootSpeed = 1.0;
		} else {
			shootSpeed = m_shootPID->Calculate(curPosition);
			
			if(curPosition > (f_triggerPosition - 100)) {
				m_maxReadyPosition = curPosition;
				m_shootPID->SetPID(0.010, 0.0006, 0);    //  0.015  0.001  0
				m_shootState = sReady;
			}
		}
		break;
	
	case sReady:
		shootSpeed = m_shootPID->Calculate(curPosition);
		if (m_maxReadyPosition < curPosition) m_maxReadyPosition = curPosition;
		break;
	
	case sFire:
		shootSpeed = m_shootPID->Calculate(curPosition);
		
		if(CommandBase::backPickup->GetBackPickupMode() != BackPickup::bShoot) {
			CommandBase::backPickup->SetPickupMode(BackPickup::bShoot);
		
		} else if(CommandBase::frontPickup->GetFrontPickupMode() != FrontPickup::fShoot) {
			CommandBase::frontPickup->SetPickupMode(FrontPickup::fShoot);
		
		} else if(CommandBase::backPickup->OnTarget() && CommandBase::frontPickup->OnTarget()) {
			if(abs(curPosition - f_triggerPosition) < 100) {
				shootSpeed = 1.0;
			} else {
				m_shootState = sStart;
				m_robotLog->LogWrite("Shooter: Move to Trigger");
			}
		}
		break;
		
	case sRelease:
		shootSpeed = m_shootPID->Calculate(curPosition);
		if (curPosition < f_releasePosition + 10 || curPosition > f_triggerPosition + 10) m_shootState = sIdle;
		break;
		
	default:;
	}
	
	if (m_shootState != startState) {
		sprintf(m_log, "Shooter: State=%s", GetStateName(m_shootState));
		m_robotLog->LogWrite(m_log);
	}

	m_shootMotor->Set(shootSpeed);
}

void BallShooter::Release() {
	if (m_shootState == sReady) {
		m_shootPID->SetPID(0.010, 0, 0);
		m_shootPID->Reset();
		m_shootPID->SetSetpoint((float) f_releasePosition);
		m_shootState = sRelease;
		sprintf(m_log, "Shooter: State=%s", GetStateName(sRelease));
		m_robotLog->LogWrite(m_log);
	}
}

void BallShooter::Reset() {
	if (m_shootState == sReady && !HasBall()) {
		f_triggerPosition -= 5;
		m_shootState = sStart;
		sprintf(m_log, "Shooter: Reset to %d  MaxReady=%d", f_triggerPosition, m_maxReadyPosition);
		m_robotLog->LogWrite(m_log);
	}
}

void BallShooter::SetConstant(const char* key, INT32 value) {
	if(strcmp(key,"triggerPosition") == 0) {
		f_triggerPosition = value;
		sprintf(m_log, "BallShooter: Set TriggerPosition=%d", value);
		m_robotLog->LogWrite(m_log);
	
	} else if(strcmp(key,"releasePosition") == 0) {
		f_releasePosition = value;
		sprintf(m_log, "BallShooter: Set ReleasePosition=%d", value);
		m_robotLog->LogWrite(m_log);
	}
}

void BallShooter::StopMotors() {
	m_shootMotor->Set(0.0);
}


//  ******************** PRIVATE ********************


char* BallShooter::GetStateName(ShootState state) {
	switch(state) {
	case sIdle:		return "Idle";
	case sStart:	return "Start";
	case sLoad:		return "Load";
	case sReady:	return "Ready";
	case sFire:		return "Fire";
	case sRelease:	return "Release";
	default:		return "?";
	}
}

