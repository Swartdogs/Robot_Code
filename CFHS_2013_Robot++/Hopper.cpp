
#include "Hopper.h"

float const c_loadGateClosed = 1.0;
float const c_loadGateOpen = 0.0;
float const c_shootGateClosed = 1.0;
float const c_shootGateOpen = 0.0;
INT32 const c_tiltDeadband = 25;
INT32 const c_tiltSpan = 600;
INT32 const c_tiltZeroOffset = 0;

Hopper::Hopper(UINT8 	shootGateModule,  UINT32 shootGateChannel,
			   UINT8 	loadGateModule,   UINT32 loadGateChannel,
			   UINT8 	tiltMotorModule,  UINT32 tiltMotorChannel,
			   UINT8 	tiltPotModule,	  UINT32 tiltPotChannel,
			   UINT8 	diskSensorModule, UINT32 diskSensorChannel,
			   Events  *eventHandler,	  UINT8  eventSourceId)
{
	m_shootGate = new Servo(shootGateModule, shootGateChannel);
	
	m_loadGate = new Servo(loadGateModule, loadGateChannel);
	
	m_tiltMotor = new Victor(tiltMotorModule, tiltMotorChannel);
	m_tiltMotor->SetExpiration(1.0);
	
	m_tiltPot = new AnalogChannel(tiltPotModule, tiltPotChannel);
	m_tiltPot->SetAverageBits(2);
	m_tiltPot->SetOversampleBits(0);
	
	m_diskSensor = new DigitalInput(diskSensorModule, diskSensorChannel);
	
	m_event = eventHandler;
	m_eventSourceId = eventSourceId;
	
	if(m_diskSensor->Get() == 1){
		m_hopState = hLoad;
	}else{
		m_hopState = hStore;
	}
	
	m_tiltTarget = m_tiltPot->GetAverageValue();
}

Hopper::~Hopper(){
	
	delete m_shootGate;
	delete m_loadGate;
	delete m_tiltMotor;
	delete m_tiltPot;
	delete m_diskSensor;
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

	if(m_diskSensor->Get() == 1){
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
	static int      periodicCounter = 0;
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
		
	}else if(m_pelicanStateEnabled == true && m_diskSensor->Get() == 0){
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
		case hLoad:			
			m_shootGate->Set(c_shootGateClosed);
			m_loadGate->Set(c_loadGateOpen);
			if(m_diskSensor->Get() == 0){					// Yes frisbee
				m_hopState = hStore;
			}
			break;
			
		case hShoot:
			m_shootGate->Set(c_shootGateOpen);
			m_loadGate->Set(c_loadGateClosed);
			if(m_diskSensor->Get() == 1){					// No frisbee
				periodicCounter++;
				if(periodicCounter >=5){
					m_hopState = hLoad;
					periodicCounter = 0;
				}
			}
			break;
			
		case hStore:
			m_shootGate->Set(c_shootGateClosed);
			m_loadGate->Set(c_loadGateClosed);
			break;
			
		default:;
	}
	
	if (m_hopState == hShoot) hopperFlags += 2;
	if (m_diskSensor->Get() == 0) hopperFlags += 4;
	
	return hopperFlags;
}

void Hopper::RELEASETHEFRISBEE(){	
	
	if(m_diskSensor->Get() == 0){
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
