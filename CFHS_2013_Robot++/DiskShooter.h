#ifndef DISKSHOOTER_H_
#define DISKSHOOTER_H_

#include "AnalogChannel.h"
#include "DigitalInput.h"
#include "Events.h"
#include "Jaguar.h"

class Events;

class DiskShooter{
public:
	
	DiskShooter(UINT8   shootMotorModule,	UINT32 shootMotorChannel,
				UINT8   tiltMotorModule,	UINT32 tiltMotorChannel,
				UINT8   shootPotModule,		UINT32 shootPotChannel,
				UINT8   tiltPotModule,		UINT32 tiltPotChannel,
				UINT8   diskSensorModule,	UINT32 diskSensorChannel,
				INT32   shootIdlePosition,	
				INT32   tiltZeroOffset,
				Events *eventHandler,		UINT8  eventSourceId);
	
	~DiskShooter();
	

	void  Enable();
	void  Disable();
	INT32 GetTiltPosition();
	void  Load();
	bool  Periodic();
	void  Shoot();
	void  SetTiltTarget(INT32 Target);
	
private:
	typedef enum{sIdle, sLoad, sShootReady, sShoot}ShootState;
	
	Jaguar 		  *m_shootMotor;
	Jaguar 		  *m_tiltMotor;
	AnalogChannel *m_shootPot;
	AnalogChannel *m_tiltPot;
	DigitalInput  *m_diskSensor;
	Events		  *m_event;
	char		   m_Log[100];
	UINT8		   m_eventSourceId;
	INT32		   m_shootIdlePosition;
	INT32		   m_shootReadyPosition;
	INT32		   m_RELEASETHEFRISBEEPOSITION;
	INT32		   m_tiltZeroOffset;
	INT32		   m_tiltTarget;
	ShootState 	   m_shootState;
	
	INT32 		   GetShooterPotValue();
};

#endif
