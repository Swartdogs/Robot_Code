// ********** Team 525 (2013) **********
//
// Drive Class Header File

#ifndef DRIVE_H_
#define DRIVE_H_

#include <math.h>
#include "Victor.h"
#include "Encoder.h"
#include "Gyro.h"
#include "PIDLoop.h"
#include "SmartDashboard\SmartDashboard.h"
#include "Events.h"

class Drive
{
	
public:
	
	typedef enum {dStop, dCoast, dJoystick, dStrafe, dAim, dAutoDrive, dAutoRotate} DriveRunMode;
	
	Drive(
			UINT8 lfJagModule,		UINT32 lfJagChannel,		//Left Front Jag
			UINT8 lrJagModule,		UINT32 lrJagChannel,		//Left Rear Jag
			UINT8 rfJagModule,		UINT32 rfJagChannel,		//Right Front Jag
			UINT8 rrJagModule,		UINT32 rrJagChannel,		//Right Rear Jag
			UINT8 laEncoderModule,  UINT32 laEncoderChannel,	//Left Encoder A
			UINT8 lbEncoderModule,  UINT32 lbEncoderChannel,	//Left Encoder B
			UINT8 raEncoderModule,  UINT32 raEncoderChannel,	//Right Encoder A
			UINT8 rbEncoderModule,  UINT32 rbEncoderChannel,	//Right Encoder B
			UINT8 rotateGyroModule, UINT32 rotateGyroChannel,	//Rotational Gyro
			Events *eventHandler,	UINT8  eventSourceId);
    ~Drive();
    
    void	Disable();
    void	Enable();
    float	GetAngle();
    double	GetDistance();
    void	ResetDriveSpeed();
    void	ResetEncoders();
    void	ResetGyro();
    bool	Periodic(DriveRunMode RunMode, float JoyDrive, float JoyStrafe, float JoyRotate);
    void	SetAngle(float Angle);
    void	SetDistance(double Distance);
    void	SetMaxSpeed(float Speed);
    
private:
    enum EnumDrive {driveAll, driveRear, driveFront};
    
    Victor  	  *m_motorLF;
    Victor        *m_motorLR;
    Victor        *m_motorRF;
    Victor  	  *m_motorRR;
    Encoder 	  *m_lEncoder;
    Encoder		  *m_rEncoder;
    Gyro		  *m_rotateGyro;
    float		   m_driveSpeed;
    float		   m_maxSpeed;
    float		   m_rotateDeadband;
    float	       m_targetAngle;
    double	       m_targetDistance;
    PIDLoop 	  *m_rotatePID;
    Events  	  *m_Event;
    char		   m_log[100];
    
    float	ApplyDeadband(float RawValue, float Deadband);
    double	EncoderAverage(double Value1, double Value2);
    float	Limit(float Value);
    float	Maximum(float Value1, float Value2);
    void	MecanumDrive(float Drive, float Strafe, float Rotate, EnumDrive DriveMode);
    bool	SameSignAndGreater(float Value1, float Value2);
    float   RampSpeed(float Speed, float MaxSpeed);
};

#endif
