//************Team 525 (2013)************//
//
// Pickup Mechanism Code

#ifndef DISKPICKUP_H_
#define DISKPICKUP_H_

#include "Talon.h"
#include "DigitalInput.h"
#include "AnalogChannel.h"
#include "Events.h"

class DiskPickup{	

public:
	
typedef enum {kIdle, kLoad, kDump, kShoot}PickupRunMode;
	
	DiskPickup(
			UINT8  pickupMotorModule,	UINT32 pickupMotorChannel,
			UINT8  flipMotorModule,		UINT32 flipMotorChannel,
			UINT8  diskSensorModule,	UINT32 diskSensorChannel,
			UINT8  flipPotModule,		UINT32 flipPotChannel,
			Events *eventHandler,		UINT8  eventSourceId);
	
	~DiskPickup();
	void  Enable();
	void  Disable();
	void  FeedSafety();
	void  Periodic(PickupRunMode RunMode);
	
private:
	
	Talon 		  *m_pickupMotor;
	Talon 		  *m_flipMotor;
	DigitalInput  *m_diskSensor;
	AnalogChannel *m_flipPot;
	Events 		  *m_event;
	char   		   m_log[100];
};

#endif
