//************Team 525 (2013)************//
//
// Pickup Mechanism Code

#ifndef DISKPICKUP_H_
#define DISKPICKUP_H_

#include "Victor.h"
#include "DigitalInput.h"
#include "AnalogChannel.h"
#include "Relay.h"
#include "PIDLoop.h"
#include "Events.h"

class DiskPickup{	

public:
	
typedef enum {pStore, pLoad, pDeployed, pUnderPyramid}PickupRunMode;

	DiskPickup(
			UINT8	pickupMotorModule,  UINT32 pickupMotorChannel,
			UINT8   armMotorModule,	    UINT32 armMotorChannel,
			UINT8   wristMotorModule,	UINT32 wristMotorChannel,
			UINT8   diskSensorModule,	UINT32 diskSensorChannel,
			UINT8   armPotModule,		UINT32 armPotChannel,
			UINT8   wristPotModule,		UINT32 wristPotChannel,
			Events *eventHandler,		UINT8  eventSourceId);
	
	~DiskPickup();
	void  			 Enable();
	void  	 		 Disable();
	void  			 FeedSafety();
	void  			 Periodic(PickupRunMode RunMode);
	
private:
	
	Relay		    *m_pickupMotor;
	Victor 		    *m_armMotor;
	Victor 		    *m_wristMotor;
	DigitalInput    *m_diskSensor;
	AnalogChannel   *m_armPot;
	AnalogChannel   *m_wristPot;
	PickupRunMode    m_runMode;
	PIDLoop			*m_armPID;
	PIDLoop			*m_wristPID;
	INT32			 m_armTiltTarget;
	INT32			 m_wristTiltTarget;
	Events 		    *m_event;
	char   		     m_log[100];
};

#endif
