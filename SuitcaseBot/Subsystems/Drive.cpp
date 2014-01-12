#include "Drive.h"
#include <math.h>
#include "../Commands/AllCommands.h"

float const c_rotateDeadband 	= 1.0;

Drive::Drive(RobotMap* robotMap) : Subsystem("Drive") {	// Create and Configure Encoder, Gyro, Motor Controller, and PID objects
	m_robotLog = robotMap->robotLog;
	
	m_drivePID = new PIDControl(0.025, 0, 0);
	m_drivePID->SetInputRange(-200, 200);
	m_drivePID->SetOutputRange(-0.6, 0.6);
			
	m_encoderL = new Encoder(robotMap->diMap.driveEncoderLA, robotMap->diMap.driveEncoderLB, true);
	m_encoderL->SetDistancePerPulse(0.0775);		
	
	m_encoderR = new Encoder(robotMap->diMap.driveEncoderRA, robotMap->diMap.driveEncoderRB);
	m_encoderR->SetDistancePerPulse(0.0775);
	
	m_motorLF = new Victor(robotMap->pwmMap.driveMotorLF);
	m_motorLR = new Victor(robotMap->pwmMap.driveMotorLR);
	m_motorRF = new Victor(robotMap->pwmMap.driveMotorRF);
	m_motorRR = new Victor(robotMap->pwmMap.driveMotorRR);
	
	m_motorLF->SetExpiration(1.0);
	m_motorLR->SetExpiration(1.0);
	m_motorRF->SetExpiration(1.0);
	m_motorRR->SetExpiration(1.0);
	
	m_robotDrive = new RobotDrive(m_motorLF, m_motorLR, m_motorRF, m_motorRR);
	m_robotDrive->SetSafetyEnabled(false);
	
	m_rotateGyro = new Gyro(robotMap->aiMap.driveGyro);
	m_rotateGyro->SetSensitivity(0.007);
	
	m_rotatePID = new PIDControl(0.050, 0.005, 0.20);
	m_rotatePID->SetInputRange(-360.0, 360.0);
	m_rotatePID->SetOutputRange(-0.7, 0.7);
	
	m_stopSensorFront = new DigitalInput(robotMap->diMap.stopSensorFront);
	m_stopSensorRear = new DigitalInput(robotMap->diMap.stopSensorRear);
	
	m_driveMode = dTank;
	m_driveDirection = dReverse;
	m_powerLevel = 0.5;
	m_useGyro = false;
}
    
Drive::~Drive() {										// Delete created objects
	delete m_drivePID;
	delete m_encoderL;
	delete m_encoderR;
	delete m_motorLF;
	delete m_motorLR;
	delete m_motorRF;
	delete m_motorRR;
	delete m_robotDrive;
	delete m_rotateGyro;
	delete m_rotatePID;
	delete m_stopSensorFront;
	delete m_stopSensorRear;
}

void Drive::InitDefaultCommand() {						// Default Command used when no other command is active
	SetDefaultCommand(new DriveJoystick());
}

void Drive::ChangePowerLevel() {						// Increment Power Level
	m_powerLevel += 0.1;
	if (m_powerLevel > 1.01) m_powerLevel = 0.5;
	
	sprintf(m_log, "Drive Mode=%s", GetDriveModeInfo());
	m_robotLog->LogWrite(m_log);
}

void Drive::DriveArcade(float drive, float rotate) {	// Drive robot using Arcasde mode 
	m_robotDrive->ArcadeDrive(drive * m_powerLevel, rotate * m_powerLevel, false);
}

void Drive::DriveMecanuum(float drive, float strafe, float rotate, DriveWhich which) {	// Dirve robot using Mecanuum mode
	MecanuumDrive(drive * m_powerLevel, strafe * m_powerLevel, rotate * m_powerLevel, which);
}

void Drive::DriveTank(float left, float right) {		// Drive robot using Tank mode 
	m_robotDrive->TankDrive(left * m_powerLevel, right * m_powerLevel, false);
}

void Drive::EndGyroUse() {
	m_useGyro = false;
}

void Drive::ExecuteDistance() {							// Execute Drive Distance command
	float  vRotate = 0;
	double distance = EncoderAverage(m_encoderL->GetDistance(), m_encoderR->GetDistance());

	if (!m_applyBrake) {								// Add D coefficient for braking at 85% of target distance
		if (fabs(m_targetDistance - distance) < fabs(m_targetDistance) * .15) {
			m_applyBrake = true;
			m_drivePID->SetPID(0.025, 0, 0.3);
			sprintf(m_log, "Drive    PID Brake  Distance=%6.1f", distance);
			m_robotLog->LogWrite(m_log);
		}
	}

	if (m_rampDone) {									// Ramp PWM output to peak to avoid wheel spin
		m_driveSpeed = m_drivePID->Calculate(distance);
	} else {
		m_rampDone = RampSpeed(&m_driveSpeed, m_drivePID->Calculate(distance));
	}
														// Add rotational correction
	if (m_useGyro) vRotate = m_rotatePID->Calculate(m_rotateGyro->GetAngle());

	distance = fabs(distance);
														// Drive completed when robot movement ends or reverses
	if (distance > fabs(m_targetDistance) / 2) {
		if (distance - m_lastDistance <= 0) {
			m_driveSpeed = vRotate = 0;
			
			m_onTarget = true;
			sprintf(m_log, "Drive    PID Done  Target=%6.1f  Distance=%6.1f  PWM=%5.3f", m_targetDistance, distance, m_driveSpeed);
			m_robotLog->LogWrite(m_log);
		}
	}
	
	m_lastDistance = distance;
														// Use calculated values in appropriate drive function
	switch(m_driveMode) {
		case dTank:		m_robotDrive->TankDrive(m_driveSpeed + vRotate, m_driveSpeed - vRotate, false);
						break;
		case dArcade:	m_robotDrive->ArcadeDrive(m_driveSpeed, -vRotate, false);
						break;
		case dMecanuum: MecanuumDrive(m_driveSpeed, 0, vRotate, dAll);	
						break;
	}
}

void Drive::ExecuteRotate() {							// Execute Rotation command
	static int 	onTargetCount = 0;
	float 		gyroAngle = m_rotateGyro->GetAngle();
	float 		vRotate = m_rotatePID->Calculate(gyroAngle);
	
	if (fabs(m_targetAngle - gyroAngle) <= c_rotateDeadband) {
		if (onTargetCount < 5) {
			onTargetCount++;
		} else {
			vRotate = 0;
			if (!m_onTarget) {
				m_onTarget = true;
				sprintf(m_log, "Drive    Turn Completed: Gyro=%5.1f", gyroAngle);
				m_robotLog->LogWrite(m_log);
			}
		}
	} else {
		onTargetCount = 0;
	}
	
	switch(m_driveMode) {
		case dTank:		m_robotDrive->TankDrive(vRotate, -vRotate, false);
						break;
		case dArcade:	m_robotDrive->ArcadeDrive(0, -vRotate, false);
						break;
		case dMecanuum:	MecanuumDrive(0, 0, vRotate, dAll);	
						break;
	}					
}

void Drive::ExecuteStopSensor() {						// Execute Drive with Stop Sensors command
	static int stopWait = 0;
	
	float vRotate = m_rotatePID->Calculate(m_rotateGyro->GetAngle());
	
	if (stopWait > 0) {									// Wait before changing direction
		stopWait--;
		
	} else if (m_driveDirection == dForward) {
		if (m_stopSensorFront->Get() == 0) {			// Obstacle detected
			m_driveDirection = dReverse;
			m_driveSpeed = 0;
			m_rampDone = false;
			stopWait = 20;
			
		} else if (!m_rampDone) {
			m_rampDone = RampSpeed(&m_driveSpeed, m_maxSpeed);
		}
		
	} else {
		if(m_stopSensorRear->Get() == 0) {				// Obstacle detected
			m_driveDirection = dForward;
			m_driveSpeed = 0;
			m_rampDone = false;
			stopWait = 20;

		} else if (!m_rampDone) {
			m_rampDone = RampSpeed(&m_driveSpeed, -m_maxSpeed);
		}
	}
	
	switch(m_driveMode) {								// Use calculated values in appropriate drive function
		case dTank:		m_robotDrive->TankDrive(m_driveSpeed + vRotate, m_driveSpeed - vRotate, false);
						break;
		case dArcade:	m_robotDrive->ArcadeDrive(m_driveSpeed, -vRotate, false);
						break;
		case dMecanuum: MecanuumDrive(m_driveSpeed, 0, vRotate, dAll);	
						break;
	}
}

float Drive::GetGyroAngle() {							// Get current Gyro angle
	return m_rotateGyro->GetAngle();
}

int Drive::GetDriveMode() {								// Get current Drive Mode
	return m_driveMode;
}

char* Drive::GetDriveModeInfo() {						// Get current Drive Mode Name
	static char info[20];
	
	switch (m_driveMode) {
		case dTank:		sprintf(info, "Tank (%d)     ", (int)(m_powerLevel * 100)); 
						break;
		case dArcade:	sprintf(info, "Arcade (%d)   ", (int)(m_powerLevel * 100));
						break;
		case dMecanuum:	sprintf(info, "Mecanuum (%d) ", (int)(m_powerLevel * 100));
						break;
		default:		sprintf(info, "Unknown      ");
	}
	
	return info; 
}

void Drive::GyroStrafe(float drive, float strafe) {		// Execute Gyro-Assisted Strafing
	if (m_driveMode == dMecanuum) {
		MecanuumDrive(drive, strafe, m_rotatePID->Calculate(m_rotateGyro->GetAngle()), dAll);
	} else {
		m_robotDrive->ArcadeDrive(0, 0, false);
	}
}
														// Initialize Drive Distance command
void Drive::InitDistance(double targetDistance, float maxSpeed, bool resetEncoders) {	
	m_targetDistance = targetDistance;
	m_maxSpeed = maxSpeed;

	if (resetEncoders) {
		m_encoderL->Reset();
		m_encoderR->Reset();
	}
	
	m_encoderL->Start();
	m_encoderR->Start();

	m_drivePID->Reset();
	m_drivePID->SetSetpoint(m_targetDistance);
	m_drivePID->SetOutputRange(-m_maxSpeed, m_maxSpeed);
	m_drivePID->SetPID(0.025, 0, 0);
	
	
	m_driveSpeed = 0;
	m_lastDistance = 0;
	m_applyBrake = m_onTarget = m_rampDone = m_useGyro = false;

	SetSafetyEnabled(true);
	m_robotLog->LogWrite("Initiate Drive Distance Command");
}
														// Initialize Drive Distance command with Gyro Correction
void Drive::InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle, bool resetGyro) {
	InitDistance(targetDistance, maxSpeed, resetEncoders);
	InitRotate(targetAngle, resetGyro);
	m_useGyro = true;
}

void Drive::InitGyroStrafe() {							// Initialize Gyro-Assisted Strafing command
	m_rotateGyro->Reset();
	m_rotatePID->Reset();
	m_rotatePID->SetSetpoint(0);
	m_useGyro = true;
	m_robotLog->LogWrite("Initiate Gyro Strafing");
}
														// Initialize Rotation command
void Drive::InitRotate(float targetAngle, bool resetGyro) {	
	if (resetGyro) m_rotateGyro->Reset();
	
	m_targetAngle = targetAngle;
	m_rotatePID->Reset();
	m_rotatePID->SetSetpoint(m_targetAngle);
	m_useGyro = true;
	m_onTarget = false;

	SetSafetyEnabled(true);
	m_robotLog->LogWrite("Initiate Rotate Command");
}

void Drive::InitStopSensor(float maxSpeed) {			// Initialize Stop Sensor command
	if (m_driveDirection == dForward) {
		m_driveDirection = dReverse;
	} else {
		m_driveDirection = dForward;
	}
	
	m_maxSpeed = maxSpeed;
	m_driveSpeed = 0;
	m_rampDone = false;
	m_rotateGyro->Reset();
	m_rotatePID->SetSetpoint(0);
	m_useGyro = true;
	m_robotLog->LogWrite("Initiate Stop Sensor Command");
}
														// Custom Mecanuum Drive method 
void Drive::MecanuumDrive(float drive, float strafe, float rotate, DriveWhich whichWheels){
	int i = 0;
	float MotorAbs;
	float MotorPwm[4];
	float MotorMax = 0;
	
	MotorPwm[0] = rotate + drive + strafe;				// Determine PWM value for each motor
	MotorPwm[1] = rotate + drive - strafe;
	MotorPwm[2] = rotate - drive + strafe;
	MotorPwm[3] = rotate - drive - strafe;
	
	for(i = 0; i < 4; i++){								// Find maximum PWM value
		MotorAbs = fabs(MotorPwm[i]);
		if(MotorMax < MotorAbs) MotorMax = MotorAbs;
	}

	for(i = 0; i < 4; i++){								// Limit maximum to -1.0/+1.0 and scale others accordingly
		if(MotorMax > 1.0) MotorPwm[i] = Limit(MotorPwm[i] / MotorMax);
	}
	
	switch(whichWheels){								// Set Motors to calculated PWM values
		case dAll:
			m_motorLF->Set(MotorPwm[0]);
			m_motorLR->Set(MotorPwm[1]);
			m_motorRF->Set(MotorPwm[2]);
			m_motorRR->Set(MotorPwm[3]);
			break;
		case dRear:
			m_motorLF->Set(0);
			m_motorLR->Set(MotorPwm[1]);
			m_motorRF->Set(0);
			m_motorRR->Set(MotorPwm[3]);
			break;
		case dFront:
			m_motorLF->Set(0);
			m_motorLR->Set(MotorPwm[1]);
			m_motorRF->Set(0);
			m_motorRR->Set(MotorPwm[3]);
			break;
	}

}

bool Drive::OnTarget() {								// Returns whether or not robot is at Distance or Rotation target
	return m_onTarget;
}

void Drive::ResetGyro() {								// Reset the Gyro
	if (!m_useGyro) {
		m_rotateGyro->Reset();
		m_robotLog->LogWrite("Reset Gyro");
	}
}

void Drive::SetPID(float kP, float kI, float kD) {		// Set PID coefficients
	m_rotatePID->SetPID(kP, kI, kD);
}

void Drive::SetSafetyEnabled(bool enabled) {			// Set motor safety enables
	m_motorLF->SetSafetyEnabled(enabled);
	m_motorLR->SetSafetyEnabled(enabled);
	m_motorRF->SetSafetyEnabled(enabled);
	m_motorRR->SetSafetyEnabled(enabled);
}

void Drive::StopEncoders() {							// Stop the Encoders
	m_encoderL->Stop();
	m_encoderR->Stop();
}

void Drive::StopMotors() {
	m_motorLF->Set(0);
	m_motorLR->Set(0);
	m_motorRF->Set(0);
	m_motorRR->Set(0);
}

void Drive::SwitchDriveMode() {							// Switch Drive Mode by incrementing through mode list
	switch(m_driveMode) {
		case dTank:
			m_driveMode = dArcade;
			m_rotatePID->SetPID(0.05, 0.005, 0.20);
		 	break;
		case dArcade:
			m_driveMode = dMecanuum;
			m_rotatePID->SetPID(0.025, 0.003, 0.20);
			break;
		case dMecanuum:
			m_driveMode = dTank;
			m_rotatePID->SetPID(0.05, 0.005, 0.20);
			break;
	}
	
	sprintf(m_log, "Drive Mode=%s", GetDriveModeInfo());
	m_robotLog->LogWrite(m_log);
}

// PRIVATE FUNCTIONS
														// Determine average of left and right encoder
double Drive::EncoderAverage(double value1, double value2){
	if(fabs(value1) < fabs(value2 / 2)) return value2;
	if(fabs(value2) < fabs(value1 / 2)) return value1;
										return (value1 + value2) / 2;
}
														// Limit values to -1.0 to +1.0 range
float Drive::Limit(float Value){
	if(Value > 1.0)  return 1.0;
	if(Value < -1.0) return -1.0;
					 return Value;
}
														// Ramp motors to speed to avoid wheel slip
bool Drive::RampSpeed(float *curSpeed, float maxSpeed) {
	float Direction;
	float Speed;
	bool  vReturn = false;
	
	if (maxSpeed < 0) {									// Determine direction
		Direction = -1.0;
	} else {
		Direction = 1.0;
	}
	
	maxSpeed = fabs(maxSpeed);							// Use absolute Speed values 
	Speed = fabs(*curSpeed);								

	if (Speed == 0.0) {									// If stopped, set speed to minimum value
		if (maxSpeed <= 0.2) {
			Speed = maxSpeed;
			vReturn = true;
		} else {
			Speed = 0.2;						
		}

	} else {											// Ramp to Max Speed value
		Speed = Speed + 0.03;
		
		if (Speed >= maxSpeed){
			Speed = maxSpeed;
			vReturn = true;
		}
	}
	
	*curSpeed = Speed * Direction;						// Apply direction and set curSpeed

	return vReturn;										// Return True when Ramped speed >= PID speed
}

