//************Team 525 (2013)************//
//
// Pickup Mechanism Code

#include "DiskPickup.h"

INT32 const c_armRange = 700;
INT32 const c_armZeroOffset = 780;
INT32 const c_wristRange = 350;
INT32 const c_wristZeroOffset = 800;

INT32 const c_deadband = 20;

INT32 const c_armLoad = c_armZeroOffset - 240; 			//When loading onto shooter deck
INT32 const c_armStore = c_armZeroOffset - 330; 		//default storage position
INT32 const c_armDeployed = c_armZeroOffset - 770; 		//When arm is picking up disks
INT32 const c_armPyramid = c_armZeroOffset - 580; 		//When going under pyramid
INT32 const c_wristLoad = c_wristZeroOffset - 570;
INT32 const c_wristStore = c_wristZeroOffset - 620;
INT32 const c_wristDeployed = c_wristZeroOffset - 800;
INT32 const c_wristPyramid = c_wristZeroOffset - 720;

INT32 const c_clearBumper = c_armZeroOffset - 670;

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
	
	m_armPID = new PIDLoop(0.007,			// P coefficient
						   0,				// I coefficient
						   0);				// D coefficient
//	m_armPID->SetInputRange(c_armZeroOffset - 630, c_armZeroOffset + 70);
	m_armPID->SetInputRange(0, c_armRange);
	m_armPID->SetOutputRange(-1.0, 1.0);		//-1, 1
	
	m_wristPID = new PIDLoop(0.008,		    // P coefficient
							 0,				// I coefficient
							 0);            // D coefficient
	m_wristPID->SetInputRange(0, c_wristRange);
	m_wristPID->SetOutputRange(-0.8, 0.8);		//-.8 .8
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
	m_armTiltTarget = c_armZeroOffset - m_armPot->GetAverageValue();
	m_wristTiltTarget = c_wristZeroOffset - m_wristPot->GetAverageValue();

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

int DiskPickup::Periodic(PickupRunMode *RunMode, int *sharedSpace) {

	// Pickup Flags:		1 = Frisbee in pickup
	
	INT32					curArmPosition = c_armZeroOffset - m_armPot->GetAverageValue();
	INT32   				curWristPosition = c_wristZeroOffset - m_wristPot->GetAverageValue();
	int                     pickupFlags = 0;
	static float			armTiltSpeed = 0.0;
	static float			wristTiltSpeed = 0.0;
	static PickupRunMode	runModeNow = pArgggggggggggggh;
	static bool             runPickupMotor = false;
	
//-------------------------------Set Arm/Wrist-----------------------------
	
	if(abs(m_armTiltTarget - curArmPosition) <= c_deadband && abs(m_wristTiltTarget - curWristPosition) <= c_deadband) {
		runPickupMotor = (runModeNow == pDeployed || runModeNow == pLoad);
	}
	
	if(runPickupMotor) {
		if(runModeNow == pDeployed) {
			if(m_diskSensor->Get() == 1){
				m_pickupMotor->Set(Relay::kReverse);
			} else {
				m_pickupMotor->Set(Relay::kOff);
				*RunMode = pStore;
				runPickupMotor = false;
			}
		} else if(runModeNow == pLoad) {
			if(m_diskSensor->Get() == 0){
				m_pickupMotor->Set(Relay::kReverse);
			} else {
				m_pickupMotor->Set(Relay::kOff);
				*RunMode = pStore;
				runPickupMotor = false;
			}
		}
	}
	
	if(runModeNow != *RunMode){
		runModeNow = *RunMode;
		
		m_pickupMotor->Set(Relay::kOff);
		
		switch(runModeNow){
			case pLoad:
				m_armTiltTarget = c_armLoad;
				m_wristTiltTarget = c_wristLoad;
				break;
				
			case pStore:
				m_armTiltTarget = c_armStore;
				m_wristTiltTarget = c_wristStore;
				break;
				
			case pDeployed:
				m_armTiltTarget = c_armDeployed;
				m_wristTiltTarget = c_wristDeployed;
				break;
				
			case pUnderPyramid:
				m_armTiltTarget = c_armPyramid;
				m_wristTiltTarget = c_wristPyramid;
				break;
				
			default:;
			
		}
		m_armPID->SetSetpoint((float)m_armTiltTarget);
				
	}
	
	float Setpoint = 0;
	
	if(curArmPosition < c_clearBumper) { //if arm hasn't cleared the bumper
		Setpoint = curArmPosition / 2.0;
		m_wristPID->SetSetpoint((float)Setpoint);
	} else {
		Setpoint = m_wristTiltTarget;
		m_wristPID->SetSetpoint((float)m_wristTiltTarget);
	}
	
	if (*sharedSpace == 1 && m_armTiltTarget > c_armPyramid) {			// Hopper in shared space
		m_armPID->SetSetpoint(c_armPyramid);
		m_wristPID->SetSetpoint(c_wristPyramid);
	} else {
		m_armPID->SetSetpoint(m_armTiltTarget);
	}
	
	armTiltSpeed = m_armPID->Calculate((float)curArmPosition);
	wristTiltSpeed = m_wristPID->Calculate((float)curWristPosition);
	
//	printf("Wrist Target=%d  Wrist Position=%d Wrist Tilt Speed=%f Setpoint=%f Arm Position=%d\n", 
//			m_wristTiltTarget, curWristPosition, wristTiltSpeed, Setpoint, curArmPosition);
	
	m_armMotor->Set(-armTiltSpeed);   
	m_wristMotor->Set(wristTiltSpeed);
	
	if (*sharedSpace == 0  && curArmPosition > (c_armPyramid - c_deadband)) {
		*sharedSpace = 2;
	} else if (*sharedSpace == 2 && curArmPosition < (c_armPyramid + c_deadband)) {
		*sharedSpace = 0;
	}

	if (m_diskSensor->Get() == 0){
		pickupFlags += 1;
	}
	
	return pickupFlags;
}
