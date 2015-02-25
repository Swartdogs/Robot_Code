#ifndef Drive_H
#define Drive_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../Robotmap.h"

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
	void		InitDistance(double distance, float maxPWM, bool resetEncoders, bool useBrake, float angle, AngleFrom angleFrom,
			                 double triggerDistance);
	void		InitRotate(float angle, AngleFrom angleFrom, float maxPWM);
	void 		InitStrafeDrive();
	bool		IsOnTarget();
	bool		IsAtTrigger();
	void        ResetEncoders();
	void		SetConstant(std::string key, int32_t value);
	void        SetDrivePID(float dThreshold);
	void		SetMarkAngle();
	void  		SetRotatePID();
	void		TuneDrivePID();
	void        TuneRotatePID();
	void        StopMotors();
	void		StrafeDrive(float drive, float strafe);

private:
	Encoder*	m_leftEncoder;
	Encoder*	m_rightEncoder;
	Gyro*		m_gyro;
	PIDControl*	m_drivePID;
	PIDControl*	m_rotatePID;

	#if (MY_ROBOT == 0)							// Schumacher
		VictorSP*		m_motorLF;
		VictorSP*		m_motorLR;
		VictorSP*		m_motorRF;
		VictorSP*		m_motorRR;
	#else										// Otis
		Victor*		m_motorLF;
		Victor*		m_motorLR;
		Victor*		m_motorRF;
		Victor*		m_motorRR;
	#endif

	bool		m_atTrigger;
	bool		m_onTarget;
	bool		m_rampDone;

	char		m_log[100];

	double		m_lastDistance;
	double		m_targetDistance;
	double		m_triggerDistance;

	float		m_drivePWM;
	float		m_markAngle;
	float		m_maxPWM;
	float		m_targetAngle;

	void		ArcadeDrive(float drive, float rotate);
	double		EncoderDistance(double value1, double value2);
	float		Limit(float Value);
	void 		MecanumDrive(float drive, float strafe, float rotate, DriveWheels driveWheels);
	bool		RampPWM(float& curPWM, float pidPWM);
	float       RotateError(float target);
};

#endif
