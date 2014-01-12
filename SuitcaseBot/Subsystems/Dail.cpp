#include "Dial.h"
#include <math.h>
#include "../Robotmap.h"
#include "../Commands/AllCommands.h"

float const c_deadband 		= 1.5;
float const c_zeroOffset 	= -24.0;		// Increasing offset moves dial CW

Dial::Dial(RobotMap* robotMap) : Subsystem("Meter") {					// Create required objects and initialize variables
	m_robotLog = robotMap->robotLog;									
	
	m_dialMotor = new Victor(robotMap->pwmMap.meterMotor);
	
	m_dialPID = new PIDControl(0.025, 0.004, 0);
	m_dialPID->SetInputRange(-360, 360);
	m_dialPID->SetOutputRange(-0.6, 0.6);
	m_dialPID->SetSetpoint(0);
	
	m_dialPot = new AnalogChannel(robotMap->aiMap.meterPot);
	m_dialPot->SetAverageBits(2);
	m_dialPot->SetOversampleBits(0);
	
	m_dialMode = mPresets;
	m_targetAngle = GetAngle();
}

Dial::~Dial() {
	delete m_dialMotor;
	delete m_dialPID;
	delete m_dialPot;
}
    
void Dial::InitDefaultCommand() {
}

void Dial::DriveMotor(float value) {
	m_dialMotor->Set(value);
}

char* Dial::GetDialModeName() {											// Return Name of current Dial mode
	switch (m_dialMode) {
		case mPresets:	return "Presets";
		case mGyro:		return "Gyro   ";
		default:		return "Unknown";
	}
}

bool Dial::OnTarget() {
	return m_onTarget;
}

void Dial::Periodic() {													// Run every 20ms in Auto/Teleop Periodics
	float error;
	
	switch(m_dialMode) {												// Get cuurent error based on mode
		case mPresets:
			error = GetError(m_targetAngle);
			break;
		case mGyro:
			error = GetError(-CommandBase::drive->GetGyroAngle());
			break;
	}

	if (fabs(error) < c_deadband) {										// If less than deadband, turn motor off
		m_dialMotor->Set(0.0);
		m_onTarget = true;
	} else {															// Calculate motor output using PID
		m_dialMotor->Set(m_dialPID->Calculate(error));
		m_onTarget = false;
	}
}

void Dial::SetTarget(float target) {									// Set dial target in Presets mode
	if (m_dialMode == mPresets) {
		m_targetAngle = target;
		sprintf(m_log, "Dial Target=%f", target);
		m_robotLog->LogWrite(m_log);
	}
}

void Dial::SwitchDialMode() {
	switch(m_dialMode) {												// Switch between Dial modes
		case mPresets:
			m_dialMode = mGyro;
			break;
		case mGyro:
			m_dialMode = mPresets;
			break;
	}
	
	sprintf(m_log, "Dial Mode=%s", GetDialModeName());
	m_robotLog->LogWrite(m_log);
}

float Dial::GetAngle() {												// Return current 0 to 360 angle of dial
	static float	angle = 0;
	static INT32	lastPot = m_dialPot->GetAverageValue();
	static char 	potRollover = 0;									// Continuous Pot Rollover Flag
	
	INT32 newPot =   m_dialPot->GetAverageValue();						// Current Pot Value
	float motorOut = m_dialMotor->Get();								// For CW: motorOut > 0   For CCW: motorOut < 0
	
	if (potRollover == 1) {												// Pot Rollover from 970 to 0  (CCW rotation)
		if ((newPot - lastPot >= 0 && newPot < 20) || motorOut > 0) {	// Increasing Pot values or change in motor direction
			potRollover = 0;											// Clear Rollover flag
		}
	} else if (potRollover == 2) {										// Pot Rollover from 0 to 970  (CW rotation)
		if ((lastPot - newPot >= 0 && newPot > 950) || motorOut < 0) {	// Decreasing Pot values or change in motor direction
			potRollover = 0;											// Clear Rollover flag
		}
	} else if (lastPot > 950 && lastPot - newPot > 5 && motorOut < 0) { // Near rollover, pot values change from increaing to decreasing 	
		potRollover = 1;												// Rollover from 970 to 0 detected
		
	} else if (lastPot < 20 && newPot - lastPot > 20 && motorOut > 0) { // Near rollover, pot values change from decreasing to increasing
		potRollover = 2;												// Rollover from 0 to 970 detected
	}
	
	lastPot = newPot;													// Keep current pot value	

	if (potRollover == 0) angle = 360.0 - (float)newPot / 2.7714 - c_zeroOffset;	// Calculate new angle

	if (angle > 360) angle -= 360;										// Keep angle within 0 to 360 range
	if (angle < 0)   angle += 360;
	
	return angle; 
}

float Dial::GetError(float targetAngle) {								// Calculate error between target and current angles
	float error = (float)(((int)targetAngle * 10) % 3600) / 10;			// Verify target is within 0 to 360 range
	if (error < 0) error += 360;										

	error -= GetAngle();												// Subtract current angle
	
	if (error > 180)  error -= 360;										// Determine smallest angle to target
	if (error < -180) error += 360;
	
	return -error;														// Negate result to drive dial in the right direction
}

