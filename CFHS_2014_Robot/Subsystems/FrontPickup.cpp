#include "FrontPickup.h"
#include "../Robotmap.h"
#include <math.h>
#include "../Commands/AllCommands.h"

const INT32 c_leftArmZeroOffset = 0;
const INT32 c_leftArmMaxPosition = 500;
const INT32 c_rightArmZeroOffset = 0;
const INT32 c_rightArmMaxPosition = 500;
const INT32 c_armTargetDeadband = 2;
const INT32 c_incrementValue = 10;

FrontPickup::FrontPickup(RobotLog* log) : Subsystem("FrontPickup") {
	m_leftArm = 	new Victor(MOD_FRONT_PICKUP_LEFT_ARM, PWM_FRONT_PICKUP_LEFT_ARM);
	m_rightArm = 	new Victor(MOD_FRONT_PICKUP_RIGHT_ARM, PWM_FRONT_PICKUP_RIGHT_ARM);
	
	m_leftWheels = 	new Relay(MOD_FRONT_PICKUP_LEFT_ROLLERS, RELAY_FRONT_PICKUP_LEFT_ROLLERS, Relay::kBothDirections);
	m_rightWheels = new Relay(MOD_FRONT_PICKUP_RIGHT_ROLLERS, RELAY_FRONT_PICKUP_RIGHT_ROLLERS, Relay::kBothDirections);
	
	m_ballLoadedSensor = new DigitalInput(DI_FRONT_PICKUP_BALL_SENSOR);
	
	m_leftArmPot = 	new AnalogChannel(AI_FRONT_PICKUP_LEFT_ARM_POT);
	m_rightArmPot = new AnalogChannel(AI_FRONT_PICKUP_RIGHT_ARM_POT);
	
	m_leftArmPID = 	new PIDControl(0, 0, 0);
	m_leftArmPID->SetInputRange(0, 1000);
	m_leftArmPID->SetOutputRange(-1.0, 1.0);
	
	m_rightArmPID = new PIDControl(0, 0, 0);
	m_rightArmPID->SetInputRange(0, 1000);
	m_rightArmPID->SetOutputRange(-1.0, 1.0);
	
	m_leftArmTarget = GetPosition(pLeft);
	m_rightArmTarget = GetPosition(pRight);
	
	m_rightArmPID->SetSetpoint(m_rightArmTarget);
	m_leftArmPID->SetSetpoint(m_leftArmTarget);
	
	m_useJoystickLeft = false;
	m_useJoystickRight = false;
	m_joyLeft = 0;
	m_joyRight = 0;
	
	m_log = log;
}

void FrontPickup::InitDefaultCommand() {
}

INT32 FrontPickup::GetPosition(Pot pot) {
	switch(pot) {
		case pLeft:
			return (m_leftArmPot->GetAverageValue() - c_leftArmZeroOffset);
		case pRight:
			return (m_rightArmPot->GetAverageValue() - c_rightArmZeroOffset);
	}
	return 0;
}

INT32 FrontPickup::LimitValue(Pot arm, INT32 position) {
	if(arm == pLeft) {
		position = (position > c_leftArmMaxPosition) ? c_leftArmMaxPosition :
				   (position < c_leftArmZeroOffset)  ? c_leftArmZeroOffset  : 
													   position;
	} else {
		position = (position > c_rightArmMaxPosition) ? c_rightArmMaxPosition :
				   (position < c_rightArmZeroOffset)  ? c_rightArmZeroOffset  :
														position;
	}
	
	return position;
}

void FrontPickup::Periodic() { // need to add support for rollers!
	static float leftSpeed = 0;
	static float rightSpeed = 0;
	
	static int ballTimerCount = 0;
	
	bool isTooHigh;
	bool isTooLow;
	
	INT32 curLeftPosition = GetPosition(pLeft);
	INT32 curRightPosition = GetPosition(pRight);
	
	leftSpeed = m_leftArmPID->Calculate((float) curLeftPosition);
	rightSpeed = m_rightArmPID->Calculate((float) curRightPosition);
	
	if(m_useJoystickLeft) {
		m_leftArmTarget = curLeftPosition;
		m_leftArmPID->SetSetpoint(m_leftArmTarget);
		
		isTooHigh = curLeftPosition > (c_leftArmMaxPosition - c_armTargetDeadband);
		isTooLow = curLeftPosition < (c_leftArmZeroOffset + c_armTargetDeadband);
		
		if(((m_joyLeft > 0) && isTooHigh) || ((m_joyLeft < 0) && isTooLow)) {
			leftSpeed = 0;
		} else if((m_joyLeft*leftSpeed < 0) || (m_joyLeft > leftSpeed)) {
			leftSpeed = m_joyLeft;
		}
	} else {
		if(abs(curLeftPosition - m_leftArmTarget) <= c_armTargetDeadband) {
			m_leftOnTarget = true;
		}
	}
	
	if(m_useJoystickRight) {
		m_rightArmTarget = curRightPosition;
		m_rightArmPID->SetSetpoint(m_rightArmTarget);
		
		isTooHigh = curRightPosition > (c_rightArmMaxPosition - c_armTargetDeadband);
		isTooLow = curRightPosition < (c_rightArmZeroOffset + c_armTargetDeadband);
		
		if(((m_joyRight > 0) && isTooHigh) || ((m_joyRight < 0) && isTooLow)) {
			rightSpeed = 0;
		} else if((m_joyRight*rightSpeed < 0) || (m_joyRight > rightSpeed)) {
			rightSpeed = m_joyRight;
		}
	} else {
		if(abs(curRightPosition - m_rightArmTarget) <= c_armTargetDeadband) {
			m_rightOnTarget = true;
		}
	}
	
	m_leftArm->Set(leftSpeed);
	m_rightArm->Set(rightSpeed);
	
	switch (m_frontMode) {
	case fDeployBoth:
		if (! m_ballLoadedSensor->Get()) {
			SetPickupMode(fStore);
		}
		break;
	case fDeployLeft:
		
		break;
	case fDeployRight:
		
		break;
	case fStore:
		
		break;
	case fPass:
		
		break;
	case fLowShoot:
		if (m_ballLoadedSensor->Get()) {
			m_leftWheels->Set(Relay::kOff);
			m_rightWheels->Set(Relay::kOff);
			SetPickupMode(fStore);
		}
		break;
	case fLowDeploy:
		
		break;
	case fMoveToLoad:
		if(m_leftOnTarget && m_rightOnTarget) {
			m_leftWheels->Set(Relay::kForward);
			m_rightWheels->Set(Relay::kForward);
			m_frontMode = fLoad;
		}
		break;
	case fLoad:
		if(m_ballLoadedSensor->Get()) {
			if(ballTimerCount > 25) {
				SetPickupMode(fStore);
			} else {
				ballTimerCount++;
			}
		} else {
			ballTimerCount = 0;
		}
		break;
	case fAutoDeploy:
		if(!m_ballLoadedSensor->Get()) {
			m_leftWheels->Set(Relay::kOff);
			m_rightWheels->Set(Relay::kOff);
		}
		break;
	}
}

void FrontPickup::IncrementArm(Pot arm, bool up) {
	if(arm == pLeft) {
		m_leftArmTarget += (up) ? c_incrementValue : -c_incrementValue;
		m_leftArmTarget = LimitValue(pLeft, m_leftArmTarget);
		m_leftArmPID->SetSetpoint(m_leftArmTarget);
	} else {
		m_rightArmTarget += (up) ? c_incrementValue : -c_incrementValue;
		m_rightArmTarget = LimitValue(pRight, m_rightArmTarget);
		m_rightArmPID->SetSetpoint(m_rightArmTarget);
	}
}

void FrontPickup::SetSetpoints(INT32 leftPosition, INT32 rightPosition){
	
	m_leftOnTarget = false;
	m_rightOnTarget = false;
	
	m_leftArmTarget = LimitValue(pLeft, leftPosition);
	m_rightArmTarget = LimitValue(pRight, rightPosition);
	
	m_leftArmPID->SetSetpoint(m_leftArmTarget);
	m_rightArmPID->SetSetpoint(m_rightArmTarget);
}

void FrontPickup::SetSetpoint(INT32 position, Pot arm) {
	if(arm == pLeft) {
		m_leftArmTarget = LimitValue(arm, position);
		m_leftArmPID->SetSetpoint(m_leftArmTarget);
	} else {
		m_rightArmTarget = LimitValue(arm, position);
		m_rightArmPID->SetSetpoint(m_rightArmTarget);
	}
}

void FrontPickup::SetUseJoystickLeft(bool use) {
	m_useJoystickLeft = use;
}

void FrontPickup::SetUseJoystickRight(bool use) {
	m_useJoystickRight = use;
}

void FrontPickup::SetJoystickLeft(float joyLeft) {
	m_joyLeft = joyLeft;
}

void FrontPickup::SetJoystickRight(float joyRight) {
	m_joyRight = joyRight;
}

void FrontPickup::SetPickupMode(FrontMode mode) {
	switch (mode) {
	case fDeployBoth:
		if (m_ballLoadedSensor->Get()) {				// No ball
			SetSetpoints(0,0);
			m_rightWheels->Set(Relay::kForward);
			m_leftWheels->Set(Relay::kForward);
			m_frontMode = fDeployBoth;
		}
		break;
	case fDeployLeft:
		
		break;
	case fDeployRight:
		
		break;
	case fStore:
		SetSetpoints(50,50);
		m_rightWheels->Set(Relay::kOff);
		m_leftWheels->Set(Relay::kOff);
		m_frontMode = fStore;
		break;
	case fPass:
		
		break;
	case fLowShoot:
		m_rightWheels->Set(Relay::kForward);
		m_leftWheels->Set(Relay::kForward);
		break;
	case fLowDeploy:
		if (!m_ballLoadedSensor->Get()) {				// Ball loaded
			SetSetpoints(0,0);
			m_frontMode = fLowDeploy;
		}
		break;
	case fMoveToLoad:
		if(!m_ballLoadedSensor->Get()) {
			SetSetpoints(80,80);
			m_frontMode = fMoveToLoad;
		}
		break;
	case fLoad:
		break;
	case fAutoDeploy:
		if(m_ballLoadedSensor->Get()) {
			SetSetpoints(0,0);
			m_rightWheels->Set(Relay::kForward);
			m_leftWheels->Set(Relay::kForward);
			m_frontMode = fAutoDeploy;
		}
		break;
	}
}

FrontPickup::FrontMode FrontPickup::GetFrontPickupMode() {
	return m_frontMode;
}

bool FrontPickup::HasBall() {
	return !m_ballLoadedSensor->Get();
}
