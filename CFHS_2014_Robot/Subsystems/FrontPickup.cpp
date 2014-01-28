#include "FrontPickup.h"
#include "../Robotmap.h"
#include <math.h>
#include "../Commands/AllCommands.h"

const INT32 c_leftArmZeroOffset = 0;
const INT32 c_leftArmMaxPosition = 500;
const INT32 c_rightArmZeroOffset = 0;
const INT32 c_rightArmMaxPosition = 500;
const int   c_armTargetDeadband = 2;

FrontPickup::FrontPickup() : Subsystem("FrontPickup") {
	m_leftArm = 	new Victor(MOD_FRONT_PICKUP_LEFT_ARM, PWM_FRONT_PICKUP_LEFT_ARM);
	m_rightArm = 	new Victor(MOD_FRONT_PICKUP_RIGHT_ARM, PWM_FRONT_PICKUP_RIGHT_ARM);
	m_leftWheels = 	new Relay(MOD_FRONT_PICKUP_LEFT_ROLLERS, PWM_FRONT_PICKUP_LEFT_ROLLERS);
	m_rightWheels = new Relay(MOD_FRONT_PICKUP_RIGHT_ROLLERS, PWM_FRONT_PICKUP_RIGHT_ROLLERS);
	
	m_leftArmPot = 	new AnalogChannel(AI_FRONT_PICKUP_LEFT_ARM_POT);
	m_rightArmPot = new AnalogChannel(AI_FRONT_PICKUP_RIGHT_ARM_POT);
	
	m_leftArmPID = 	new PIDControl(0, 0, 0);
	m_leftArmPID->SetInputRange(0, 1000);
	m_leftArmPID->SetOutputRange(-1.0, 1.0);
	
	m_rightArmPID = new PIDControl(0, 0, 0);
	m_rightArmPID->SetInputRange(0, 1000);
	m_rightArmPID->SetOutputRange(-1.0, 1.0);
	
	m_leftArmTarget = GetPosition(m_leftArmPot) - c_leftArmZeroOffset;
	m_rightArmTarget = GetPosition(m_rightArmPot) - c_rightArmZeroOffset;
	
	m_useJoystickLeft = false;
	m_useJoystickRight = false;

}
    
void FrontPickup::InitDefaultCommand() {
	SetDefaultCommand(new FrontPickupRun());
}


void FrontPickup::Periodic(float joyLeft, float joyRight) { // need to add support for rollers!
	static float leftSpeed = 0;
	static float rightSpeed = 0;
	
	bool isTooHigh;
	bool isTooLow;
	
	INT32 curLeftPosition = GetPosition(m_leftArmPot);
	INT32 curRightPosition = GetPosition(m_rightArmPot);
	
	if(m_useJoystickLeft) {
		m_leftArmTarget = curLeftPosition;
		
		isTooHigh = curLeftPosition > (c_leftArmMaxPosition - c_armTargetDeadband);
		isTooLow = curLeftPosition < (c_leftArmZeroOffset + c_armTargetDeadband);
		
		if((joyLeft > 0) && isTooHigh) {
			leftSpeed = 0;
		} else if((joyLeft < 0) && isTooLow) {
			leftSpeed = 0;
		} else {
			leftSpeed = joyLeft;
		}
	} else {
		m_leftArmPID->SetSetpoint(m_leftArmTarget);
		
		leftSpeed = m_leftArmPID->Calculate((float) curLeftPosition);
		
		if(abs(curLeftPosition - m_leftArmTarget) <= c_armTargetDeadband) {
			leftSpeed = 0;
		}
	}
	
	if(m_useJoystickRight) {
		m_rightArmTarget = curRightPosition;
		
		isTooHigh = curRightPosition > (c_rightArmMaxPosition - c_armTargetDeadband);
		isTooLow = curRightPosition < (c_rightArmZeroOffset + c_armTargetDeadband);
		
		if((joyRight > 0) && isTooHigh) {
			rightSpeed = 0;
		} else if((joyRight < 0) && isTooLow) {
			rightSpeed = 0;
		} else {
			rightSpeed = joyRight;
		}
	} else {
		m_rightArmPID->SetSetpoint(m_rightArmTarget);
		
		rightSpeed = m_rightArmPID->Calculate((float) curRightPosition);
		
		if(abs(curRightPosition - m_rightArmTarget) <= c_armTargetDeadband) {
			rightSpeed = 0;
		}
	}
	
	m_leftArm->Set(leftSpeed);
	m_rightArm->Set(rightSpeed);
}

void FrontPickup::Run(){
	float leftSpeed;
	float rightSpeed;
	int   curLeftPosition = m_leftArmPot->GetValue();
	int   curRightPosition = m_rightArmPot->GetValue();
	
	m_leftArmPID->SetSetpoint(m_leftArmTarget);
	m_rightArmPID->SetSetpoint(m_rightArmTarget);
	
	leftSpeed = m_leftArmPID->Calculate(curLeftPosition);
	rightSpeed = m_rightArmPID->Calculate(curRightPosition);
	
	m_leftArm->Set(leftSpeed);
	m_rightArm->Set(rightSpeed);
}

void FrontPickup::MoveArmsWithJoystick(float leftArmPower, float rightArmPower){
	if(m_leftArmPot->GetValue() <= c_leftArmMaxPosition && m_leftArmPot->GetValue() >= c_leftArmZeroOffset){
		m_leftArm->Set(leftArmPower);
		m_leftArmTarget = m_leftArmPot->GetValue();
	}
	if(m_rightArmPot->GetValue() <= c_rightArmMaxPosition && m_rightArmPot->GetValue() >= c_rightArmZeroOffset){
		m_rightArm->Set(rightArmPower);
		m_rightArmTarget = m_rightArmPot->GetValue();
	}
}

void FrontPickup::SetArmsToPosition(int leftPosition, int rightPosition){
	int curLeftPosition = m_leftArmPot->GetValue();
	int curRightPosition = m_rightArmPot->GetValue();
	float leftSpeed;
	float rightSpeed;
	
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
	
	leftSpeed = m_leftArmPID->Calculate(curLeftPosition);
	rightSpeed = m_rightArmPID->Calculate(curRightPosition);
	
	m_leftArm->Set(leftSpeed);
	m_rightArm->Set(rightSpeed);
	
	if(abs(curLeftPosition - m_leftLastPosition) < c_armTargetDeadband) m_leftOnTarget = true;
	if(abs(curRightPosition - m_rightLastPosition) < c_armTargetDeadband) m_rightOnTarget = true;
	
	m_leftLastPosition = curLeftPosition;
	m_rightLastPosition = curRightPosition;
}

void FrontPickup::RunLeftWheels(Relay::Value value){
	m_leftWheels->Set(value);
}

void FrontPickup::RunRightWheels(Relay::Value value){
	m_rightWheels->Set(value);
}

INT32 FrontPickup::GetPosition(AnalogChannel* pot) {
	INT32 curReading = 0;
	static INT32 lastReading = 0;
	INT32 returnValue = 0;
	
	curReading = pot->GetAverageValue();
	returnValue = curReading;
	
	if((curReading < lastReading) || (abs(curReading - lastReading) >= 200)) {
		returnValue = lastReading;
	}	
	lastReading = curReading;
	
	return returnValue;
}

void FrontPickup::SetUseJoystickLeft(bool use) {
	m_useJoystickLeft = use;
}

void FrontPickup::SetUseJoystickRight(bool use) {
	m_useJoystickRight = use;
}
