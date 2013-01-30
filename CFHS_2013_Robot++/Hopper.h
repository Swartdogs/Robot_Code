#ifndef HOPPER_H_
#define HOPPER_H_

#include "Jaguar.h"
#include "DigitalInput.h"
#include "Servo.h"
#include "AnalogChannel.h"
#include "Events.h"

class Events;

class Hopper{
public:
	 Hopper(UINT8 	shootGateModule,  UINT32 shootGateChannel,
			UINT8 	hopperGateModule, UINT32 hopperGateChannel,
			UINT8 	hopperTiltModule, UINT32 hopperTiltChannel,
			UINT8 	tiltPotModule,	  UINT32 tiltPotChannel,
			UINT8   storagePosition,
			float	shootGateClosed,  float  shootGateOpen,
			float   hopGateClosed,    float  hopGateOpen,
			UINT8 	diskSensorModule, UINT32 diskSensorChannel,
			Events *eventHandler,	  UINT8  eventSourceId);
	~Hopper();
	
	void Disable();
	void Enable();
	void PELICANMOVE(bool pelicanStateEnabled);
	void Periodic();
	void RELEASETHEFRISBEES();
	void SetHopperTiltTarget(INT32 Target);
	
private:
	typedef enum{hShoot, hStore, hLoad}HopState;
	
	Servo 		  *m_shootGate;
	Servo 		  *m_hopperGate;
	Jaguar 		  *m_hopperTiltMotor;
	AnalogChannel *m_hopperTiltPot;
	DigitalInput  *m_diskSensor;
	Events		  *m_event;
	char		   m_Log[100];
	UINT8		   m_eventSourceId;
	UINT8		   m_storagePosition;
	INT32		   m_tiltTarget;
	HopState	   m_hopState;
	float		   m_shootGateClosedPos;
	float		   m_shootGateOpenPos;
	float          m_hopGateClosedPos;
	float		   m_hopGateOpenPos;
	bool		   m_pelicanStateEnabled;
};

#endif
