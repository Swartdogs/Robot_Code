#ifndef HOPPER_H_
#define HOPPER_H_

#include "Victor.h"
#include "DigitalInput.h"
#include "Relay.h"
#include "AnalogChannel.h"
#include "PIDLoop.h"
#include "Events.h"

class Hopper{
public:
	 Hopper(UINT8 	hopperGateModule,   UINT32 hopperGateChannel,
			UINT8 	tiltMotorModule,    UINT32 tiltMotorChannel,
			UINT8 	tiltPotModule,	    UINT32 tiltPotChannel,
			UINT8 	beforeSensorModule, UINT32 beforeSensorChannel,
			UINT8   afterSensorModule,  UINT32 afterSensorChannel,
			Events *eventHandler,	    UINT8  eventSourceId);
	~Hopper();
	
	void  Disable();
	void  Enable();
	void  PELICANMOVE(bool pelicanStateEnabled);
	int   Periodic(float joyValue);
	void  RELEASETHEFRISBEE();
	void  SetTiltTarget(INT32 Target);

private:
	typedef enum{hEmpty, hLoad, hStore, hShoot}HopState;
	
	Relay		  *m_hopperGate; 		   
	Victor 		  *m_tiltMotor;
	AnalogChannel *m_tiltPot;
	DigitalInput  *m_beforeSensor;
	DigitalInput  *m_afterSensor;
	Events		  *m_event;
	PIDLoop       *m_tiltPID;
	
	char		   m_log[100];
	UINT8		   m_eventSourceId;
	INT32		   m_tiltTarget;
	HopState	   m_hopState;
	bool		   m_pelicanStateEnabled;
	bool           m_newTiltTarget;
};

#endif
