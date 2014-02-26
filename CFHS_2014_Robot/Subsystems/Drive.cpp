#include "Drive.h"
#include "../Robotmap.h"
#include "../Commands/AllCommands.h"
#include <math.h>

Drive::Drive(RobotLog *robotLog) : Subsystem("Drive") {
	m_robotLog = robotLog;
	
	m_leftFront        = new Victor(PWM_DRIVE_LEFT_FRONT);
	m_leftCenterRear   = new Victor(PWM_DRIVE_LEFT_CENTER_REAR);
	m_rightFront       = new Victor(PWM_DRIVE_RIGHT_FRONT); 
	m_rightCenterRear  = new Victor(PWM_DRIVE_RIGHT_CENTER_REAR);
	
	m_gyro = new Gyro(AI_GYRO);
	m_gyro->SetSensitivity(0.007);
	
	m_leftEncoder = new Encoder(DI_DRIVE_LEFT_A, DI_DRIVE_LEFT_B, true);
	m_leftEncoder->SetDistancePerPulse(0.036);
	
	m_rightEncoder = new Encoder(DI_DRIVE_RIGHT_A, DI_DRIVE_RIGHT_B);
	m_rightEncoder->SetDistancePerPulse(0.036);
	
	m_tapeSensor = new DigitalInput(DI_DRIVE_TAPE_SENSOR);
	m_tapeDetectLED = new Solenoid(SOLENOID_TAPE_DETECT);
	
	m_rangefinder = new AnalogChannel(AI_RANGEFINDER);
	m_rangefinder->SetAverageBits(2);
	m_rangefinder->SetOversampleBits(0);
	
	m_tapeMode = tOff;
	m_rangeMode = rOff;
	
	m_onTarget = false;
	
	m_useGyro  = false;
	m_targetAngle = 0;
	m_relativeZero = 0;
	
	m_targetDistance = 0;
	m_lastDistance = 0;
	m_driveSpeed = 0;
	m_maxSpeed = 0;
	m_rampDone = false;
	m_brakeApplied = false;
	
	m_leftFront->SetExpiration(1.0);
	m_leftCenterRear->SetExpiration(1.0);
	m_rightFront->SetExpiration(1.0);
	m_rightCenterRear->SetExpiration(1.0);
	
	m_drivePID = new PIDControl(0.025,0,0);
	m_drivePID->SetInputRange(-200,200);
	m_drivePID->SetOutputRange(-0.6,0.6);
	
	m_rotatePID = new PIDControl(0.05, 0.005, 0.4);
	m_rotatePID->SetInputRange(-360.0,360.0);
	m_rotatePID->SetOutputRange(-0.7,0.7);
	m_rotatePID->SetSetpoint(0);
	
	m_leftEncoder->Start();
	m_rightEncoder->Start();
	
	m_startCollection = false;
	m_tapeDetected = false;
	
//	m_tapeDetectLED->Set(true);
}

bool Drive::CrossedTape() {
	return (!m_tapeSensor->Get());
}

void Drive::DriveArcade(float moveValue, float rotateValue) {
//	float pwm[2];
//	float maxValue;
//	
//	pwm[0] = moveValue - rotateValue;
//	pwm[1] = maxValue = moveValue + rotateValue;
//	
//	if(fabs(pwm[0]) > fabs(maxValue)) {
//		maxValue = fabs(pwm[0]);
//	}
//	
//	if(fabs(maxValue) > 1.0) {
//		pwm[0] /= maxValue;
//		pwm[1] /= maxValue;
//	}
//	
//	m_leftFront->Set(-pwm[0]);
//	m_leftCenterRear->Set(-pwm[0]);
//	m_rightFront->Set(pwm[1]);
//	m_rightCenterRear->Set(pwm[1]);
	
	float leftMotorOutput;
	float rightMotorOutput;
	
	if (moveValue > 0.0) {
		if (rotateValue > 0.0) {
			leftMotorOutput = moveValue - rotateValue;
			rightMotorOutput = max(moveValue, rotateValue);
		} else {
			leftMotorOutput = max(moveValue, -rotateValue);
			rightMotorOutput = moveValue + rotateValue;
		}
	} else {
		if (rotateValue > 0.0) {
			leftMotorOutput = - max(-moveValue, rotateValue);
			rightMotorOutput = moveValue + rotateValue;
		} else {
			leftMotorOutput = moveValue - rotateValue;
			rightMotorOutput = - max(-moveValue, -rotateValue);
		}
	}
	m_leftFront->Set(-leftMotorOutput);
	m_leftCenterRear->Set(-leftMotorOutput);
	m_rightFront->Set(rightMotorOutput);
	m_rightCenterRear->Set(rightMotorOutput);
	
	// printf("Left: %.3f, Right: %.3f\n", leftMotorOutput, rightMotorOutput);
	// printf(Left: );
}

void Drive::TapeDetectExecute(float move, float rotate) {
	switch(m_tapeMode) {
	case tOff:
		m_tapeDetected = false;
		m_tapeDetectLED->Set(false);
		DriveArcade(move, rotate);
		break;
	
	case tArm:
		if(CrossedTape() && !m_tapeDetected) {
			m_tapeDetected = true;
			InitDistance(132, 0.7, true, 24, 0);
			m_tapeMode = tDetect;
			m_tapeDetectLED->Set(true);
			
			sprintf(m_log, "Drive    Tape Detected: Angle=%5.1f", GetGyroAngle());
			m_robotLog->LogWrite(m_log);
		} 
		
		DriveArcade(move, rotate);
		break;
	
	case tDetect:
		ExecuteDistance();
		if(m_onTarget) m_tapeMode = tOff;
		break;
	default:;
	}
}

void Drive::RangeDetectExecute(float move, float rotate) {
	switch(m_rangeMode) {
		case rOn:
			if(GetRange() < 132 && GetRange() > 60) {
				m_tapeDetectLED->Set(true);
			} else {
				m_tapeDetectLED->Set(false);
			}
			break;
		case rOff:
			m_tapeDetectLED->Set(false);
			break;
		default:;
	}
	DriveArcade(move, rotate);
}

void Drive::ExecuteDistance() {
	const float brake_threshold = 0.15f;
	float curDistance = EncoderDistance(m_leftEncoder->GetDistance(), m_rightEncoder->GetDistance());
	float rotateSpeed = 0;
	
	if (m_fireDistance > 0) {
		if (curDistance >= m_fireDistance) {
			CommandBase::ballShooter->Fire();
			m_fireDistance = 0;
		}
	}
	
	if (!m_brakeApplied) {
		// Brake is not applied.
		// We check to see if it needs to be.
		
		if (fabs(m_targetDistance - curDistance) <= fabs(m_targetDistance) * brake_threshold) {
			m_brakeApplied = true;
			m_drivePID->SetPID(0.025f,0.0f,0.3f);
			sprintf(m_log, "Drive    PID Brake applied: Distance=%6.1f", curDistance);
			m_robotLog->LogWrite(m_log);
		}
	}
	
	if (m_rampDone) {
		m_driveSpeed = m_drivePID->Calculate(curDistance);		
	} else {
		m_rampDone = RampSpeed(m_driveSpeed, m_drivePID->Calculate(curDistance));
	}
	
	if (m_useGyro) rotateSpeed = m_rotatePID->Calculate(RotateError());

	curDistance = fabs(curDistance);
	
	if (curDistance > fabs(m_targetDistance / 2)) {
		if(curDistance - m_lastDistance <= 0) {
			m_driveSpeed = 0;
			
			if (!m_useGyro) {
				m_onTarget = true;
				sprintf(m_log, "Drive    Drive Done: Distance=%6.1f", curDistance);
				m_robotLog->LogWrite(m_log);

			} else if (fabs(RotateError()) <= 1.0) {
				rotateSpeed = 0;
				m_onTarget = true;
				sprintf(m_log, "Drive    Drive/Rotate Done: Distance=%6.1f  Angle=%5.1f", curDistance, GetGyroAngle());
				m_robotLog->LogWrite(m_log);
			}
		}
	}
	
	m_lastDistance = curDistance;
	DriveArcade(m_driveSpeed, rotateSpeed);
}

void Drive::ExecuteRotate() {
	static int counter = 5;
	float error = RotateError();
	float rotateSpeed = m_rotatePID->Calculate(error);

	if (fabs(error) <= 1.0) {
		if(counter > 0) {
			counter--;
		} else {
			rotateSpeed = 0;
			if (!m_onTarget) {
				m_onTarget = true;
				sprintf(m_log, "Drive    Rotate Done: Angle=%5.1f", GetGyroAngle());
				m_robotLog->LogWrite(m_log);
			}
		}
	} else {
		counter = 5;
	}
	
//	printf("RotateError: %f  RotateSpeed: %f\n", error, rotateSpeed);
	
	DriveArcade(0, rotateSpeed);
}

double Drive::GetRange() {
	return (m_rangefinder->GetAverageValue() * (12.0/58));
}

void Drive::InitDefaultCommand() {
	SetDefaultCommand(new DriveWithJoystick());
}

void Drive::InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, double fireDistance) {
	if(resetEncoders) {
		m_leftEncoder->Reset();
		m_rightEncoder->Reset();
	}
	
	m_targetDistance = targetDistance;
	m_maxSpeed = maxSpeed;
	m_fireDistance = fireDistance;
	
	m_leftEncoder->Start();
	m_rightEncoder->Start();
	
	m_drivePID->Reset();
	m_drivePID->SetSetpoint(m_targetDistance);
	m_drivePID->SetOutputRange(-m_maxSpeed, m_maxSpeed);
	m_drivePID->SetPID(0.025, 0, 0);
	
	m_driveSpeed = 0;
	m_lastDistance = 0;
	m_onTarget = m_useGyro = false;
	m_brakeApplied = m_rampDone = false;

	sprintf(m_log, "Drive    Initiate Drive: Distance=%6.1f", m_targetDistance);
	m_robotLog->LogWrite(m_log);
}

void Drive::InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, double fireDistance, float absoluteAngle) {
	InitDistance(targetDistance, maxSpeed, resetEncoders, fireDistance);
	InitRotate(absoluteAngle);
	m_useGyro = true;
}

void Drive::InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, double fireDistance, float relativeAngle, bool setRelativeZero) {
	InitDistance(targetDistance, maxSpeed, resetEncoders, fireDistance);
	InitRotate(relativeAngle, setRelativeZero);
	m_useGyro = true;
}

void Drive::InitRotate(float relativeAngle, bool setRelativeZero) {
	if(setRelativeZero) m_relativeZero = GetGyroAngle();
	InitRotate(relativeAngle + m_relativeZero);
}

void Drive::InitRotate(float absoluteAngle) {
	m_targetAngle = ((int)((absoluteAngle) * 10) % 3600) / 10.0;
	if (m_targetAngle < 0) m_targetAngle += 360;
	
	m_rotatePID->Reset();
	m_useGyro = true;
	m_onTarget = false;

	sprintf(m_log, "Drive    Initiate Rotate: Angle=%5.1f", m_targetAngle);
	m_robotLog->LogWrite(m_log);
}

bool Drive::OnTarget() {
	return m_onTarget;
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
	m_leftCenterRear->SetSafetyEnabled(enabled);
	m_rightFront->SetSafetyEnabled(enabled);
	m_rightCenterRear->SetSafetyEnabled(enabled);
	
}

void Drive::StopEncoders() {
	m_leftEncoder->Stop();
	m_rightEncoder->Stop();
}

void Drive::StopMotors() {
	m_leftFront->Set(0);
	m_leftCenterRear->Set(0);
	m_rightFront->Set(0);
	m_rightCenterRear->Set(0);
}

void Drive::EncoderTestEnabled(bool state) {
	m_startCollection = state;
}

Drive::TapeMode Drive::GetTapeMode() {
	return m_tapeMode;
}
void Drive::SetTapeMode(TapeMode mode) {
	m_tapeMode = mode;
	
	if (m_tapeMode == tOff) {
		m_tapeDetected = false;
		m_tapeDetectLED->Set(false);
		
	} else if (m_tapeMode == tArm) { 
		m_robotLog->LogWrite("Drive    Tape Detect Arm");
	}
}

Drive::RangeMode Drive::GetRangeMode() {
	return m_rangeMode;
}

void Drive::SetRangeMode(RangeMode mode) {
	m_rangeMode = mode;
	
	if (m_rangeMode == rOff)  {
		m_tapeDetectLED->Set(false);
	} else {
		m_robotLog->LogWrite("Drive    Initiate Range Mode");
	}
}

void Drive::ResetGyro() {
	sprintf(m_log, "Drive    Reset Gyro from %5.1f", GetGyroAngle());
	m_robotLog->LogWrite(m_log);
	m_gyro->Reset();
	m_gyro->GetAngle();
	m_gyro->GetAngle();
}


//  ******************** PRIVATE ********************


double Drive::EncoderDistance(double val1, double val2) {	
	// Return distance with greatest error
	if (fabs(m_targetDistance - val1) > fabs(m_targetDistance - val2)) return val1; 
	return val2;
}

float Drive::GetGyroAngle() {
	// Retun angle in 0 to 360 degree range
	float angle = ((int)(m_gyro->GetAngle() * 10) % 3600) / 10.0;
	if(angle < 0) angle += 360.0;
	return angle;
}

bool Drive::RampSpeed(float& curSpeed, float pidSpeed) {
	// Ramp curSpeed until >= pidSpeed
	float Direction;
	float Speed;
	bool  vReturn = false;
	
	if (pidSpeed < 0) {									// Determine direction
		Direction = -1.0;
	} else {
		Direction = 1.0;
	}
	
	pidSpeed = fabs(pidSpeed);							// Use absolute Speed values 
	Speed = fabs(curSpeed);								

	if (Speed == 0.0) {									// If stopped, set speed to minimum value
		if (pidSpeed <= 0.2) {
			Speed = pidSpeed;
			vReturn = true;
		} else {
			Speed = 0.2;						
		}

	} else {											// Ramp to PID Output value
		Speed = Speed + 0.05;
		
		if (Speed >= pidSpeed){
			Speed = pidSpeed;
			vReturn = true;
		}
	}
	
	curSpeed = Speed * Direction;						// Apply direction and set curSpeed

	return vReturn;
}

float Drive::RotateError() {
	// Determine RotateError in -180 to +180 range
	float error = (m_targetAngle - GetGyroAngle());
	
	error = (error > 180) ? (error - 360):
			(error < -180) ? (error + 360):
			 error;
	
	return error;
}
