//************Team 525 (2013)************//
//
// Pickup Mechanism Code

#include "DiskPickup.h"

DiskPickup::DiskPickup(
		UINT8  pickupMotorModule,	UINT32 pickupMotorChannel,
		UINT8  flipMotorModule,		UINT32 flipMotorChannel,
		UINT8  diskSensorModule,	UINT32 diskSensorChannel,
		UINT8  flipPotModule,		UINT32 flipPotChannel,
		Events *eventHandler,		UINT8  eventSourceId)
{	
	m_pickupMotor = new Talon(pickupMotorModule, pickupMotorChannel);
	m_pickupMotor->SetExpiration(1.0);
	
	m_flipMotor = new Talon(flipMotorModule, flipMotorChannel);
	m_flipMotor->SetExpiration(1.0);

	m_diskSensor = new DigitalInput(diskSensorModule, diskSensorChannel);
	
	m_flipPot = new AnalogChannel(flipPotModule, flipPotChannel);
	m_flipPot->SetAverageBits(2);
	m_flipPot->SetOversampleBits(0);
}

DiskPickup::~DiskPickup(){
	delete m_pickupMotor;
	delete m_flipMotor;
	delete m_diskSensor;
	delete m_flipPot;
	delete m_event;
}

void DiskPickup::Enable(){
	m_pickupMotor->Set(0);
	m_pickupMotor->SetSafetyEnabled(true);
	
	m_flipMotor->Set(0);
	m_flipMotor->SetSafetyEnabled(true);
}

void DiskPickup::Disable(){
	m_pickupMotor->Set(0);
	m_pickupMotor->SetSafetyEnabled(false);
	
	m_flipMotor->Set(0);
	m_flipMotor->SetSafetyEnabled(false);
}

void DiskPickup::FeedSafety(){
	m_pickupMotor->Set(0);
	m_flipMotor->Set(0);
}
