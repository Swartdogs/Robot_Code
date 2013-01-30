//************Team 525 (2013)************//
//
// Pickup Mechanism Code

#ifndef PICKUP_H_
#define PICKUP_H_

#include "Talon.h"
#include "DigitalInput.h"
#include "AnalogChannel.h"
#include "Events.h"

class Events;

class Pickup{	

public:
	
typedef enum EnumRunMode{kShoot, kLoad, kDump, kIdle};
	
	Pickup(
			UINT8  pickupMotorModule,	UINT32 pickupMotorChannel,
			UINT8  flipMotorModule,		UINT32 flipMotorChannel,
			UINT8  lightSensorModule,	UINT32 lightSensorChannel,
			UINT8  potModule,			UINT32 potChannel,
			Events *eventHandler,		UINT8  eventSourceId);
	
	~Pickup();
	void  Enable();
	void  Disable();
	void  FeedSafety();
	UINT8 Run(EnumRunMode RunMode);
	
private:
	
	Talon 		  *m_pickupMotor;
	Talon 		  *m_flipMotor;
	DigitalInput  *m_lightSensor;
	AnalogChannel *m_pot;
	Events 		  *m_event;
	char   		   m_Log[100];
};

#endif
