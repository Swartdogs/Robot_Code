
#include "Hopper.h"

INT32 const c_storagePosition = 0;
float const c_shootGateClosed = 1;
float const c_shootGateOpen = 0;
float const c_hopGateClosed = 1;
float const c_hopGateOpen = 0;

Hopper::Hopper(UINT8 	shootGateModule,  UINT32 shootGateChannel,
			   UINT8 	hopperGateModule, UINT32 hopperGateChannel,
			   UINT8 	hopperTiltModule, UINT32 hopperTiltChannel,
			   UINT8 	tiltPotModule,	  UINT32 tiltPotChannel,
			   UINT8 	diskSensorModule, UINT32 diskSensorChannel,
			   Events  *eventHandler,	  UINT8  eventSourceId)
{
	m_shootGate = new Servo(shootGateModule, shootGateChannel);
	m_hopperGate = new Servo(hopperGateModule, hopperGateChannel);
	
	m_hopperTiltMotor = new Jaguar(hopperTiltModule, hopperTiltChannel);
	m_hopperTiltPot = new AnalogChannel(tiltPotModule, tiltPotChannel);
	m_hopperTiltPot->SetAverageBits(2);
	m_hopperTiltPot->SetOversampleBits(0);
	
	m_diskSensor = new DigitalInput(diskSensorModule, diskSensorChannel);
	
	m_event = eventHandler;
	m_eventSourceId = eventSourceId;
	
	if(m_diskSensor->Get() == 1){
		m_hopState = hLoad;
	}else{
		m_hopState = hStore;
	}
	
	printf("State = %d \n", m_hopState);
	
	m_tiltTarget = m_hopperTiltPot->GetAverageValue();
	
	printf("Tilt Target = %d \n", m_tiltTarget);
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
	if(m_diskSensor->Get() == 0){
		m_pelicanStateEnabled = false;
	}else{
		m_pelicanStateEnabled = pelicanStateEnabled;
	}
}

void Hopper::Periodic(){
	static INT32	curHopTiltTarget = m_tiltTarget;
	INT32			curHopTiltPosition = m_hopperTiltPot->GetAverageValue() - c_storagePosition;
	static float	tiltSpeed = 0.0;
	INT32			deadband = 25;
	static int		periodicCounter;
	
	printf("Tilt Target = %d \n", curHopTiltPosition);

	if(m_pelicanStateEnabled == false || m_diskSensor->Get() == 0){
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
			m_shootGate->Set(c_shootGateClosed);
			m_hopperGate->Set(c_hopGateOpen);
			if(m_diskSensor->Get() == 0){
				m_hopState = hStore;
			}
			break;
			
		case hShoot:
			m_shootGate->Set(c_shootGateOpen);
			m_hopperGate->Set(c_hopGateClosed);
			if(m_diskSensor->Get() == 1){
				m_hopState = hLoad;
			}
			break;
			
		case hStore:
			m_shootGate->Set(c_shootGateClosed);
			m_hopperGate->Set(c_hopGateClosed);
			break;
			
		default:;
	}
}

void Hopper::RELEASETHEFRISBEES(){	
	if(m_diskSensor->Get() == 0){
		m_hopState = hShoot;
	}
}

void Hopper::SetHopperTiltTarget(INT32 Target){
	m_tiltTarget = Target;
}
