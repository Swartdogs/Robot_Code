
#include "Hopper.h"

Hopper::Hopper(UINT8 	shootGateModule,  UINT32 shootGateChannel,
			   UINT8 	hopperGateModule, UINT32 hopperGateChannel,
			   UINT8 	hopperTiltModule, UINT32 hopperTiltChannel,
			   UINT8 	tiltPotModule,	  UINT32 tiltPotChannel,
			   UINT8    storagePosition,
			   float	shootGateClosed,  float  shootGateOpen,
			   float    hopGateClosed,    float  hopGateOpen,
			   UINT8 	diskSensorModule, UINT32 diskSensorChannel,
			   Events  *eventHandler,	  UINT8  eventSourceId)
{
	m_shootGate = new Servo(shootGateModule, shootGateChannel);
	m_hopperGate = new Servo(hopperGateModule, hopperGateChannel);
	
	m_hopperTiltMotor = new Jaguar(hopperTiltModule, hopperTiltChannel);
	m_hopperTiltPot = new AnalogChannel(tiltPotModule, tiltPotChannel);
	m_hopperTiltPot->SetAverageBits(2);
	m_hopperTiltPot->SetOversampleBits(0);
	
	m_storagePosition = storagePosition;
	
	m_diskSensor = new DigitalInput(diskSensorModule, diskSensorChannel);
	
	m_event = eventHandler;
	m_eventSourceId = eventSourceId;
	
	m_shootGateClosedPos = shootGateClosed;
	m_shootGateOpenPos = shootGateOpen;
	
	m_hopGateClosedPos = hopGateClosed;
	m_hopGateOpenPos = hopGateOpen;
	
	if(m_diskSensor->Get() == 0){
		m_hopState = hLoad;
	}else{
		m_hopState = hStore;
	}
	
	m_tiltTarget = m_hopperTiltPot->GetAverageValue();
}

Hopper::~Hopper(){
	delete m_shootGate;
	delete m_hopperGate;
	delete m_hopperTiltMotor;
	delete m_hopperTiltPot;
	delete m_diskSensor;
	delete m_event;
}

void Hopper::Disable(){
	m_shootGate->SetSafetyEnabled(false);
	
	m_hopperGate->SetSafetyEnabled(false);
	
	m_hopperTiltMotor->Set(0);
	m_hopperTiltMotor->SetSafetyEnabled(false);
}

void Hopper::Enable(){
	m_shootGate->SetSafetyEnabled(true);
	
	m_hopperGate->SetSafetyEnabled(true);
	
	m_hopperTiltMotor->Set(0);
	m_hopperTiltMotor->SetSafetyEnabled(true);
}

void Hopper::PELICANMOVE(bool pelicanStateEnabled){
	if(m_diskSensor->Get() == 1){
		m_pelicanStateEnabled = false;
	}else{
		m_pelicanStateEnabled = pelicanStateEnabled;
	}
}

void Hopper::Periodic(){
	static INT32	curHopTiltTarget = m_tiltTarget;
	INT32			curHopTiltPosition = m_hopperTiltPot->GetAverageValue() - m_storagePosition;
	static float	tiltSpeed = 0.0;
	INT32			deadband = 25;
	static int		periodicCounter;
	
	if(m_pelicanStateEnabled == false || m_diskSensor->Get() == 1){
		m_pelicanStateEnabled = false;
		if(curHopTiltTarget != m_tiltTarget){
			curHopTiltTarget = m_tiltTarget;
			if(curHopTiltPosition < curHopTiltTarget - deadband){
				tiltSpeed = 1.0;
			}else if(curHopTiltPosition > curHopTiltTarget + deadband){
				tiltSpeed = -1.0;
			}
		}else if(tiltSpeed > 0.0){
			if(curHopTiltPosition >= curHopTiltTarget) tiltSpeed = 0.0;
		}else if(tiltSpeed < 0.0){
			if(curHopTiltPosition <= curHopTiltTarget) tiltSpeed = 0.0;
		}
	}else{
		periodicCounter++;
		if(periodicCounter <= 10){
			tiltSpeed = 1.0;
		}else if(periodicCounter <= 15){
			tiltSpeed = 0.0;
		}else if(periodicCounter <= 25){
			tiltSpeed = -1.0;
		}else if(periodicCounter <= 30){
			tiltSpeed = 0;
		}else{
			periodicCounter = 0;
		}
	}
	
	m_hopperTiltMotor->Set(tiltSpeed);
		
	switch(m_hopState){
		case hLoad:
			m_shootGate->Set(m_shootGateClosedPos);
			m_hopperGate->Set(m_hopGateOpenPos);
			if(m_diskSensor->Get() == 1){
				m_hopState = hStore;
			}
			break;
			
		case hShoot:
			m_shootGate->Set(m_shootGateOpenPos);
			m_hopperGate->Set(m_hopGateClosedPos);
			if(m_diskSensor->Get() == 0){
				m_hopState = hLoad;
			}
			break;
			
		case hStore:
			m_shootGate->Set(m_shootGateClosedPos);
			m_hopperGate->Set(m_hopGateClosedPos);
			break;
			
		default:;
	}
}

void Hopper::RELEASETHEFRISBEES(){	
	if(m_diskSensor->Get() == 1){
		m_hopState = hShoot;
	}
}

void Hopper::SetHopperTiltTarget(INT32 Target){
	m_tiltTarget = Target;
}
