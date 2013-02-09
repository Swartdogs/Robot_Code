
#include "Hopper.h"

INT32 const c_tiltDeadband = 25;
INT32 const c_tiltSpan = 600;
INT32 const c_tiltZeroOffset = 0;

Hopper::Hopper(UINT8 	hopperGateModule,   UINT32 hopperGateChannel,
			   UINT8 	tiltMotorModule,    UINT32 tiltMotorChannel,
			   UINT8 	tiltPotModule,	    UINT32 tiltPotChannel,
			   UINT8 	beforeSensorModule, UINT32 beforeSensorChannel,
			   UINT8    afterSensorModule,  UINT32  afterSensorChannel,
			   Events  *eventHandler,	    UINT8  eventSourceId)
{
	m_hopperGate = new Relay(hopperGateModule, hopperGateChannel, Relay::kForwardOnly);
	
	m_tiltMotor = new Victor(tiltMotorModule, tiltMotorChannel);
	m_tiltMotor->SetExpiration(1.0);
	
	m_tiltPot = new AnalogChannel(tiltPotModule, tiltPotChannel);
	m_tiltPot->SetAverageBits(2);
	m_tiltPot->SetOversampleBits(0);
	
	m_beforeSensor = new DigitalInput(beforeSensorModule, beforeSensorChannel);
	m_afterSensor = new DigitalInput(afterSensorModule, afterSensorChannel);
	
	m_event = eventHandler;
	m_eventSourceId = eventSourceId;
	
	if(m_afterSensor->Get() == 0){
		m_hopState = hStore;
	}else if(m_beforeSensor->Get() == 0){
		m_hopState = hLoad;
	} else {
		m_hopState = hEmpty;
	}
	
	m_tiltTarget = m_tiltPot->GetAverageValue();
}

Hopper::~Hopper(){
	
	delete m_hopperGate;
	delete m_tiltMotor;
	delete m_tiltPot;
	delete m_beforeSensor;
	delete m_event;
}

void Hopper::Disable(){	
	m_tiltMotor->Set(0);
	m_tiltMotor->SetSafetyEnabled(false);
}

void Hopper::Enable(){	
	m_tiltMotor->Set(0);
	m_tiltMotor->SetSafetyEnabled(true);
}

void Hopper::PELICANMOVE(bool pelicanStateEnabled){

	if(m_beforeSensor->Get() == 1){
		m_pelicanStateEnabled = false;
	}else{
		m_pelicanStateEnabled = pelicanStateEnabled;
	}
}

int Hopper::Periodic(float joyValue){

	// hopperFlags:  Bit 1 = Hopper Tilt Completed
	//                   2 = Shoot Gate Open
	// 					 4 = Frisbee Stored
	
	static int		pelicanCounter = 0;
	static float	tiltSpeed = 0.0;

	
	INT32			curTiltPosition = m_tiltPot->GetAverageValue() - c_tiltZeroOffset;
	int             hopperFlags = 0;
	
	
	if(joyValue != 0){
		m_tiltTarget = curTiltPosition;

		if (joyValue > 0 && curTiltPosition > c_tiltSpan - c_tiltDeadband) {
			tiltSpeed = 0.0;
		}else if(joyValue < 0 && curTiltPosition < c_tiltDeadband) {
			tiltSpeed = 0.0;
		}else{
			tiltSpeed = joyValue;
		}
		
	}else if(m_pelicanStateEnabled == true && m_beforeSensor->Get() == 0){
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
		
		if(curTiltPosition < m_tiltTarget - c_tiltDeadband){
			tiltSpeed = 1.0;
		}else if(curTiltPosition > m_tiltTarget + c_tiltDeadband) {
			tiltSpeed = -1.0;
		}else{
			tiltSpeed = 0.0;
			hopperFlags += 1;
		}
	}
	
	m_tiltMotor->Set(tiltSpeed);
	
	switch(m_hopState){
		case hEmpty:
			if(m_beforeSensor->Get() == 0) {
				m_hopState = hLoad;
			} else {
				m_hopperGate->Set(Relay::kOff);
			}
			break;
			
		case hLoad:			
			m_hopperGate->Set(Relay::kOn);
			if(m_afterSensor->Get() == 0){					// Yes frisbee
				m_hopState = hStore;
			}
			break;
			
		case hShoot:
			m_hopperGate->Set(Relay::kOn);
			if(m_afterSensor->Get() == 1){					// No frisbee
				m_hopState = hEmpty;
			}
			break;
			
		case hStore:
			m_hopperGate->Set(Relay::kOff);
			break;
			
		default:;
	}
	
	if (m_hopState == hShoot) hopperFlags += 2;
	if (m_beforeSensor->Get() == 0) hopperFlags += 4;
	
	return hopperFlags;
}

void Hopper::RELEASETHEFRISBEE(){	
	
	if(m_beforeSensor->Get() == 0){
		m_hopState = hShoot;
	}
}

void Hopper::SetTiltTarget(INT32 Target){
	
	if(Target < 0){
		Target = 0;
	}else if(Target > c_tiltSpan){
		Target = c_tiltSpan;
	}
	
	m_tiltTarget = Target;
}
