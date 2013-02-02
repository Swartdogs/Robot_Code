#ifndef HOPPER_H_
#define HOPPER_H_

#include "Jaguar.h"
#include "DigitalInput.h"
#include "Servo.h"
#include "AnalogChannel.h"
#include "Events.h"

class Hopper{
public:
	typedef enum{hFeeder, hDrive, hPyramid}HopTarget;
	 Hopper(UINT8 	shootGateModule,  UINT32 shootGateChannel,
			UINT8 	loadGateModule,   UINT32 loadGateChannel,
			UINT8 	tiltMotorModule,  UINT32 tiltMotorChannel,
			UINT8 	tiltPotModule,	  UINT32 tiltPotChannel,
			UINT8 	diskSensorModule, UINT32 diskSensorChannel,
			Events *eventHandler,	  UINT8  eventSourceId);
	~Hopper();
	
	void Disable();
	void Enable();
	void PELICANMOVE(bool pelicanStateEnabled);
	void Periodic(float joyValue);
	void RELEASETHEFRISBEES();
	void SetTiltTarget(HopTarget Target);
	
private:
	typedef enum{hLoad, hStore, hShoot}HopState;
	
	Servo 		  *m_shootGate;
	Servo 		  *m_loadGate;
	Jaguar 		  *m_tiltMotor;
	AnalogChannel *m_tiltPot;
	DigitalInput  *m_diskSensor;
	Events		  *m_event;
	char		   m_log[100];
	UINT8		   m_eventSourceId;
	INT32		   m_tiltTarget;
	HopState	   m_hopState;
	bool		   m_pelicanStateEnabled;
};

#endif
