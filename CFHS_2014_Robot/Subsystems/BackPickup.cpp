#include "BackPickup.h"
#include "../Robotmap.h"

const INT32 c_baseMotorDeadband = 1;
const INT32 c_baseZeroOffset = 0;
const INT32 c_baseMaxPosition = 1000;

BackPickup::BackPickup(RobotLog* log) : Subsystem("BackPickup") {
	m_baseMotor = new Victor(MOD_BACK_PICKUP_BASE_MOTOR, PWM_BACK_PICKUP_BASE_MOTOR);
	m_rollers = new Victor(MOD_BACK_PICKUP_ROLLERS, PWM_BACK_PICKUP_ROLLERS);
	
	m_baseMotorPot = new AnalogChannel(AI_BACK_PICKUP_BASE_POT);
	
	m_baseMotorPID = new PIDControl(0, 0, 0);
	m_baseMotorPID->SetInputRange(0, 1024);
	m_baseMotorPID->SetOutputRange(-1.0, 1.0);
	
	m_log = log;
	
	m_useJoystick = false;
	m_joySpeed = 0;
	
	m_baseTarget = GetPosition();
	
	m_baseMotorPID->SetSetpoint(m_baseTarget);
	
	m_onTarget = true;
}

INT32 BackPickup::GetPosition() {
	return (m_baseMotorPot->GetAverageValue() - c_baseZeroOffset);
}
    
void BackPickup::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

void BackPickup::Periodic(){
	float motorPower;
	bool  isTooHigh;
	bool  isTooLow;
	
	INT32 curPosition = GetPosition();
	isTooHigh = curPosition > (c_baseMaxPosition - c_baseMotorDeadband);
	isTooLow = curPosition < (c_baseZeroOffset + c_baseMotorDeadband);
	
	motorPower = m_baseMotorPID->Calculate(curPosition);
	
	if(m_useJoystick) {
		m_baseTarget = curPosition;
		m_baseMotorPID->SetSetpoint(m_baseTarget);
		
		if(((m_joySpeed > 0) && isTooHigh) || ((m_joySpeed < 0) && isTooLow)) {
			motorPower = 0;
		} else if((m_joySpeed*motorPower < 0) || (m_joySpeed > motorPower)){
			motorPower = m_joySpeed;
		}
	} else {
		if(abs(curPosition - m_baseTarget) <= c_baseMotorDeadband) m_onTarget = true;
	}
	
	m_baseMotor->Set(motorPower);
}

void BackPickup::SetSetpoint(INT32 target){
	
	target = (target > c_baseMaxPosition) ? c_baseMaxPosition:
			 (target < c_baseZeroOffset)  ? c_baseZeroOffset:
										    target;
	
	m_baseTarget = target;
	m_onTarget = false;
	m_baseMotorPID->SetSetpoint(target);
}

void BackPickup::SetRollers(float power){
	m_rollers->Set(power);
}

bool BackPickup::OnTarget(){
	return m_onTarget;
}

void BackPickup::SetUseJoystick(bool use) {
	m_useJoystick = use;
}

void BackPickup::SetJoystickSpeed(float speed) {
	m_joySpeed = speed;
}
