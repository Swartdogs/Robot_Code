//************Team 525 (2013)************//
//
// Pickup Mechanism Code

#include "DiskPickup.h"

INT32 const c_armRange = 500;
INT32 const c_armZeroOffset = 200;
INT32 const c_wristRange = 500;
INT32 const c_wristZeroOffset = 100;

INT32 const c_armLoad = c_armZeroOffset - 100;
INT32 const c_armStore = c_armZeroOffset + 250;
INT32 const c_armDeployed = c_armZeroOffset + 300;
INT32 const c_armPyramid = c_armZeroOffset + 275;
INT32 const c_wristLoad = c_wristZeroOffset;
INT32 const c_wristStore = c_wristZeroOffset + 200;
INT32 const c_wristDeployed = c_wristZeroOffset + 400;
INT32 const c_wristPyramid = c_wristZeroOffset + 350;

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
	
	m_armTiltTarget = m_armPot->GetAverageValue() - c_armZeroOffset;
	m_wristTiltTarget = m_wristPot->GetAverageValue() - c_wristZeroOffset;
	
	m_runMode = pStore;
	
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
	m_armTiltTarget = m_armPot->GetAverageValue() - c_armZeroOffset;
	m_wristTiltTarget = m_wristPot->GetAverageValue() - c_wristZeroOffset;

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
	INT32					curArmPosition = m_armPot->GetAverageValue() - c_armZeroOffset;
	INT32   				curWristPosition = m_wristPot->GetAverageValue() - c_wristZeroOffset;
	static float			armTiltSpeed = 0.0;
	static float			wristTiltSpeed = 0.0;
	static PickupRunMode	runModeNow;
	
//-------------------------------Set Arm/Wrist-----------------------------
	if(runModeNow != RunMode){
		runModeNow = RunMode;
		
		switch(RunMode){
			case pLoad:
				m_armTiltTarget = c_armLoad;
				m_wristTiltTarget = c_wristLoad;
				m_armPID->SetSetpoint((float)m_armTiltTarget);
				m_wristPID->SetSetpoint((float)m_wristTiltTarget);
				if(m_diskSensor->Get() == 1){
					m_pickupMotor->Set(Relay::kForward);
				} else {
					m_pickupMotor->Set(Relay::kOff);
				}
				break;
				
			case pStore:
				m_armTiltTarget = c_armStore;
				m_wristTiltTarget = c_wristStore;
				m_armPID->SetSetpoint((float)m_armTiltTarget);
				m_wristPID->SetSetpoint((float)m_wristTiltTarget);
				m_pickupMotor->Set(Relay::kOff);
				break;
				
			case pDeployed:
				m_armTiltTarget = c_armDeployed;
				m_wristTiltTarget = c_wristDeployed;
				m_armPID->SetSetpoint((float)m_armTiltTarget);
				m_wristPID->SetSetpoint((float)m_wristTiltTarget);
				if(m_diskSensor->Get() == 1){
					m_pickupMotor->Set(Relay::kForward);
				} else {
					m_pickupMotor->Set(Relay::kOff);
				}
				break;
				
			case pUnderPyramid:
				m_armTiltTarget = c_armPyramid;
				m_wristTiltTarget = c_wristPyramid;
				m_armPID->SetSetpoint((float)m_armTiltTarget);
				break;
				
			default:;
			
		}
	} else {
		
	}
	
	armTiltSpeed = m_armPID->Calculate((float)curArmPosition);
	wristTiltSpeed = m_wristPID->Calculate((float)curWristPosition);
	
	m_armMotor->Set(armTiltSpeed);
	m_wristMotor->Set(wristTiltSpeed);
}
