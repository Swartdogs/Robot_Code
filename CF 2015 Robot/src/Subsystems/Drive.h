#ifndef Drive_H
#define Drive_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"

class Drive: public Subsystem {
public:
	typedef enum {fStart, fNewMark, fMark} AngleFrom;
	typedef enum {mArcade, mMecanum} DriveMode;
	typedef enum {wAll, wRear, wFront} DriveWheels;
	typedef enum {eRight, eLeft} WhichEncoder;

	Drive();
	~Drive();
	void		DriveWithJoystick(float drive, float rotate);
	void		DriveWithJoystick(float drive, float strafe, float rotate, DriveWheels driveWheels);
	void		ExecuteDistance(bool showPID);
	void		ExecuteRotate(bool showPID);
	DriveMode	GetDriveMode();
	double		GetEncoderDistance(WhichEncoder which);
	float       GetGyroAngle();
	void 		InitDefaultCommand();
	void		InitDistance(double distance, float maxPWM, bool resetEncoders, bool useBrake, float angle, AngleFrom angleFrom);
	void		InitRotate(float angle, AngleFrom angleFrom);
	bool		IsOnTarget();
	void		SetConstant(std::string key, int32_t value);
	void        SetDrivePID();
	void  		SetRotatePID();
	void		TuneDrivePID();
	void        TuneRotatePID();
	void        StopMotors();

private:
	Encoder*	m_leftEncoder;
	Encoder*	m_rightEncoder;
	Gyro*		m_gyro;
	PIDControl*	m_drivePID;
	PIDControl*	m_rotatePID;
	Victor*		m_motorLF;
	Victor*		m_motorLR;
	Victor*		m_motorRF;
	Victor*		m_motorRR;

	bool		m_onTarget;
	bool		m_rampDone;

	char		m_log[100];

	double		m_lastDistance;
	double		m_targetDistance;

	float		m_drivePWM;
	float		m_markAngle;
	float		m_maxPWM;
	float		m_targetAngle;

	void		ArcadeDrive(float drive, float rotate);
	double		EncoderDistance(double value1, double value2);
	float		Limit(float Value);
	void 		MecanumDrive(float drive, float strafe, float rotate, DriveWheels driveWheels);
	bool		RampPWM(float& curPWM, float pidPWM);
	float       RotateError();
};

#endif
