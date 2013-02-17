
#include "Hopper.h"

INT32 const c_tiltDeadband = 5;
INT32 const c_tiltSpan = 400;
INT32 const c_tiltZeroOffset = 300;
INT32 const c_spaceLimit = 120;


Hopper::Hopper(UINT8 	hopperGateModule,   UINT32 hopperGateChannel,
			   UINT8 	tiltMotorModule,    UINT32 tiltMotorChannel,
			   UINT8 	tiltPotModule,	    UINT32 tiltPotChannel,
			   UINT8 	beforeSensorModule, UINT32 beforeSensorChannel,
			   UINT8    afterSensorModule,  UINT32  afterSensorChannel,
			   Events  *eventHandler,	    UINT8  eventSourceId)
{
	m_hopperGate = new Relay(hopperGateModule, hopperGateChannel, Relay::kReverseOnly);
	
	m_tiltMotor = new Victor(tiltMotorModule, tiltMotorChannel);
	m_tiltMotor->SetExpiration(1.0);
	
	m_tiltPot = new AnalogChannel(tiltPotModule, tiltPotChannel);
	m_tiltPot->SetAverageBits(2);
	m_tiltPot->SetOversampleBits(0);

	m_beforeSensor = new DigitalInput(beforeSensorModule, beforeSensorChannel);
	m_afterSensor = new DigitalInput(afterSensorModule, afterSensorChannel);
	
	m_event = eventHandler;
	m_eventSourceId = eventSourceId;
	
	m_tiltPID = new PIDLoop(0.02, 0.002, 0.0);
	m_tiltPID->SetInputRange(0, (float) c_tiltSpan);
	m_tiltPID->SetOutputRange(-1.0, 1.0);

	m_tiltTarget = m_tiltPot->GetAverageValue() - c_tiltZeroOffset;
	m_newTiltTarget = true;
	m_hopState =hEmpty;
	m_frisbeeBeforeGate = false;
	m_pelicanStateEnabled = false;
}

Hopper::~Hopper(){
	
	delete m_hopperGate;
	delete m_tiltMotor;
	delete m_tiltPot;
	delete m_beforeSensor;
	delete m_afterSensor;
	delete m_tiltPID;
	delete m_event;
}

void Hopper::Disable(){
	
	m_tiltMotor->Set(0);
	m_tiltMotor->SetSafetyEnabled(false);
}

void Hopper::Enable(){
	
	m_hopState = hEmpty;
	m_hopperGate->Set(Relay::kOff);

	m_tiltTarget = m_tiltPot->GetAverageValue() - c_tiltZeroOffset;
	m_newTiltTarget = true;

	m_tiltMotor->Set(0);
	m_tiltMotor->SetSafetyEnabled(true);
	m_tiltPID->Reset();
}

INT32 Hopper::GetHopperPosition() {
	return m_tiltPot->GetAverageValue() - c_tiltZeroOffset;
}

void Hopper::PELICANMOVE(bool pelicanStateEnabled){

	if(m_beforeSensor->Get() == 0){				// Frisbee before gate
		m_pelicanStateEnabled = false;
	}else{
		m_pelicanStateEnabled = pelicanStateEnabled;
	}
}

int Hopper::Periodic(float joyValue, bool pickupOtterSpace){

	// hopperFlags:  Bit 1 = Hopper Tilt Completed
	// 					 2 = Frisbee Stored
	//                   4 = Hopper out of shared space
	
	static int		pelicanCounter = 0;
	static float	tiltSpeed = 0.0;
	
	INT32			curTiltPosition = m_tiltPot->GetAverageValue() - c_tiltZeroOffset;
	INT32           lowLimit = 0;
	int             hopperFlags = 0;

	if (pickupOtterSpace) {
		lowLimit = c_tiltDeadband;
	} else {
		lowLimit = c_spaceLimit;
	}
	
	if(joyValue != 0){
		m_pelicanStateEnabled = false;
		m_tiltTarget = curTiltPosition;
		m_newTiltTarget = true;
		
		if (joyValue > 0 && curTiltPosition > c_tiltSpan - c_tiltDeadband) {
			tiltSpeed = 0.0;
			hopperFlags += 1;
		}else if(joyValue < 0 && curTiltPosition < lowLimit) {
			tiltSpeed = 0.0;
			hopperFlags += 1;
		}else{
			tiltSpeed = joyValue;
			printf("Hopper Tilt=%d \n", curTiltPosition);
		}
		
	}else if(m_pelicanStateEnabled == true && m_beforeSensor->Get() == 1){
		pelicanCounter++;
		
		if(pelicanCounter <= 10){
			tiltSpeed = 1.0;
		}else if(pelicanCounter <= 15){
			tiltSpeed = 0.0;
		}else if(pelicanCounter <= 25){
			tiltSpeed = -1.0;
		}else if(pelicanCounter <= 30){
			tiltSpeed = 0;
		}else{
			pelicanCounter = 0;
		}
	
	}else{
		m_pelicanStateEnabled = false;
		
		if (m_newTiltTarget) {
			m_newTiltTarget = false;
//			m_tiltPID->SetSetpoint(m_tiltTarget);
			m_tiltPID->Reset();
		}
		
		if (!pickupOtterSpace && m_tiltTarget < c_spaceLimit) {
			m_tiltPID->SetSetpoint(c_spaceLimit);
		} else {
			m_tiltPID->SetSetpoint(m_tiltTarget);
		}
		
		tiltSpeed = m_tiltPID->Calculate((float) curTiltPosition);
		
		if (abs(curTiltPosition - m_tiltTarget) <= c_tiltDeadband) {
			tiltSpeed = 0;
			hopperFlags += 1;
		}
	}
	
	m_tiltMotor->Set(tiltSpeed);
	
	
	switch(m_hopState){
		case hEmpty:
			if(m_afterSensor->Get() == 0) {
				m_hopState = hStore;
			} else if(m_beforeSensor->Get() == 0) {			// Yes frisbee
				m_hopState = hLoad;
				m_hopperGate->Set(Relay::kOn);
			}
			break;
			
		case hLoad:			
			if(m_afterSensor->Get() == 0){					// Yes frisbee
				m_hopState = hStore;
				m_hopperGate->Set(Relay::kOff);
			}
			break;
			
		case hShoot:
			if(m_afterSensor->Get() == 1){					// No frisbee
				if (m_beforeSensor->Get() == 0 || m_frisbeeBeforeGate) {
					m_hopState = hLoad;
				} else {
					m_hopState = hEmpty;
					m_hopperGate->Set(Relay::kOff);
				}
			}
			break;
			
		default:;
	}
	
	if (m_afterSensor->Get() == 0) hopperFlags += 2;
	if (curTiltPosition > (c_spaceLimit - c_tiltDeadband)) hopperFlags +=4;
	
	return hopperFlags;
}

void Hopper::RELEASETHEFRISBEE(){	
	
	printf("Release HopState=%d\n", m_hopState);
	
	if(m_hopState == hStore){
		m_hopState = hShoot;
		m_frisbeeBeforeGate = (m_beforeSensor->Get() == 0);
		m_hopperGate->Set(Relay::kOn);
	}
}

void Hopper::SetTiltTarget(INT32 Target){
	
	if(Target < 0){
		Target = 0;
	}else if(Target > c_tiltSpan){
		Target = c_tiltSpan;
	}
	
	m_tiltTarget = Target;
	m_newTiltTarget = true;
}
