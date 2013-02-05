#include "DiskShooter.h"

INT32 const c_shootIdlePosition = 0;
INT32 const c_tensionDeadband = 50;
INT32 const c_tensionRange = 600;
INT32 const c_tensionZeroOffset = 0;
INT32 const c_tiltDeadband = 25;
INT32 const c_tiltRange = 600;
INT32 const c_tiltZeroOffset = 0;

DiskShooter::DiskShooter(UINT8   shootMotorModule,		UINT32 shootMotorChannel,
						 UINT8   tiltMotorModule,		UINT32 tiltMotorChannel,
						 UINT8	 tensionMotorModule,	UINT32 tensionMotorChannel,
						 UINT8   shootPotModule,		UINT32 shootPotChannel,
						 UINT8   tiltPotModule,			UINT32 tiltPotChannel,
						 UINT8   tensionPotModule,		UINT32 tensionPotChannel,
						 UINT8   diskSensorModule,		UINT32 diskSensorChannel,
						 Events *eventHandler,			UINT8  eventSourceId)
{
	m_shootMotor = new Victor(shootMotorModule, shootMotorChannel);
	m_tiltMotor = new Victor(tiltMotorModule, tiltMotorChannel);
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

int DiskShooter::Periodic(float joyValue){
	
	// shooterFlags:  Bit 0 = Shooter Tilt completed
	//                    1 = Shooter Tension completed
	//                    2 = Shooter Arm ready for Load
	//                    3 = Shooter Arm ready to Shoot
	//                    4 = Disc Loaded
	
	static float 		tiltSpeed = 0.0;

	INT32				curShootPosition = GetShooterPosition();
	INT32				curTensionPosition = m_tensionPot->GetAverageValue() - c_tensionZeroOffset;
	INT32		 		curTiltPosition = GetTiltPosition();
	float				shootSpeed = 0.0;
	Relay::Value        tensionState;
	int                 shooterFlags = 0;
	
	//----------------------------Tilt Related Stuff-------------------------------------
	
	if(joyValue != 0){
		m_tiltTarget = curTiltPosition;

		if(joyValue > 0 && curTiltPosition > c_tiltRange - c_tiltDeadband) {
			tiltSpeed = 0.0;
			shooterFlags += 1;
		}else if(joyValue < 0 && curTiltPosition < c_tiltDeadband) {
			tiltSpeed = 0.0;
			shooterFlags += 1;
		}else{
			tiltSpeed = joyValue;
		}
		
	}else if(m_tiltTarget == 0 && curTiltPosition < c_tiltDeadband){
		tiltSpeed = 0.0;
		shooterFlags += 1;
	}else{
		tiltSpeed = m_tiltPID->Calculate((float)curTiltPosition);
		if (abs(curTiltPosition - m_tiltTarget) < c_tiltDeadband) shooterFlags += 1;
	}

	m_tiltMotor->Set(tiltSpeed);
	
	//------------------------------Tension Stuff-----------------------------------------

	if(curTensionPosition < m_tensionTarget - c_tensionDeadband) {
		tensionState = Relay::kForward;
	}else if(curTensionPosition > m_tensionTarget + c_tensionDeadband) {
		tensionState = Relay::kReverse;
	}else{
		tensionState = Relay::kOff;
		shooterFlags += 2;
	}
	
	m_tensionMotor->Set(tensionState);
	
	//----------------------------Shoot Arm Stuff-----------------------------------------
	
	switch(m_shootState){
		case sLoad:
			if(curShootPosition >= m_shootReadyPosition){
				shootSpeed = 0.0;
				m_shootState = sShootReady;
				m_shootPID->SetSetpoint((float)curShootPosition);
				shooterFlags += 12;
			}else{
				shootSpeed = 1.0;
				if(curShootPosition >= m_RELEASETHEFRISBEEPOSITION) shooterFlags += 4; 
			}
			break;

		case sShootReady:
			shootSpeed = m_shootPID->Calculate(curShootPosition);
			shooterFlags += 12;
			break;

		case sShoot:
			if(curShootPosition >= c_shootIdlePosition && curShootPosition < m_shootReadyPosition){
				shootSpeed = 0.0;
				m_shootState = sIdle;
			}else{
				shootSpeed = 1.0;
			}
			break;
		
		default:
			shootSpeed = 0.0;
	}
	
	m_shootMotor->Set(shootSpeed);
	
	if (m_diskSensor->Get() == 0) shooterFlags += 16;
	
	return shooterFlags;
}

void DiskShooter::Shoot(){
	
	if(m_shootState == sShootReady){
		m_shootState = sShoot;
	}
}

void DiskShooter::SetTensionTarget(INT32 Target){
	
	if(Target < 0){
		Target = 0;
	}else if (Target > c_tensionRange) {
		Target = c_tensionRange;
	}
	
	m_tensionTarget = Target;
}

void DiskShooter::SetTiltTarget(INT32 Target){
	
	if(Target < 0){
		Target = 0;
	}else if(Target > c_tiltRange){
		Target = c_tiltRange;
	}

	m_tiltTarget = Target;
	m_tiltPID->SetSetpoint((float)m_tiltTarget);
}
