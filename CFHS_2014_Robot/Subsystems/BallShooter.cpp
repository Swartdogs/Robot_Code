#include "BallShooter.h"
#include "../Robotmap.h"
#include "../CommandBase.h"

INT32 const c_triggerPosition = 930; 
INT32 const c_releasePosition = 100;

BallShooter::BallShooter(RobotLog* log) : Subsystem("BallShooter") {
	m_robotLog = log;
	
	m_shootMotor = new Victor(PWM_SHOOT_MOTOR);
	
	m_shootPot = new AnalogChannel(AI_SHOOT_POT);
	m_shootPot->SetAverageBits(2);
	m_shootPot->SetOversampleBits(0);
	
	m_shootState = sIdle;
	m_shootPID = new PIDControl(0.015, 0.0, 0);
	m_shootPID->SetInputRange(0, 1000);       
	m_shootPID->SetOutputRange(-1.0, 1.0);
	
	m_ballSensor = new DigitalInput(DI_BALL_SHOOTER_SENSOR);
	
	// INIParser Stuff
	UpdateConstants();
}
    
void BallShooter::Fire() {
	if(HasBall()) {
		m_shootState = sFire;
		sprintf(m_log, "Shooter: State=%s", GetStateName(sFire));
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
		m_shootPID->SetPID(0.015, 0, 0); // 0.015, 0, 0
		m_shootPID->Reset();
		m_shootPID->SetSetpoint((float) c_triggerPosition);
		m_shootState = sLoad;
		shootSpeed = 1.0;
		break;
	
	case sLoad:
		if(curPosition > c_triggerPosition || curPosition < 100) {
			shootSpeed = 1.0;
		} else {
			shootSpeed = m_shootPID->Calculate(curPosition);
			
			if(curPosition > (c_triggerPosition - 100)) {
				m_shootPID->SetPID(0.015, 0.001, 0); 
				m_shootState = sReady;
			}
		}
		break;
	
	case sReady:
		shootSpeed = m_shootPID->Calculate(curPosition);
		break;
	
	case sFire:
		if(CommandBase::backPickup->GetBackPickupMode() != BackPickup::bShoot) {
			CommandBase::backPickup->SetPickupMode(BackPickup::bShoot);
		
		} else if(CommandBase::frontPickup->GetFrontPickupMode() != FrontPickup::fShoot) {
			CommandBase::frontPickup->SetPickupMode(FrontPickup::fShoot);
		
		} else if(CommandBase::backPickup->OnTarget() && CommandBase::frontPickup->OnTarget()) {
			if(abs(curPosition - c_triggerPosition) < 100) {
				shootSpeed = 1.0;
			} else {
				m_shootState = sStart;
				m_robotLog->LogWrite("Shooter: Move to Trigger");
			}
		}
		break;
		
	case sRelease:
		shootSpeed = m_shootPID->Calculate(curPosition);
		if (curPosition < c_releasePosition + 10) m_shootState = sIdle;
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
		m_shootPID->SetPID(0.015, 0, 0);
		m_shootPID->Reset();
		m_shootPID->SetSetpoint((float) c_releasePosition);
		m_shootState = sRelease;
	}
}

void BallShooter::StopMotors() {
	m_shootMotor->Set(0.0);
}

void BallShooter::UpdateConstants() {
	CommandBase::iniParser->SetSubsystem("BALLSHOOTER");
	f_triggerPosition = CommandBase::iniParser->FindValue("triggerPosition", c_triggerPosition);
	f_releasePosition = CommandBase::iniParser->FindValue("releasePosition", c_releasePosition);
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

