#ifndef DRIVE_H
#define DRIVE_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"
#include "../RobotMap.h"

class Drive: public Subsystem {
public:
	typedef enum {dTank, dArcade, dMecanuum}DriveMode;
    typedef enum {dAll, dRear, dFront}DriveWhich;
	
	Drive(RobotMap* robotMap);
	~Drive();
	
	void InitDefaultCommand();
	
	void    ChangePowerLevel();
	void 	DriveArcade(float drive, float rotate);
	void    DriveMecanuum(float drive, float strafe, float rotate, DriveWhich which);
	void    DriveTank(float left, float right);
	void    EndGyroUse();
	void    ExecuteDistance();
	void    ExecuteRotate();
	void    ExecuteStopSensor();
	float   GetGyroAngle();
	int		GetDriveMode();
	char*   GetDriveModeInfo();
	void	GyroStrafe(float drive, float strafe);
	void    InitDistance(double targetDistance, float maxSpeed, bool resetEncoders);
	void    InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle, bool resetGyro);
	void    InitGyroStrafe();
	void	InitRotate(float targetAngle, bool resetGyro);
	void    InitStopSensor(float maxSpeed);
	void    MecanuumDrive(float drive, float strafe, float rotate, DriveWhich whichWheels);
	bool    OnTarget();
	void    ResetGyro();
	void	SetPID(float kP, float kI, float kD);
	void    SetSafetyEnabled(bool enabled);
	void    StopEncoders();
	void    StopMotors();
	void    SwitchDriveMode();
	
private:
	typedef enum {dForward, dReverse}DriveDirection;
	
	PIDControl*		m_drivePID;
	Encoder*		m_encoderL;
	Encoder* 		m_encoderR;
	Victor*			m_motorLF;
	Victor*			m_motorLR;
	Victor*			m_motorRF;
	Victor* 		m_motorRR;
	RobotDrive*		m_robotDrive;
	RobotLog*		m_robotLog;
	Gyro*			m_rotateGyro;
	PIDControl*		m_rotatePID;
	DigitalInput*	m_stopSensorFront;
	DigitalInput*	m_stopSensorRear;
	
	bool			m_applyBrake;
	DriveDirection  m_driveDirection;
	float      	 	m_driveSpeed;
	DriveMode		m_driveMode;
	double			m_lastDistance;
	char      	  	m_log[100];
	float			m_maxSpeed;
	bool			m_onTarget;
	float           m_powerLevel;
	bool    		m_rampDone;
	float			m_targetAngle;
	double			m_targetDistance;
	bool        	m_useGyro;
	
	double	EncoderAverage(double value1, double value2);
    float	Limit(float Value);
	bool	RampSpeed(float* curSpeed, float pidSpeed);
};

#endif
