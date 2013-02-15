//************Team 525 (2013)************//
//
// Pickup Mechanism Code

#include "DiskPickup.h"

INT32 c_armRange = 500;
INT32 c_wristRange = 500;
INT32 c_armLoad = 100;
INT32 c_armStore = 250;
INT32 c_armDeployed = 500;
INT32 c_wristLoad = 100;
INT32 c_wristStore = 300;
INT32 c_wristDeployed = 500;

DiskPickup::DiskPickup(
					UINT8	pickupMotorModule,  UINT32 pickupMotorChannel,
					UINT8   armMotorModule,	    UINT32 armMotorChannel,
					UINT8   wristMotorModule,	UINT32 wristMotorChannel,
					UINT8   diskSensorModule,	UINT32 diskSensorChannel,
					UINT8   armPotModule,		UINT32 armPotChannel,
					UINT8   wristPotModule,		UINT32 wristPotChannel,
					Events *eventHandler,		UINT8  eventSourceId)
{	
	m_pickupMotor = new Relay(pickupMotorModule, pickupMotorChannel, Relay::kBothDirections);
	
	m_armMotor = new Victor(armMotorModule, armMotorChannel);
	m_armMotor->SetExpiration(1.0);
	
	m_wristMotor = new Victor(wristMotorModule, wristMotorChannel);
	m_wristMotor->SetExpiration(1.0);

	m_diskSensor = new DigitalInput(diskSensorModule, diskSensorChannel);
	
	m_armPot = new AnalogChannel(armPotModule, armPotChannel);
	m_armPot->SetAverageBits(2);
	m_armPot->SetOversampleBits(0);
	
	m_wristPot = new AnalogChannel(wristPotModule, wristPotChannel);
	m_wristPot->SetAverageBits(2);
	m_wristPot->SetOversampleBits(0);
	
	m_armTiltTarget = m_armPot->GetAverageValue();
	m_wristTiltTarget = m_wristPot->GetAverageValue();
	
	m_runMode = pIdle;
	
	m_armPID = new PIDLoop(0.0015,			// P coefficient
						   0,				// I coefficient
						   0);				// D coefficient
	
	m_wristPID = new PIDLoop(0.0015,		// P coefficient
							 0,				// I coefficient
							 0);			// D coefficient
}

DiskPickup::~DiskPickup(){
	delete m_pickupMotor;
	delete m_wristMotor;
	delete m_armMotor;
	delete m_diskSensor;
	delete m_wristPot;
	delete m_armPot;
	delete m_wristPID;
	delete m_armPID;
	delete m_event;
}

void DiskPickup::Enable(){
	m_pickupMotor->Set(Relay::kOff);

	m_wristMotor->Set(0);
	m_wristMotor->SetSafetyEnabled(true);
	
	m_armMotor->Set(0);
	m_armMotor->SetSafetyEnabled(true);
	
	m_wristPID->Reset();
	m_armPID->Reset();
}

void DiskPickup::Disable(){
	m_pickupMotor->Set(Relay::kOff);
	
	m_wristMotor->Set(0);
	m_wristMotor->SetSafetyEnabled(false);
	
	m_armMotor->Set(0);
	m_armMotor->SetSafetyEnabled(false);
}

void DiskPickup::FeedSafety(){
	m_wristMotor->Set(0);
	m_armMotor->Set(0);
}

void DiskPickup::Periodic(PickupRunMode RunMode){
	INT32			curArmPosition = m_armPot->GetAverageValue();
	INT32   		curWristPosition = m_wristPot->GetAverageValue();
	static float	armTiltSpeed = 0.0;
	static float	wristTiltSpeed = 0.0;
	
//-------------------------------Set Arm/Wrist-----------------------------
	
	if (m_pickPos == pLoadShooter){
		m_armTiltTarget = c_armLoad;
		m_wristTiltTarget = c_wristLoad;
	} else if (m_pickPos == pStored){
		m_armTiltTarget = c_armStore;
		m_wristTiltTarget = c_wristStore;
	} else if (m_pickPos == pDeployed){
		m_armTiltTarget = c_armDeployed;
		m_wristTiltTarget = c_wristDeployed;
	}
	
	switch(RunMode){
		case pLoad:
			m_pickPos = pLoadShooter;
			m_armPID->SetSetpoint((float)m_armTiltTarget);
			m_wristPID->SetSetpoint((float)m_wristTiltTarget);
			if(m_diskSensor->Get() == 1){
				m_pickupMotor->Set(Relay::kForward);
			} else {
				m_pickupMotor->Set(Relay::kOff);
			}
			break;
			
		case pIdle:
			m_armTiltTarget = curArmPosition;
			m_wristTiltTarget = curWristPosition;
			m_armPID->SetSetpoint((float)m_armTiltTarget);
			m_wristPID->SetSetpoint((float)m_wristTiltTarget);
			m_pickupMotor->Set(Relay::kOff);
			break;
			
		case pGetFrisbee:
			m_pickPos = pDeployed;
			m_armPID->SetSetpoint((float)m_armTiltTarget);
			m_wristPID->SetSetpoint((float)m_wristTiltTarget);
			if(m_diskSensor->Get() == 1){
				m_pickupMotor->Set(Relay::kForward);
			} else {
				m_pickupMotor->Set(Relay::kOff);
			}
			break;
	}
	
	armTiltSpeed = m_armPID->Calculate((float)curArmPosition);
	wristTiltSpeed = m_wristPID->Calculate((float)curWristPosition);
	
	m_armMotor->Set(armTiltSpeed);
	m_wristMotor->Set(wristTiltSpeed);
}
