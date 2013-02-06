#ifndef DISKSHOOTER_H_
#define DISKSHOOTER_H_

#include "AnalogChannel.h"
#include "DigitalInput.h"
#include "Events.h"
#include "Victor.h"
#include "PIDLoop.h"
#include "Relay.h"

class DiskShooter{
	
public:
	DiskShooter(UINT8   shootMotorModule,	UINT32 shootMotorChannel,
				UINT8   tiltMotorModule,	UINT32 tiltMotorChannel,
				UINT8	tensionMotorModule, UINT32 tensionMotorChannel,
				UINT8   shootPotModule,		UINT32 shootPotChannel,
				UINT8   tiltPotModule,		UINT32 tiltPotChannel,
				UINT8   tensionPotModule,	UINT32 tensionPotChannel,
				UINT8   diskSensorModule,	UINT32 diskSensorChannel,
				Events *eventHandler,		UINT8  eventSourceId);
	
	~DiskShooter();

	void  Enable();
	void  Disable();
	INT32 GetTiltTarget();
	void  Load();
	int   Periodic(float joyValue);
	void  SetTensionTarget(INT32 Target);
	void  SetTiltTarget(INT32 Target);
	void  FIREINTHEHOLE();
	
private:
	typedef enum{sIdle, sLoad, sShootReady, sShoot}ShootState;

	DigitalInput  *m_diskSensor;
	Events		  *m_event;
	UINT8		   m_eventSourceId;
	char		   m_log[100];
	INT32		   m_RELEASETHEFRISBEEPOSITION;
	Victor 		  *m_shootMotor;
	PIDLoop		  *m_shootPID;
	AnalogChannel *m_shootPot;
	INT32		   m_shootReadyPosition;
	ShootState 	   m_shootState;
	Relay		  *m_tensionMotor;
	INT32		   m_tensionTarget;
	AnalogChannel *m_tensionPot;
	Victor 		  *m_tiltMotor;
	PIDLoop		  *m_tiltPID;
	AnalogChannel *m_tiltPot;
	INT32		   m_tiltTarget;
	
	INT32 		   GetShooterPosition();
	INT32 		   GetTiltPosition();
};

#endif
