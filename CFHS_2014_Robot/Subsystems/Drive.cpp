#include "Drive.h"
#include "../Robotmap.h"
#include "../Commands/DriveWithJoystick.h"
#include <math.h>

Drive::Drive() : Subsystem("Drive") {
	m_leftFront    = new Victor(PWM_DRIVE_LEFT_FRONT);
	m_leftCenter   = new Victor(PWM_DRIVE_LEFT_CENTER);
	m_leftRear     = new Victor(PWM_DRIVE_LEFT_REAR);
	m_rightFront   = new Victor(PWM_DRIVE_RIGHT_FRONT); 
	m_rightCenter  = new Victor(PWM_DRIVE_RIGHT_CENTER);
	m_rightRear    = new Victor(PWM_DRIVE_RIGHT_REAR);
	
	m_gyro         = new Gyro(AI_GYRO);
	
	m_leftEncoder  = new Encoder(DI_DRIVE_LEFT_A, DI_DRIVE_LEFT_B);
	m_rightEncoder = new Encoder(DI_DRIVE_RIGHT_A, DI_DRIVE_RIGHT_B);
	
	m_onTarget = false;
	m_useGyro  = false;
	m_targetDistance = 0;
	m_lastDistance = 0;
	m_targetAngle = 0;
	m_maxSpeed = 0;
	m_relativeZero = 0;
	
	m_leftFront->SetExpiration(1.0);
	m_leftCenter->SetExpiration(1.0);
	m_leftRear->SetExpiration(1.0);
	m_rightFront->SetExpiration(1.0);
	m_rightCenter->SetExpiration(1.0);
	m_rightRear->SetExpiration(1.0);
	
	m_drivePID = new PIDControl(0.025,0,0);
	m_drivePID->SetInputRange(-200,200);
	m_drivePID->SetOutputRange(-0.6,0.6);
	
	m_rotatePID = new PIDControl(0.05, 0.005, 0.2);
	m_rotatePID->SetInputRange(-360.0,-360.0);
	m_rotatePID->SetOutputRange(-0.7,0.7);
	m_rotatePID->SetSetpoint(0);
}
    
void Drive::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
	
	SetDefaultCommand(new DriveWithJoystick());
}

bool Drive::OnTarget() {
	return m_onTarget;
}

void Drive::InitDistance(double targetDistance, float maxSpeed, bool resetEncoders) {
	if(resetEncoders) {
		m_leftEncoder->Reset();
		m_rightEncoder->Reset();
	}
	
	m_targetDistance = targetDistance;
	m_maxSpeed = maxSpeed;
	
	m_leftEncoder->Start();
	m_rightEncoder->Start();
	
	m_drivePID->Reset();
	m_drivePID->SetSetpoint(m_targetDistance);
	m_drivePID->SetOutputRange(-m_maxSpeed, m_maxSpeed);
	m_drivePID->SetPID(0.025, 0, 0);
	
	m_onTarget = m_useGyro = false;
	m_brakeApplied = false;
}

void Drive::InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle) {
	InitDistance(targetDistance, maxSpeed, resetEncoders);
	InitRotate(targetAngle);
	m_useGyro = true;
}

void Drive::InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle, bool resetGyro) {
	InitDistance(targetDistance, maxSpeed, resetEncoders);
	InitRotate(targetAngle, resetGyro);
	m_useGyro = true;
}

void Drive::InitRotate(float targetAngle, bool resetGyro) {
	if(resetGyro) {
		m_relativeZero = GetAngle();
	}
	InitRotate(targetAngle + m_relativeZero);
}

void Drive::InitRotate(float targetAngle) {
	m_targetAngle = ((int)((targetAngle)*10) % 3600) / 10.0;
	m_onTarget = false;
}

void Drive::DriveArcade(float move, float rotate) {
	float pwm[2];
	float maxValue;
	
	pwm[0] = move + rotate;
	pwm[1] = maxValue = move - rotate;
	
	if(fabs(pwm[0]) > fabs(maxValue)) {
		maxValue = fabs(pwm[0]);
	}
	
	if(fabs(maxValue) > 1.0) {
		pwm[0] /= maxValue;
		pwm[1] /= maxValue;
	}
	
	m_leftFront->Set(pwm[0]);
	m_leftCenter->Set(pwm[0]);
	m_leftRear->Set(pwm[0]);
	m_rightFront->Set(-pwm[1]);
	m_rightCenter->Set(-pwm[1]);
	m_rightRear->Set(-pwm[1]);
}

void Drive::ExecuteDistance() {
	
	const float brake_threshold = 0.15f;
	
	float moveSpeed = 0;
	float rotateSpeed = 0;
	float curDistance = EncoderAverage(m_leftEncoder->Get(), m_rightEncoder->Get());
	
	if (!m_brakeApplied) {
		// Brake is not applied.
		// We check to see if it needs to be.
		
		if (fabs(m_targetDistance) - fabs(curDistance) <= fabs(m_targetDistance) * brake_threshold) {
			m_brakeApplied = true;
			m_drivePID->SetPID(0.025f,0.0f,0.3f);
		}
		
	}
	
	moveSpeed = m_drivePID->Calculate(m_targetDistance);		
	rotateSpeed = m_rotatePID->Calculate(m_targetAngle);
	
	if(m_useGyro) {
		DriveArcade(moveSpeed, rotateSpeed);
	} else {
		DriveArcade(moveSpeed, 0);
	}
	
	if(curDistance - m_lastDistance <= 0) {
		m_onTarget = true;
	}
	
	m_lastDistance = curDistance;
}

void Drive::ExecuteRotate() {
	
	static int counter = 5;

	float distance = (m_targetAngle - GetAngle());
	distance = (distance > 180) ? (distance - 360):
			   (distance < 180) ? (distance + 360):
								   distance;
		
	float rotateSpeed = m_rotatePID->Calculate(distance);
	DriveArcade(0, rotateSpeed);
	if(fabs(distance) <= 1) {
		if(counter > 0) {
			counter--;
		} else {
			m_onTarget = true;
			counter = 5;
		}
	}
}

void Drive::SetPID(float kP, float kI, float kD, bool drive) {
	if(drive) {
		m_drivePID->SetPID(kP, kI, kD);
	} else {
		m_rotatePID->SetPID(kP, kI, kD);
	}
}

void Drive::SetSafetyEnabled(bool enabled) {
	m_leftFront->SetSafetyEnabled(enabled);
	m_leftCenter->SetSafetyEnabled(enabled);
	m_leftRear->SetSafetyEnabled(enabled);
	m_rightFront->SetSafetyEnabled(enabled);
	m_rightCenter->SetSafetyEnabled(enabled);
	m_rightRear->SetSafetyEnabled(enabled);
	
}

void Drive::StopEncoders() {
	m_leftEncoder->Stop();
	m_rightEncoder->Stop();
}

void Drive::StopMotors() {
	m_leftFront->Set(0);
	m_leftCenter->Set(0);
	m_leftRear->Set(0);
	m_rightFront->Set(0);
	m_rightCenter->Set(0);
	m_rightRear->Set(0);
}

// PRIVATE 

double EncoderAverage(double val1, double val2) {
	if(fabs(val1) < fabs(val2/2)) return val2;
	if(fabs(val2) < fabs(val1/2)) return val1;
								  return (val1+val2)/2;
}

bool RampSpeed(float& curSpeed, float pidSpeed) {
	float direction;
	float speed;
	bool vReturn = false;
	
	if (pidSpeed < 0) {
		direction = -1.0;
	} else {
		direction = 1.0;
	}
	
	pidSpeed = fabs(pidSpeed);
	speed = fabs(curSpeed);
	
	if(speed == 0.0) {
		if (pidSpeed <= 0.2) {
			speed = pidSpeed;
			vReturn = true;
		} else {
			speed = speed + 0.03;
			
			if (speed >= pidSpeed) {
				speed = pidSpeed;
				vReturn = true;
			}
		}
	}
		
	pidSpeed = speed * direction;
				
	return vReturn;
}

float Drive::GetAngle() {
	float angle = ((int)(m_gyro->GetAngle() * 10) % 3600) / 10.0;
	if(angle < 0) angle += 360.0;
	return angle;
}
