#ifndef DISKSHOOTER_H_
#define DISKSHOOTER_H_

#include "AnalogChannel.h"
#include "DigitalInput.h"
#include "Events.h"
#include "Jaguar.h"
#include "PIDLoop.h"
#include "Relay.h"

class Events;

class DiskShooter{
public:
	
	DiskShooter(UINT8   shootMotorModule,	UINT32 shootMotorChannel,
				UINT8   tiltMotorModule,	UINT32 tiltMotorChannel,
				UINT8	tensionMotorModule, UINT32 tensionMotorChannel,
				UINT8   shootPotModule,		UINT32 shootPotChannel,
				UINT8   tiltPotModule,		UINT32 tiltPotChannel,
				UINT8   tensionPotModule,	UINT32 tensionPotChannel,
				UINT8   diskSensorModule,	UINT32 diskSensorChannel,
				INT32   shootIdlePosition,	
				INT32   tiltZeroOffset,
				INT32	tensionZeroOffset,
				Events *eventHandler,		UINT8  eventSourceId);
	
	~DiskShooter();
	

	void  Enable();
	void  Disable();
	INT32 GetTiltPosition();
	void  Load();
	bool  Periodic();
	void  SetTensionTarget(INT32 Target);
	void  SetTiltTarget(INT32 Target);
	void  Shoot();
	
private:
	typedef enum{sIdle, sLoad, sShootReady, sShoot}ShootState;
	
	DigitalInput  *m_diskSensor;
	Events		  *m_event;
	UINT8		   m_eventSourceId;
	char		   m_Log[100];
	INT32		   m_RELEASETHEFRISBEEPOSITION;
	INT32		   m_shootIdlePosition;
	Jaguar 		  *m_shootMotor;
	PIDLoop		  *m_shootPID;
	AnalogChannel *m_shootPot;
	INT32		   m_shootReadyPosition;
	ShootState 	   m_shootState;
	Relay		  *m_tensionMotor;
	INT32		   m_tensionTarget;
	AnalogChannel *m_tensionPot;
	INT32		   m_tensionZeroOffset;
	Jaguar 		  *m_tiltMotor;
	PIDLoop		  *m_tiltPID;
	AnalogChannel *m_tiltPot;
	INT32		   m_tiltTarget;
	INT32		   m_tiltZeroOffset;
	
	INT32 		   GetShooterPotValue();
};

#endif
