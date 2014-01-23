#include "FrontPickup.h"
#include "../Robotmap.h"
#include <math.h>
#include "../Commands/FrontPickupRun.h"

const INT32 c_leftArmZeroOffset = 0;
const INT32 c_leftArmMaxPosition = 500;
const INT32 c_rightArmZeroOffset = 0;
const INT32 c_rightArmMaxPosition = 500;
const int   c_armTargetDeadband = 2;

FrontPickup::FrontPickup() : Subsystem("FrontPickup") {
	m_leftArm = new Victor(MOD_FRONT_PICKUP_LEFT_ARM, PWM_FRONT_PICKUP_LEFT_ARM);
	m_rightArm = new Victor(MOD_FRONT_PICKUP_RIGHT_ARM, PWM_FRONT_PICKUP_RIGHT_ARM);
	m_leftWheels = new Relay(MOD_FRONT_PICKUP_LEFT_ROLLERS, PWM_FRONT_PICKUP_LEFT_ROLLERS);
	m_rightWheels = new Relay(MOD_FRONT_PICKUP_RIGHT_ROLLERS, PWM_FRONT_PICKUP_RIGHT_ROLLERS);
	
	m_leftArmPot = new AnalogChannel(AI_FRONT_PICKUP_LEFT_ARM_POT);
	m_rightArmPot = new AnalogChannel(AI_FRONT_PICKUP_RIGHT_ARM_POT);
	
	m_leftArmPID = new PIDControl(0, 0, 0);
	m_leftArmPID->SetInputRange(0, 1000);
	m_leftArmPID->SetOutputRange(-1.0, 1.0);
	
	m_rightArmPID = new PIDControl(0, 0, 0);
	m_rightArmPID->SetInputRange(0, 1000);
	m_rightArmPID->SetOutputRange(-1.0, 1.0);
}
    
void FrontPickup::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
	SetDefaultCommand(new FrontPickupRun());
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

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
