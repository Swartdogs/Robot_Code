#include "DiskShooter.h"

INT32 const tiltRange = 500;

DiskShooter::DiskShooter(UINT8   shootMotorModule,		UINT32 shootMotorChannel,
						 UINT8   tiltMotorModule,		UINT32 tiltMotorChannel,
						 UINT8	 tensionMotorModule,	UINT32 tensionMotorChannel,
						 UINT8   shootPotModule,		UINT32 shootPotChannel,
						 UINT8   tiltPotModule,			UINT32 tiltPotChannel,
						 UINT8   tensionPotModule,		UINT32 tensionPotChannel,
						 UINT8   diskSensorModule,		UINT32 diskSensorChannel,
						 INT32   shootIdlePosition,	
						 INT32   tiltZeroOffset,
						 INT32	 tensionZeroOffset,
						 Events *eventHandler,			UINT8  eventSourceId)
{
	m_shootMotor = new Jaguar(shootMotorModule, shootMotorChannel);
	m_tiltMotor = new Jaguar(tiltMotorModule, tiltMotorChannel);
	m_tensionMotor = new Relay(tensionMotorModule, tensionMotorChannel, Relay::kBothDirections);
	
	m_shootPot = new AnalogChannel(shootPotModule, shootPotChannel);
	m_shootPot->SetAverageBits(2);
	m_shootPot->SetOversampleBits(0);
	
	m_tiltPot = new AnalogChannel(tiltPotModule, tiltPotChannel);
	m_tiltPot->SetAverageBits(2);
	m_tiltPot->SetOversampleBits(0);
	
	m_tensionPot = new AnalogChannel(tensionPotModule, tensionPotChannel);
	m_tensionPot->SetAverageBits(2);
	m_tensionPot->SetOversampleBits(0);
	
	m_diskSensor = new DigitalInput(diskSensorModule, diskSensorChannel);
	
	m_shootIdlePosition = shootIdlePosition;	
	m_tiltZeroOffset= tiltZeroOffset;
	
	m_tiltTarget = m_tiltPot->GetAverageValue() - m_tiltZeroOffset;
	m_tensionTarget = m_tensionPot->GetAverageValue() - m_tensionZeroOffset;
	
	if(m_shootPot->GetAverageValue() > m_shootIdlePosition + 20){
		m_shootState = sShootReady;
	}else{
		m_shootState = sIdle;
	}
	
	m_shootReadyPosition = m_shootIdlePosition + 500;
	m_RELEASETHEFRISBEEPOSITION = m_shootIdlePosition + 250;
	
	m_tiltPID = new PIDLoop(0.025, 0, 0.02);
	m_tiltPID->SetInputRange(0, (float)tiltRange);
	m_tiltPID->SetOutputRange(-1.0, 1.0);
	
	m_shootPID = new PIDLoop(0.025, 0, 0.02);
	m_shootPID->SetInputRange(0, 1000);
	m_shootPID->SetOutputRange(-1.0, 1.0);
	
	m_event = eventHandler;	
	m_eventSourceId = eventSourceId;
}

DiskShooter::~DiskShooter(){
	delete m_shootMotor;
	delete m_tiltMotor;
	delete m_shootPot;
	delete m_tiltPot;
	delete m_diskSensor;
	delete m_event;
	delete m_tiltPID;
	delete m_shootPID;
	delete m_tensionPot;
	delete m_tensionMotor;
}

void DiskShooter::Disable(){
	m_shootMotor->Set(0);
	m_shootMotor->SetSafetyEnabled(false);
	
	m_tiltMotor->Set(0);
	m_tiltMotor->SetSafetyEnabled(false);
	
	m_tiltPID->Reset();
}

void DiskShooter::Enable(){
	m_shootMotor->Set(0);
	m_shootMotor->SetSafetyEnabled(true);
	
	m_tiltMotor->Set(0);
	m_tiltMotor->SetSafetyEnabled(true);
}

INT32 DiskShooter::GetShooterPotValue(){
	static INT32 lastReading = 0;
	INT32 		 curReading = 0;
	
	curReading = m_shootPot->GetAverageValue();
	if(curReading < lastReading){
		curReading = lastReading;
	}
	
	lastReading = m_shootPot->GetAverageValue();
	
	return curReading;
}

INT32 DiskShooter::GetTiltPosition(){
	return m_tiltPot->GetAverageValue() - m_tiltZeroOffset;
}

void DiskShooter::Load(){
	if(m_shootState == sIdle){
		m_shootState = sLoad;
	}
}

bool DiskShooter::Periodic(){
	static INT32 		curTiltTarget = m_tiltTarget;
	INT32		 		curTiltPosition = m_tiltPot->GetAverageValue() - m_tiltZeroOffset;
	static float 		tiltSpeed = 0.0;
	float				shootSpeed = 0.0;
	INT32				curShootPosition = GetShooterPotValue();
	static bool			loadingFlag;
	INT32				deadband = 25;
	static INT32		curTensionTarget = m_tensionTarget;
	INT32				curTensionPosition = m_tensionPot->GetAverageValue() - m_tensionZeroOffset;
	static float		tensionSpeed = 0.0;
	
//----------------------------Tilt Related Stuff-------------------------------------
//	if(curTiltTarget != m_tiltTarget){ 
//		curTiltTarget = m_tiltTarget;
//		if(curTiltPosition > curTiltTarget + deadband){
//			tiltSpeed = -1.0;
//		}
//		else if(curTiltPosition < curTiltTarget - deadband){
//			tiltSpeed = 1.0;
//		}
//		else{
//			tiltSpeed = 0.0;
//		}
//	}else if(tiltSpeed > 0.0){
//		if(curTiltPosition >= curTiltTarget) tiltSpeed = 0.0;
//	}else if(tiltSpeed < 0.0){
//		if(curTiltPosition <= curTiltTarget) tiltSpeed = 0.0;
//	}
//	
	if(curTiltTarget == 0){
		tiltSpeed = 0.0;
	}else{
		tiltSpeed = m_tiltPID->Calculate((float)curTiltPosition);
	}
	m_tiltMotor->Set(tiltSpeed);
	
//------------------------------Tension Stuff-----------------------------------------
	if(curTensionTarget != m_tensionTarget){
		curTensionTarget = m_tensionTarget;
		if(curTensionPosition > curTensionTarget + deadband){
			tensionSpeed = -1.0;
		}else if(curTensionPosition < curTensionTarget - deadband){
			tensionSpeed = 1.0;
		}else{
			tensionSpeed = 0.0;
		}
	}else if(tensionSpeed > 0.0){
		if(curTensionPosition >= curTensionTarget) tensionSpeed = 0.0;
	}else if(tensionSpeed < 0.0){
		if(curTensionPosition <= curTensionTarget) tensionSpeed = 0.0;
	}
	
//----------------------------Shoot Arm Stuff-----------------------------------------
	if(m_shootState == sLoad){
		if(curShootPosition >= m_shootReadyPosition){
			shootSpeed = 0.0;
			m_shootState = sShootReady;
			m_shootPID->SetSetpoint((float)curShootPosition);
		}else{
			shootSpeed = 1.0;
			if(curShootPosition >= m_RELEASETHEFRISBEEPOSITION && !loadingFlag){
				loadingFlag = true;
				m_event->RaiseEvent(m_eventSourceId, 1);
			}
		}
	}else if(m_shootState == sShoot){
		if(curShootPosition >= m_shootIdlePosition && curShootPosition < m_shootReadyPosition){
			shootSpeed = 0.0;
			m_shootState = sIdle;
			loadingFlag = false;
		}else{
			shootSpeed = 1.0;
		}
	}else if(m_shootState == sShootReady){
		shootSpeed = m_shootPID->Calculate(curShootPosition);
	}else{
		shootSpeed = 0.0;
	}
	
	m_shootMotor->Set(shootSpeed);
	
	return (m_shootState == sShootReady && tiltSpeed == 0.0 && m_diskSensor->Get());
}

void DiskShooter::Shoot(){
	if(m_shootState == sShootReady){
		m_shootState = sShoot;
	}
}

void DiskShooter::SetTensionTarget(INT32 Target){
	m_tensionTarget = Target;
}

void DiskShooter::SetTiltTarget(INT32 Target){
	m_tiltTarget = Target;
	m_tiltPID->SetSetpoint((float)Target);
}
