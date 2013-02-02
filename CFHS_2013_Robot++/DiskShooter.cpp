#include "DiskShooter.h"

INT32 const c_shootIdlePosition = 0;
INT32 const c_tensionZeroOffset = 0;
INT32 const c_tensionDeadband = 50;
INT32 const c_tiltZeroOffset = 0;
INT32 const c_tiltRange = 500;

DiskShooter::DiskShooter(UINT8   shootMotorModule,		UINT32 shootMotorChannel,
						 UINT8   tiltMotorModule,		UINT32 tiltMotorChannel,
						 UINT8	 tensionMotorModule,	UINT32 tensionMotorChannel,
						 UINT8   shootPotModule,		UINT32 shootPotChannel,
						 UINT8   tiltPotModule,			UINT32 tiltPotChannel,
						 UINT8   tensionPotModule,		UINT32 tensionPotChannel,
						 UINT8   diskSensorModule,		UINT32 diskSensorChannel,
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
	
	m_tiltTarget = m_tiltPot->GetAverageValue() - c_tiltZeroOffset;
	m_tensionTarget = m_tensionPot->GetAverageValue() - c_tensionZeroOffset;
	
	if(m_shootPot->GetAverageValue() > c_shootIdlePosition + 200){
		m_shootState = sShootReady;
	}else{
		m_shootState = sIdle;
	}
	
	m_shootReadyPosition = c_shootIdlePosition + 500;
	m_RELEASETHEFRISBEEPOSITION = c_shootIdlePosition + 250;
	
	m_tiltPID = new PIDLoop(0.025, 0, 0.02);
	m_tiltPID->SetInputRange(0, (float)c_tiltRange);
	m_tiltPID->SetOutputRange(-1.0, 1.0);
	
	m_shootPID = new PIDLoop(0.025, 0, 0.02);
	m_shootPID->SetInputRange(0, 900);
	m_shootPID->SetOutputRange(0, 1.0);
	
	m_event = eventHandler;	
	m_eventSourceId = eventSourceId;
}

DiskShooter::~DiskShooter(){
	delete m_diskSensor;
	delete m_event;
	delete m_shootMotor;
	delete m_shootPID;
	delete m_shootPot;
	delete m_tensionMotor;
	delete m_tensionPot;
	delete m_tiltMotor;
	delete m_tiltPID;
	delete m_tiltPot;
}

void DiskShooter::Disable(){
	m_shootMotor->Set(0);
	m_shootMotor->SetSafetyEnabled(false);
	
	m_tiltMotor->Set(0);
	m_tiltMotor->SetSafetyEnabled(false);
	
	m_shootPID->Reset();
	m_tiltPID->Reset();
}

void DiskShooter::Enable(){
	m_shootMotor->Set(0);
	m_shootMotor->SetSafetyEnabled(true);
	
	m_tiltMotor->Set(0);
	m_tiltMotor->SetSafetyEnabled(true);
}

INT32 DiskShooter::GetShooterPosition(){
	INT32 		 curReading = 0;
	static INT32 lastReading = 0;
	INT32        vReturn = 0;
	
	curReading = m_shootPot->GetAverageValue();
	
	if(curReading < lastReading){
		vReturn = lastReading;
	}else{
		vReturn = curReading;
	}
	
	lastReading = curReading;
	return vReturn;
}

INT32 DiskShooter::GetTiltPosition(){
	return m_tiltPot->GetAverageValue() - c_tiltZeroOffset;
}

void DiskShooter::Load(){
	if(m_shootState == sIdle){
		m_shootState = sLoad;
	}
}

bool DiskShooter::Periodic(){
	static INT32 		curTiltTarget = m_tiltTarget;
	INT32		 		curTiltPosition = GetTiltPosition();
	static float 		tiltSpeed = 0.0;
	float				shootSpeed = 0.0;
	INT32				curShootPosition = GetShooterPosition();
	static bool			loadingFlag = false;
	INT32				curTensionPosition = m_tensionPot->GetAverageValue() - c_tensionZeroOffset;
	Relay::Value        tensionState;
	
//----------------------------Tilt Related Stuff-------------------------------------
	
	if(curTiltTarget == 0 && curTiltPosition < 20){
		tiltSpeed = 0.0;
	}else{
		tiltSpeed = m_tiltPID->Calculate((float)curTiltPosition);
	}

	m_tiltMotor->Set(tiltSpeed);
	
//------------------------------Tension Stuff-----------------------------------------

	if(curTensionPosition < m_tensionTarget - c_tensionDeadband) {
		tensionState = Relay::kForward;
	}else if(curTensionPosition > m_tensionTarget + c_tensionDeadband) {
		tensionState = Relay::kReverse;
	}else{
		tensionState = Relay::kOff;
	}
	
	m_tensionMotor->Set(tensionState);
	
//----------------------------Shoot Arm Stuff-----------------------------------------
	
	switch(m_shootState){
		case sLoad:
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
			break;

		case sShootReady:
			shootSpeed = m_shootPID->Calculate(curShootPosition);
			break;

		case sShoot:
			if(curShootPosition >= c_shootIdlePosition && curShootPosition < m_shootReadyPosition){
				shootSpeed = 0.0;
				m_shootState = sIdle;
				loadingFlag = false;
			}else{
				shootSpeed = 1.0;
			}
			break;
		
		default:
			shootSpeed = 0.0;
	}
	
	m_shootMotor->Set(shootSpeed);
	
	return (m_shootState == sShootReady && tiltSpeed == 0.0 && m_diskSensor->Get() == 0);
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
