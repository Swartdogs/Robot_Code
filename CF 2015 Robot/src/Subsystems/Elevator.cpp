#include "Elevator.h"
#include "../Robotmap.h"
#include "../Commands/AllCommands.h"
#include <math.h>

float const c_elevatorPID_P		= 0.0;
float const c_elevatorPID_I		= 0.0;
float const c_elevatorPID_D		= 0.0;

Elevator::Elevator() : Subsystem("Elevator") {
	f_elevZeroOffset = 0;

	f_elevReadyLift = 100;
	
	m_elevPID = new PIDControl();
	SetElevPID();

	m_motor1 = new Victor(PWM_ELEVATOR_1);
	m_motor2 = new Victor(PWM_ELEVATOR_2);

	m_elevPot = new AnalogInput(AI_ELEVATOR_POT);
	m_elevPot->SetAverageBits(2);
	m_elevPot->SetOversampleBits(0);

	m_limitUp = new DigitalInput(DI_ELEVATOR_UP);
	m_limitDown = new DigitalInput(DI_ELEVATOR_DOWN);

	m_elevTarget = 0;
	m_onTarget = false;

	m_elevBrake = new Solenoid(S_ELEVATOR_BRAKE);
}

Elevator::~Elevator() {
	delete m_elevPID;
	delete m_motor1;
	delete m_motor2;
	delete m_elevPot;
	delete m_limitUp;
	delete m_limitDown;
}
int32_t Elevator::GetPosition() {
	return m_elevPot->GetAverageValue() - f_elevZeroOffset;

}

bool Elevator::GetLimitSwitch(WhichLimitSwitch which) {
	return which == lUp ? m_limitUp->Get() :
						  m_limitDown->Get();

}

void Elevator::InitDefaultCommand() {
	SetDefaultCommand(new ElevPID());

}

void Elevator::RunWithJoystick(float pwm){
	m_elevTarget = GetPosition();

	if(m_limitUp->Get() && pwm > 0) {
		pwm = 0;
	} else if(m_limitDown->Get() && pwm < 0){
		pwm = 0;
	}

	m_motor1->Set(pwm);
	m_motor2->Set(pwm);
}

void Elevator::RunWithPID(){
	float pwm = 0;

	if(!m_onTarget){
		if (abs(m_elevTarget - GetPosition()) < 5){
			m_onTarget = true;
		} else {
			pwm = m_elevPID->Calculate((float)GetPosition(),false);
			if(m_limitUp->Get() && pwm > 0) {
				pwm = 0;
			} else if(m_limitDown->Get() && pwm < 0){
				pwm = 0;
			}
		}
	}

	m_elevBrake->Set(!m_onTarget);
	m_motor1->Set(pwm);
	m_motor2->Set(pwm);
}

void Elevator::SetBrake(BrakeState state) {
	m_elevBrake->Set(state == bOff);
}

void Elevator::SetConstant(std::string key, int32_t value){
	if(key == "zeroOffset") {
		f_elevZeroOffset = value;
		sprintf(m_log, "Elevator: Set Zero Offset=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "readyLift") {
		f_elevReadyLift = value;
		sprintf(m_log, "Elevator: Set Ready Lift=%d", value);
		MyRobot::robotLog->Write(m_log);
	}
}

void Elevator::SetElevPID() {
	m_elevPID->SetCoefficient('P', 0, c_elevatorPID_P, 0);
	m_elevPID->SetCoefficient('I', 0, c_elevatorPID_I, 0);
	m_elevPID->SetCoefficient('D', 0, c_elevatorPID_D, 0);

	m_elevPID->SetInputRange(0, 1000);
	m_elevPID->SetOutputRange(-0.6, 0.6);
}

void Elevator::SetElevPosition(ElevPosition position) {
	switch(position) {
		case pReadyLiftBin:		SetSetpoint(f_elevReadyLift);	break;
		default:;
	}
}

void Elevator::SetSetpoint(int32_t target) {
	target = (target > f_elevMaxPosition) ? f_elevMaxPosition:
							  (target <0) ? 0: target;
	
	m_elevTarget = target;
	m_onTarget = false;
	m_elevPID->SetSetpoint(target);
}

void Elevator::TuneElevPID() {
	m_elevPID->SetCoefficient('P', (float)MyRobot::dashboard->GetDashValue(DV_PID_P_THRESHOLD),
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_P_ABOVE) / 1000,
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_P_BELOW) / 1000);
	m_elevPID->SetCoefficient('I', (float)MyRobot::dashboard->GetDashValue(DV_PID_I_THRESHOLD),
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_I_ABOVE) / 1000,
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_I_BELOW) / 1000);
	m_elevPID->SetCoefficient('D', (float)MyRobot::dashboard->GetDashValue(DV_PID_D_THRESHOLD),
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_D_ABOVE) / 1000,
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_D_BELOW) / 1000);

	float maxPWM = (float)MyRobot::dashboard->GetDashValue(DV_PID_MAX_PWM) / 100;
	m_elevPID->SetOutputRange(-maxPWM, maxPWM);

	SetSetpoint(MyRobot::dashboard->GetDashValue(DV_PID_SETPOINT));
}



