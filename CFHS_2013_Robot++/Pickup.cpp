//************Team 525 (2013)************//
//
// Pickup Mechanism Code

#include "Pickup.h"

Pickup::Pickup(
		UINT8  pickupMotorModule,	UINT32 pickupMotorChannel,
		UINT8  flipMotorModule,		UINT32 flipMotorChannel,
		UINT8  lightSensorModule,	UINT32 lightSensorChannel,
		UINT8  potModule,			UINT32 potChannel,
		Events *eventHandler,		UINT8  eventSourceId)
{	
	m_pickupMotor = new Talon(pickupMotorModule, pickupMotorChannel);
	m_pickupMotor->SetExpiration(1.0);
	
	m_flipMotor = new Talon(flipMotorModule, flipMotorChannel);
	m_flipMotor->SetExpiration(1.0);

	m_lightSensor = new DigitalInput(lightSensorModule, lightSensorChannel);
	
	m_pot = new AnalogChannel(potModule, potChannel);
	m_pot->SetAverageBits(2);
	m_pot->SetOversampleBits(0);
}

Pickup::~Pickup(){
	delete m_pickupMotor;
	delete m_flipMotor;
	delete m_lightSensor;
	delete m_pot;
}

void Pickup::Enable(){
	m_pickupMotor->Set(0);
	m_pickupMotor->SetSafetyEnabled(true);
	
	m_flipMotor->Set(0);
	m_flipMotor->SetSafetyEnabled(true);
}

void Pickup::Disable(){
	m_pickupMotor->Set(0);
	m_pickupMotor->SetSafetyEnabled(false);
	
	m_flipMotor->Set(0);
	m_flipMotor->SetSafetyEnabled(false);
}

void Pickup::FeedSafety(){
	m_pickupMotor->Set(0);
	m_flipMotor->Set(0);
}
