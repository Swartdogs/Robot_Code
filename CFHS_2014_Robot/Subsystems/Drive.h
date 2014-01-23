#ifndef DRIVE_H
#define DRIVE_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"

/**
 *
 *
 * @author Srinu
 */
class Drive: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	Victor     *m_leftFront;
	Victor 	   *m_leftCenter;
	Victor	   *m_leftRear;
	Victor	   *m_rightFront;
	Victor	   *m_rightCenter;
	Victor	   *m_rightRear;
	Gyro       *m_gyro;
	Encoder    *m_leftEncoder;
	Encoder    *m_rightEncoder;
	PIDControl *m_drivePID;
	PIDControl *m_rotatePID;
	
	bool        m_onTarget;
	bool        m_useGyro;
	double      m_targetDistance;
	double      m_lastDistance;
	float       m_targetAngle;
	float       m_maxSpeed;
	float       m_relativeZero;
	
	bool        m_brakeApplied;
	
	double EncoderAverage(double val1, double val2);
	bool RampSpeed(float& curSpeed, float pidSpeed);
	float GetAngle();
	
public:
	Drive();
	void InitDefaultCommand();
	bool OnTarget();
	void InitDistance(double targetDistance, float maxSpeed, bool resetEncoders);
	void InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle);
	void InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle, bool resetGyro);
	void InitRotate(float targetAngle);
	void InitRotate(float targetAngle, bool resetGyro);
	void DriveArcade(float move, float rotate);
	void ExecuteDistance();
	void ExecuteRotate();
	void Drive::SetPID(float kP, float kI, float kD, bool drive);
	void SetSafetyEnabled(bool enabled);
	void StopEncoders();
	void StopMotors();
};

#endif
