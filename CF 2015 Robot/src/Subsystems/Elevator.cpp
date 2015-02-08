#include "Elevator.h"
#include "../Robotmap.h"
#include "../Commands/AllCommands.h"
#include <math.h>

Elevator::Elevator() : Subsystem("Elevator") {
	f_elevZeroOffset 	= 1080;
	f_elevReadyLift 	= 1500;
	f_elevMaxPosition	= 2650;
	
	m_elevPID = new PIDControl();

	#if (MY_ROBOT == 0)													// Otis
		m_motor1 = new VictorSP(PWM_ELEVATOR_1);
		m_motor2 = new VictorSP(PWM_ELEVATOR_2);
	#else																// Schumacher
		m_motor1 = new Victor(PWM_ELEVATOR_1);
		m_motor2 = new Victor(PWM_ELEVATOR_2);
	#endif

	m_elevPot = new AnalogInput(AI_ELEVATOR_POT);
	m_elevPot->SetAverageBits(2);
	m_elevPot->SetOversampleBits(0);

	m_elevDirection = dDown;
	m_elevPWM = 0;
	m_elevTarget = GetPosition();
	m_onTarget = true;
	m_rampDone = false;

	m_elevBrake = new Solenoid(S_ELEVATOR_BRAKE);
}

Elevator::~Elevator() {
	delete m_elevPID;
	delete m_motor1;
	delete m_motor2;
	delete m_elevPot;
}
int32_t Elevator::GetPosition() {
	return m_elevPot->GetAverageValue() - f_elevZeroOffset;

}

void Elevator::InitDefaultCommand() {
	SetDefaultCommand(new ElevPID());
}

void Elevator::RunWithJoystick(float joyPWM){
	m_elevTarget = GetPosition();
	m_onTarget = true;

//	printf("Pwm=%f\n", joyPWM);

	if (joyPWM < 0) {													// Move Elevator Down
		if (m_elevTarget <= 60) {										// Within 1" of bottom
			SetBrake(bOn);												// Stop
			joyPWM = 0;
		} else {
			SetBrake(bOff);
			if (m_elevTarget < 360) joyPWM *= 0.4;						// Slow down within 6" of bottom
		}

	} else if (joyPWM > 0) {											// Move Elevator Up
		if (m_elevTarget >= (f_elevMaxPosition - 60)) {					// Within 1" of top
			SetBrake(bOn);												// Stop
			joyPWM = 0;
		} else {
			SetBrake(bOff);
			if (m_elevTarget > (f_elevMaxPosition - 360)) joyPWM *= 0.6;	// Slow down within 6" of top
		}
	}

	m_motor1->Set(-joyPWM);												// Joystick PWM > 0 moves Elevator UP
	m_motor2->Set(joyPWM);
}

void Elevator::RunWithPID(bool showPID){
	static int 	counter = 0;

	if(!m_onTarget){
		if (m_elevDirection != dUp) {									// Elevator moving down
			if (m_elevTarget - GetPosition() > 0) {						// Target above current position (Overshot target)
				if (m_elevDirection != dDownTooFar) {					// Set I coefficient high (Faster upward movement)
					m_elevDirection = dDownTooFar;
					m_elevPID->SetCoefficient('I', 200, 0, 0.01);
				}
			} else if (m_elevDirection != dDown) {
				m_elevDirection = dDown;								// Set I coefficient low (Force downward movement if required)
				m_elevPID->SetCoefficient('I', 100, 0, 0.0005);
			}
		}

		if (m_rampDone) {												// Ramp of PWM completed
			m_elevPWM = m_elevPID->Calculate((float)GetPosition(), showPID);
		} else {														// Ramp PWM to calculated PID value to minimize sudden movement
			m_rampDone = RampPWM(m_elevPWM, m_elevPID->Calculate((float)GetPosition(), showPID));
		}

		if (abs(m_elevTarget - GetPosition()) < 15) {					// Elevator within 0.25 inch of Target
			counter++;
			if (counter > 2) {											// Maintain position at least 60 ms
				printf("Elevator on Target\n");							// Elevator on target
				m_onTarget = true;
				m_elevPWM = 0;
			}
		} else {														// Reset counter if not in deadband
			counter = 0;
		}

	} else {															// Set PWM = 0 if on target
		m_elevPWM = 0;
	}

	m_elevBrake->Set(m_onTarget);
	m_motor1->Set(-m_elevPWM);											// m_elevPWM > 0 moves Elevator UP
	m_motor2->Set(m_elevPWM);
}

void Elevator::SetBrake(BrakeState state) {								// Turn Brake On and Off
	m_elevBrake->Set(state == bOn);
}

void Elevator::SetConstant(std::string key, int32_t value) {			// Set constant values read from INI file
	if(key == "zeroOffset") {
		f_elevZeroOffset = value;
		sprintf(m_log, "Elevator: Set Zero Offset=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "maxPosition") {
		f_elevMaxPosition = value;
		sprintf(m_log, "Elevator: Set Max Position=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "readyLift") {
		f_elevReadyLift = value;
		sprintf(m_log, "Elevator: Set Ready Lift=%d", value);
		MyRobot::robotLog->Write(m_log);
	}
}

void Elevator::SetElevPID(ElevDirection direction) {					// Set PID coefficients based on direction of movement
	printf("Set Elev PID=%d\n", direction);

	m_elevDirection = direction;
	m_elevPID->SetInputRange(0, f_elevMaxPosition);

	switch (m_elevDirection) {
		case dUp:														// Elevator moving UP
			m_elevPID->SetCoefficient('P', 0, .003, 0);
			m_elevPID->SetCoefficient('I', 200, 0, 0.0005);				// Add I when near target to increase PWM
			m_elevPID->SetCoefficient('D', 100, 0, 0.02);				// Add D when near target to brake if moving too fast
			m_elevPID->SetOutputRange(-0.4, 1.0);
			break;

		case dDown:														// Elevator moving DOWN and above target
			m_elevPID->SetCoefficient('P', 0, .002, 0);
			m_elevPID->SetCoefficient('I', 100, 0, 0.0005);				// Add I when near target to increase PWM
			m_elevPID->SetCoefficient('D', 0, 0.02, 0);					// Use D to brake if moving too fast
			m_elevPID->SetOutputRange(-0.6, 1.0);
			break;

		case dDownTooFar:												// Elevator moving DOWN but overshot target
			m_elevPID->SetCoefficient('P', 0, .002, 0);
			m_elevPID->SetCoefficient('I', 200, 0, 0.01);				// Add large I when near target to force upward return to target
			m_elevPID->SetCoefficient('D', 0, 0.02, 0);
			m_elevPID->SetOutputRange(-0.6, 1.0);
			break;

		default:;
	}
}

void Elevator::SetElevPosition(ElevPosition position) {					// Determine PID setpoint for new position
	switch(position) {
		case pReadyLiftBin:		SetSetpoint(f_elevReadyLift);	break;
		default:;
	}
}

void Elevator::SetSetpoint(int32_t target) {							// Set PID Setpoint
	target = (target > f_elevMaxPosition) ? f_elevMaxPosition:
							  (target <0) ? 0: target;
	
	m_elevTarget = target;
	m_onTarget = false;
	m_rampDone = (target < GetPosition());
	m_elevPWM = 0;
																		// Set PID coefficients based on direction of travel
	if (m_elevTarget > GetPosition()) {									// Moving Up
		SetElevPID(dUp);
	} else {															// Moving Down
		SetElevPID(dDown);
	}

	m_elevPID->SetSetpoint((float)target);
	m_elevPID->Reset();
}

void Elevator::TuneElevPID() {
//	m_elevPID->SetCoefficient('P', (float)MyRobot::dashboard->GetDashValue(DV_PID_P_THRESHOLD),
//								   (float)MyRobot::dashboard->GetDashValue(DV_PID_P_ABOVE) / 10000,
//								   (float)MyRobot::dashboard->GetDashValue(DV_PID_P_BELOW) / 10000);
//	m_elevPID->SetCoefficient('I', (float)MyRobot::dashboard->GetDashValue(DV_PID_I_THRESHOLD),
//								   (float)MyRobot::dashboard->GetDashValue(DV_PID_I_ABOVE) / 10000,
//								   (float)MyRobot::dashboard->GetDashValue(DV_PID_I_BELOW) / 10000);
//	m_elevPID->SetCoefficient('D', (float)MyRobot::dashboard->GetDashValue(DV_PID_D_THRESHOLD),
//								   (float)MyRobot::dashboard->GetDashValue(DV_PID_D_ABOVE) / 10000,
//								   (float)MyRobot::dashboard->GetDashValue(DV_PID_D_BELOW) / 10000);

	SetSetpoint(MyRobot::dashboard->GetDashValue(DV_PID_SETPOINT));

	float maxPWM = (float)MyRobot::dashboard->GetDashValue(DV_PID_MAX_PWM) / 100;
//	m_elevPID->SetOutputRange(-maxPWM, maxPWM);

	printf("Tune Elev PID: MaxPWM=%5.2f  Setpoint=%d\n", maxPWM, m_elevTarget);
}

// ******************** PRIVATE ********************

bool Elevator::RampPWM(float& curPWM, float pidPWM) {
	float 	myPWM;
	bool	vReturn = false;

	float direction = (pidPWM < 0 ? -1.0 : 1.0);

	pidPWM = fabs(pidPWM);									// Use absolute value for PWMs
	myPWM = fabs(curPWM);

	if (myPWM == 0.0) {										// If stopped, set speed to minimum value
		if (pidPWM <= 0.2) {
			myPWM = pidPWM;
			vReturn = true;
		} else {
			myPWM = 0.2;
		}

	} else {												// Ramp speed
		myPWM = myPWM + 0.05;

		if (myPWM >= pidPWM) {
			myPWM = pidPWM;
			vReturn = true;
		}
	}

	curPWM = myPWM * direction;								// Apply direction and set the current PWM

	return vReturn;
}

