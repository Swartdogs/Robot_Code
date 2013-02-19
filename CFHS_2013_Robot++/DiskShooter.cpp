#include "DiskShooter.h"

INT32 const c_shootTriggerPosition = 790;
INT32 const c_shootDeadband = 20;
INT32 const c_tensionDeadband = 5;
INT32 const c_tensionRange = 150;			// 300
INT32 const c_tensionZeroOffset = 250;
INT32 const c_tiltDeadband = 5;
INT32 const c_tiltRange = 330;
INT32 const c_tiltZeroOffset = 430;

DiskShooter::DiskShooter(UINT8   shootMotorModule,		UINT32 shootMotorChannel,
						 UINT8   tiltMotorModule,		UINT32 tiltMotorChannel,
						 UINT8	 tensionMotorModule,	UINT32 tensionMotorChannel,
						 UINT8   shootPotModule,		UINT32 shootPotChannel,
						 UINT8   tiltPotModule,			UINT32 tiltPotChannel,
						 UINT8   tensionPotModule,		UINT32 tensionPotChannel,
						 Events *eventHandler,			UINT8  eventSourceId)
{
	m_shootMotor = new Victor(shootMotorModule, shootMotorChannel);
	m_shootMotor->SetExpiration(1.0);
	
	m_tiltMotor = new Victor(tiltMotorModule, tiltMotorChannel);
	m_tiltMotor->SetExpiration(1.0);
	
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
	
	m_tiltPID = new PIDLoop(0.02, 0.002, 0.05);
	m_tiltPID->SetInputRange(0, (float)c_tiltRange);
	m_tiltPID->SetOutputRange(-1.0, 1.0);
	
	m_shootPID = new PIDLoop(0.0030, 0, 0);
	m_shootPID->SetInputRange(0, 900);
	m_shootPID->SetOutputRange(-0.5, 1.0);
	
	m_event = eventHandler;	
	m_eventSourceId = eventSourceId;

	m_RELEASETHEFRISBEEPOSITION = c_shootTriggerPosition - 300;

	m_tiltTarget = m_tiltPot->GetAverageValue() - c_tiltZeroOffset;
	m_tensionTarget = GetTension();
	m_shootState = sIdle;
}

DiskShooter::~DiskShooter(){
	
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
	
	m_shootState = sIdle;
	
	m_tensionTarget = GetTension();
	m_tiltTarget = m_tiltPot->GetAverageValue() - c_tiltZeroOffset;
	m_newTiltTarget = true;
	
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
	
	if (curReading > lastReading) {
		vReturn = curReading;
	} else if (abs(curReading - lastReading) < 200) {
		vReturn = curReading;
	} else {
		vReturn = lastReading;
	}
	
	lastReading = curReading;
	return vReturn;
}

INT32 DiskShooter::GetTension() {
	return  m_tensionPot->GetAverageValue() - c_tensionZeroOffset;
}

INT32 DiskShooter::GetTiltPosition(){
	
	return m_tiltPot->GetAverageValue() - c_tiltZeroOffset;
}


INT32 DiskShooter::GetTensionTarget() {
	return m_tensionTarget;
}


INT32 DiskShooter::GetTiltTarget(){
	return m_tiltTarget;
}

void DiskShooter::IncrementShooter(bool Up){
	if (Up == true){
		m_tiltTarget += 5;
	} else {
		m_tiltTarget -= 5;
	}
	m_newTiltTarget = true;
}

void DiskShooter::Load(){
	
	if(m_shootState == sIdle){
		m_shootPID->SetPID(0.003, 0, 0);
		m_shootPID->Reset();
		m_shootPID->SetSetpoint((float) c_shootTriggerPosition);
		
		m_shootState = sStart;
	}
}

int DiskShooter::Periodic(float joyValue){
	
	// shooterFlags:  Bit 1 = Shooter Tilt completed
	//                    2 = Shooter Tension completed
	//                    4 = Shooter Arm ready for Load
	//                    8 = Shooter Arm ready to Shoot
	//                    16 = Disc Loaded
	//                    32 = Shoot State is sIdle
	
	static float 		tiltSpeed = 0.0;

	INT32				curShootPosition = GetShooterPosition();
	INT32				curTensionPosition = GetTension();
	INT32		 		curTiltPosition = GetTiltPosition();
	float				shootSpeed = 0.0;
	Relay::Value        tensionState;
	int                 shooterFlags = 0;
	
	//----------------------------Tilt Related Stuff-------------------------------------
	
	if(joyValue != 0){
		m_tiltTarget = curTiltPosition;
		m_newTiltTarget = true;
		
		if(joyValue > 0 && curTiltPosition > c_tiltRange - c_tiltDeadband) {
			tiltSpeed = 0.0;
			shooterFlags += 1;
		}else if(joyValue < 0 && curTiltPosition < c_tiltDeadband) {
			tiltSpeed = 0.0;
			shooterFlags += 1;
		}else{
			tiltSpeed = joyValue;
			printf("Shooter Tilt=%d \n", curTiltPosition);
		}
		
	}else if(m_tiltTarget == 0 && curTiltPosition < c_tiltDeadband){
		tiltSpeed = 0.0;
		shooterFlags += 1;
		
	}else{
		if (m_newTiltTarget) {
			if(m_tiltTarget > c_tiltRange - 60) m_tiltTarget = c_tiltRange;
			m_newTiltTarget = false;
			m_tiltPID->SetSetpoint((float)m_tiltTarget);
			m_tiltPID->Reset();
		}
		
		tiltSpeed = m_tiltPID->Calculate((float)curTiltPosition);
		
		if (abs(curTiltPosition - m_tiltTarget) <= c_tiltDeadband) {
			tiltSpeed = 0;
			shooterFlags += 1;
		}
	}

	m_tiltMotor->Set(-tiltSpeed);
	
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
		case sStart:
			if (curShootPosition > c_shootTriggerPosition - 100 || curShootPosition < 100) {
				shootSpeed = 1.0;
			} else {
				m_shootState = sLoad;
				shootSpeed = m_shootPID->Calculate(curShootPosition);
			}
			break;
			
		case sLoad:
		case sReady:
			if (curShootPosition > c_shootTriggerPosition - 100) {
				m_shootPID->SetPID(0.003, 0.0004, 0);      // 0.00005, 0.00007
			} else {
				m_shootPID->SetPID(0.003, 0, 0);
				m_shootPID->Reset();		
			}
			
			shootSpeed = m_shootPID->Calculate(curShootPosition);

			if (curShootPosition > c_shootTriggerPosition + 25) {
				printf("Idle Detected:  State=%d  Position=%d \n", m_shootState, curShootPosition);
//				m_shootState = sIdle;
//				shootSpeed = 0;
			}
			
			if (curShootPosition > c_shootTriggerPosition - 50) {
				m_shootState = sReady;
				shooterFlags += 12; // 8 + 4
			} else {
				m_shootState = sLoad;
				if (curShootPosition > m_RELEASETHEFRISBEEPOSITION) shooterFlags += 4;
			}
			break;
			
		case sFire:
			if (abs(curShootPosition - c_shootTriggerPosition) < 50 ){      // 100
				shootSpeed = 1.0;
			}else{
				shootSpeed = 0;
				m_shootState = sIdle;
				printf("Shoot State = %d\n", m_shootState);
			}
			break;
		
		default:
			shootSpeed = 0.0;
	}
	
	m_shootMotor->Set(-shootSpeed);
	
	if (m_shootState == sReady) shooterFlags += 16;
	if (m_shootState == sIdle) shooterFlags += 32;
	
	return shooterFlags;
}

void DiskShooter::FIREINTHEHOLE(){
	
	if(m_shootState == sReady){
		m_shootState = sFire;
	}
}

void DiskShooter::SetTensionTarget(INT32 Target){
	
	if(Target < 0){
		Target = 0;
	}else if (Target > c_tensionRange) {
		Target = c_tensionRange;
	}
	
	printf("Tension Target=%d\n", Target);
	m_tensionTarget = Target;
}

void DiskShooter::SetTiltTarget(INT32 Target){
	
	if(Target < 0){
		Target = 0;
	}else if(Target > c_tiltRange - 60){
		Target = c_tiltRange;
	}

	m_tiltTarget = Target;
	m_newTiltTarget = true;
}
