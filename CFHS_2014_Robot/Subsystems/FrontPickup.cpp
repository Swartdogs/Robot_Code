#include "FrontPickup.h"
#include "../Robotmap.h"
#include <math.h>
#include "../Commands/AllCommands.h"

const INT32 c_leftArmZeroOffset = 0;
const INT32 c_leftArmMaxPosition = 500;
const INT32 c_rightArmZeroOffset = 0;
const INT32 c_rightArmMaxPosition = 500;
const INT32 c_armTargetDeadband = 2;

FrontPickup::FrontPickup(RobotLog* log) : Subsystem("FrontPickup") {
	m_leftArm = 	new Victor(MOD_FRONT_PICKUP_LEFT_ARM, PWM_FRONT_PICKUP_LEFT_ARM);
	m_rightArm = 	new Victor(MOD_FRONT_PICKUP_RIGHT_ARM, PWM_FRONT_PICKUP_RIGHT_ARM);
	m_leftWheels = 	new Victor(MOD_FRONT_PICKUP_LEFT_ROLLERS, PWM_FRONT_PICKUP_LEFT_ROLLERS);
	m_rightWheels = new Victor(MOD_FRONT_PICKUP_RIGHT_ROLLERS, PWM_FRONT_PICKUP_RIGHT_ROLLERS);
	
	m_rightArmSensor = new DigitalInput(DI_FRONT_PICKUP_RIGHT_SENSOR);
	
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

INT32 FrontPickup::GetPosition(Pot pot) {
	switch(pot) {
		case pLeft:
			return (m_leftArmPot->GetAverageValue() - c_leftArmZeroOffset);
		case pRight:
			return (m_rightArmPot->GetAverageValue() - c_rightArmZeroOffset);
	}
	return 0;
}
    
void FrontPickup::InitDefaultCommand() {
}


void FrontPickup::Periodic() { // need to add support for rollers!
	static float leftSpeed = 0;
	static float rightSpeed = 0;
	
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
		if (! m_rightArmSensor->Get()) {
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
		
		break;
	case fLowDeploy:
		
		break;
	}
}

void FrontPickup::SetSetpoints(INT32 leftPosition, INT32 rightPosition){
	
	m_leftOnTarget = false;
	m_rightOnTarget = false;
	
	if(leftPosition > c_leftArmMaxPosition) leftPosition = c_leftArmMaxPosition;
	if(rightPosition > c_rightArmMaxPosition) rightPosition = c_rightArmMaxPosition;
	if(leftPosition < c_leftArmZeroOffset) leftPosition = c_leftArmZeroOffset;
	if(rightPosition < c_rightArmZeroOffset) rightPosition = c_rightArmZeroOffset;
	
	m_leftArmTarget = leftPosition;
	m_rightArmTarget = rightPosition;
	
	m_leftArmPID->SetSetpoint(m_leftArmTarget);
	m_rightArmPID->SetSetpoint(m_rightArmTarget);
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
	m_frontMode = mode;
	
	switch (mode) {
	case fDeployBoth:
		SetSetpoints(0,0);
		m_rightWheels->Set(1.0);
		m_leftWheels->Set(1.0);
		break;
	case fDeployLeft:
		
		break;
	case fDeployRight:
		
		break;
	case fStore:
		SetSetpoints(50,50);
		m_rightWheels->Set(0.0);
		m_leftWheels->Set(0.0);
		break;
	case fPass:
		
		break;
	case fLowShoot:
		
		break;
	case fLowDeploy:
		SetSetpoints(0,0);
		break;
	}
}

FrontPickup::FrontMode FrontPickup::GetFrontPickupMode() {
	return m_frontMode;
}
