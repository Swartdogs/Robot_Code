#ifndef DRIVE_H
#define DRIVE_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"

/**
 *
 *
 * @author Srinu
 */
class Drive: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	
	Victor*       m_leftFront;
	Victor* 	  m_leftCenter;
	Victor*	   	  m_leftRear;
	Victor*	   	  m_rightFront;
	Victor*	   	  m_rightCenter;
	Victor*	   	  m_rightRear;
	Gyro*         m_gyro;
	Encoder*      m_leftEncoder;
	Encoder*      m_rightEncoder;
	PIDControl*   m_drivePID;
	PIDControl*	  m_rotatePID;
	RobotLog*     m_robotLog;
	DigitalInput* m_tapeSensor;
	
	char		  m_log[100];
	bool          m_onTarget;

	bool          m_useGyro;
	float         m_targetAngle;
	float         m_relativeZero;

	double        m_targetDistance;
	double        m_lastDistance;
	float		  m_driveSpeed;
	float         m_maxSpeed;
	bool		  m_rampDone;
	bool          m_brakeApplied;
	
	double EncoderDistance(double val1, double val2);
	bool   RampSpeed(float& curSpeed, float pidSpeed);
	float  GetGyroAngle();
	float  RotateError();
	
public:
	Drive(RobotLog *robotLog);
	
	void InitDefaultCommand();
	bool OnTarget();
	void InitDistance(double targetDistance, float maxSpeed, bool resetEncoders);
	void InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, float absoluteAngle);
	void InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, float relativeAngle, bool setRelativeZero);
	void InitRotate(float absoluteAngle);
	void InitRotate(float relativeAngle, bool setRelativeZero);
	void DriveArcade(float move, float rotate);
	void ExecuteDistance();
	void ExecuteRotate();
	void Drive::SetPID(float kP, float kI, float kD, bool drive);
	void SetSafetyEnabled(bool enabled);
	void StopEncoders();
	void StopMotors();
	bool CrossedTape();
};

#endif
