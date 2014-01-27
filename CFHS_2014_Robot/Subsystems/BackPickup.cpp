#include "BackPickup.h"
#include "../Robotmap.h"
#include "../Commands/AllCommands.h"

const int c_baseMotorDeadband = 1;

BackPickup::BackPickup() : Subsystem("BackPickup") {
	m_baseMotor = new Victor(MOD_BACK_PICKUP_BASE_MOTOR, PWM_BACK_PICKUP_BASE_MOTOR);
	m_rollers = new Victor(MOD_BACK_PICKUP_ROLLERS, PWM_BACK_PICKUP_ROLLERS);
	
	m_baseMotorPot = new AnalogChannel(AI_BACK_PICKUP_BASE_POT);
	
	m_baseMotorPID = new PIDControl(0, 0, 0);
	m_baseMotorPID->SetInputRange(0, 1024);
	m_baseMotorPID->SetOutputRange(-1.0, 1.0);
}
    
void BackPickup::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
	SetDefaultCommand(new BackPickupPeriodic());
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

void BackPickup::Periodic(){
	float motorPower;
	
	float curPosition = m_baseMotorPot->GetValue();
	m_baseMotorPID->SetSetpoint(m_baseTarget);
	
	motorPower = m_baseMotorPID->Calculate(curPosition);
	
	m_baseMotor->Set(motorPower);
}

void BackPickup::SetToPosition(int target){
	float motorPower;
	int curPosition = m_baseMotorPot->GetValue();
	
	m_onTarget = false;
	
	m_baseTarget = target;
	m_baseMotorPID->SetSetpoint(m_baseTarget);
	
	motorPower = m_baseMotorPID->Calculate(curPosition);
	
	m_baseMotor->Set(motorPower);
	
	if(abs(curPosition - m_baseTarget) < c_baseMotorDeadband) m_onTarget = true;
	
	m_lastPosition = curPosition;
}

void BackPickup::SetRollers(float power){
	m_rollers->Set(power);
}

bool BackPickup::OnTarget(){
	return m_onTarget;
}
