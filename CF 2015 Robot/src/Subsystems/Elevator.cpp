#include "Elevator.h"
#include "../Robotmap.h"
#include "../Commands/AllCommands.h"
#include <math.h>

Elevator::Elevator() : Subsystem("Elevator") {
	f_potZeroOffsetL    = 3873;
	f_potZeroOffsetR   	= 949;
	f_potCheckDown		= 1100;
	f_potCheckUp		= 975;
	f_elevMaxPosition	= 2815;
	f_toteCenterMin  	= 1520;
	f_toteEjectAbove	= 2150;
	f_toteEjectBelow	= 1500;

	f_binLoad 			= 25;
	f_binRelease		= 2400;
	f_feederLoad		= 2815;
	f_liftFromTote		= 1215;
	f_liftFromFloor		= 335;
	f_carry				= 550;
	f_step				= 1080;
	f_stepUnload		= 650;
	f_stepTote			= 1930;
	f_stepToteUnload 	= 1465;
	f_platform			= 490;
	
	m_elevPID = new PIDControl();

	#if (MY_ROBOT == 0)													// Schumacher
		m_motor1 = new VictorSP(PWM_ELEVATOR_1);
		m_motor2 = new VictorSP(PWM_ELEVATOR_2);
	#else																// Otis
		m_motor1 = new Victor(PWM_ELEVATOR_1);
		m_motor2 = new Victor(PWM_ELEVATOR_2);
	#endif

	m_elevPotL = new AnalogInput(AI_ELEVATOR_POT_L);
	m_elevPotL->SetAverageBits(2);
	m_elevPotL->SetOversampleBits(0);

	m_elevPotR = new AnalogInput(AI_ELEVATOR_POT_R);
	m_elevPotR->SetAverageBits(2);
	m_elevPotR->SetOversampleBits(0);

	m_elevPotCheck = new DigitalInput(DI_ELEV_POT_CHECK);
	m_elevTipCheck = new DigitalInput(DI_ELEV_TIP_CHECK);

	m_potInUse = potRight;
	m_potStatus[0] = m_potStatus[1] = 0;								// Pot Status Bit Map: 0=Out of Sync; 1=No Change; 2=Failed  Check

	m_elevDirection = dDown;
	m_elevPWM = 0;
	m_elevTarget = GetPosition(m_potInUse);

	m_onTarget = true;
	m_rampDone = false;
	m_runPID = true;

	m_elevBrake = new Solenoid(S_ELEVATOR_BRAKE);
	m_brakeState = sOff;

	m_toteEject = new Solenoid(S_TOTE_EJECT);
	m_toteEjectState = sOff;

	m_toteCenter = new Solenoid(S_TOTE_CENTER);
	m_toteCenterState = sOff;
}

Elevator::~Elevator() {
	delete m_elevPID;
	delete m_motor1;
	delete m_motor2;
	delete m_elevPotL;
	delete m_elevPotR;
	delete m_elevBrake;
	delete m_toteEject;
	delete m_toteCenter;
}

int32_t Elevator::GetPosition(ElevPot pot) {
	if (pot == potLeft) {
		return f_potZeroOffsetL - m_elevPotL->GetAverageValue();
	} else if (pot == potRight) {
		return m_elevPotR->GetAverageValue() - f_potZeroOffsetR;
	} else {
		return 0;
	}
}

void Elevator::IncrementSetpoint(ElevDirection direction) {
	if (direction == dUp) {
		if (m_elevTarget < f_elevMaxPosition) SetSetpoint(m_elevTarget + 15);
	} else {
		if (m_elevTarget > 0) SetSetpoint(m_elevTarget - 15);
	}
}

void Elevator::InitDefaultCommand() {
}

bool Elevator::IsOnTarget() {
	return m_onTarget;
}

void Elevator::LogPotInUse() {
	sprintf(m_log, "Elevator: %s Pot in Use", ElevPotName(m_potInUse).c_str());
	MyRobot::robotLog->Write(m_log);
}

void Elevator::RunWithJoystick(float joyPWM){
	static bool elevAtBottom = false;									// Used to prevent hunting when elevator is at Bottom
	static bool elevAtTop = false;										// or Top

	if (m_potInUse == potNone) {
		joyPWM = 0;

	} else {
		m_elevTarget = GetPosition(m_potInUse);							// Get position from pot in use
		if (!m_onTarget) {
			m_onTarget = true;
			MyRobot::powerPanel->SetCapturePower(false);
		}

//		printf("Position=%d  Pwm=%f  atBottom=%d  atTop=%d\n", m_elevTarget, joyPWM, elevAtBottom, elevAtTop);

		if (joyPWM < 0 ) {												// Move Elevator Down
			if (elevAtBottom) {											// At bottom
				joyPWM = 0;

			} else if (m_elevTarget <= 25) {							// Reached bottom
				elevAtBottom = true;
				joyPWM = 0;												// Stop

//			} else if (m_elevTipCheck->Get()) {							// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TAKE THIS OUT !!!!!!!!!!!!!!!!!
//				joyPWM = 0;
//				MyRobot::robotLog->Write("Elevator: Robot Tip Detected (Joystick)");

			} else {
				if (elevAtTop) elevAtTop = false;

				SetBrake(sOff);
				if (m_elevTarget < 360) joyPWM *= 0.4;					// Slow down within 6" of bottom
				ElevPotCheck(mDown);
			}

		} else if (joyPWM > 0) {										// Move Elevator Up
			if (elevAtTop) {											// At Top
				joyPWM = 0;

			} else if (m_elevTarget >= (f_elevMaxPosition - 15)) {		// Reached Top
				elevAtTop = true;
				SetBrake(sOn);
				joyPWM = 0;												// Stop

			} else {
				if (elevAtBottom) elevAtBottom = false;

				SetBrake(sOff);
				if (m_elevTarget > (f_elevMaxPosition - 360)) joyPWM *= 0.6;	// Slow down within 6" of top
				ElevPotCheck(mUp);
			}

		} else {
			ElevPotCheck(mStop);
		}
	}

	m_motor1->Set(-joyPWM);												// Joystick PWM > 0 moves Elevator UP
	m_motor2->Set(joyPWM);
}

void Elevator::RunPID(bool showPID){
	static int 	counter = 0;

	if (!m_runPID) return;
//	printf("Elevator:  PotCheck=%d  TipDetect=%d\n", m_elevPotCheck->Get(), m_elevTipCheck->Get());

	if (m_potInUse == potNone) {
		m_elevPWM = 0;
		if (!m_onTarget) {
			m_onTarget = true;
			MyRobot::powerPanel->SetCapturePower(false);
		}

	} else if(!m_onTarget){
		int32_t elevPosition = GetPosition(m_potInUse);					// Get position from pot in use

		if (m_elevDirection != dUp) {									// Elevator moving down
			ElevPotCheck(mDown);

			if (m_elevDirection != dDownToLoad) {
				if (m_elevTarget - elevPosition > 0) {					// Target above current position (Overshot target)
					if (m_elevDirection != dDownTooFar) {				// Set I coefficient high (Faster upward movement)
						m_elevDirection = dDownTooFar;
						m_elevPID->SetCoefficient('I', 200, 0, 0.01);
					}
				} else if (m_elevDirection != dDown) {
					m_elevDirection = dDown;							// Set I coefficient low (Force downward movement if required)
					m_elevPID->SetCoefficient('I', 100, 0, 0.0005);
				}
			}

		} else {														// Elevator moving up
			ElevPotCheck(mUp);
		}

		if (m_rampDone) {												// Ramp of PWM completed
			m_elevPWM = m_elevPID->Calculate((float)elevPosition, showPID);
		} else {														// Ramp PWM to calculated PID value to minimize sudden movement
			m_rampDone = RampPWM(m_elevPWM, m_elevPID->Calculate((float)elevPosition, showPID));
		}

		if (abs(m_elevTarget - elevPosition) < 15) {					// Elevator within 0.25 inch of Target
			counter++;
			if (counter > 2) {											// Maintain position at least 60 ms
				m_onTarget = true;
				m_elevPWM = 0;

				if (m_elevDirection == dUp) {
					MyRobot::powerPanel->SetCapturePower(false);
					sprintf(m_log, "Elevator: Peak Power=%5.1f  Average Power=%5.1f",
							MyRobot::powerPanel->GetPeakPower(),
							MyRobot::powerPanel->GetAveragePower());
					MyRobot::robotLog->Write(m_log);
				}
			}
		} else {														// Reset counter if not in deadband
			counter = 0;
		}

//		if ((m_elevDirection == dDown || m_elevDirection == dDownToLoad) && m_elevTipCheck->Get()){  // !!!!!!!!TAKE THIS OUT !!!!!!
//			m_elevPWM = 0;
//			m_onTarget = true;
//			m_elevTarget = elevPosition;
//			MyRobot::robotLog->Write("Elevator: Robot Tip Detected (PID)");
//		}

	} else {															// Set PWM = 0 if on target
		m_elevPWM = 0;
		ElevPotCheck(mStop);
	}

	if (m_onTarget) {
		if (m_brakeState == sOff) SetBrake(sOn);
	} else if (m_brakeState == sOn) {
		SetBrake(sOff);
	}

	m_motor1->Set(-m_elevPWM);											// m_elevPWM > 0 moves Elevator UP
	m_motor2->Set(m_elevPWM);
}

void Elevator::SetBrake(SolenoidState state) {							// Turn Brake On and Off
	if (m_brakeState != state) {
		m_brakeState = state;
		m_elevBrake->Set(state == sOn);
	}
}

void Elevator::SetConstant(std::string key, int32_t value) {			// Set constant values read from INI file
	if(key == "zeroOffsetL") {
		f_potZeroOffsetL = value;
		sprintf(m_log, "Elevator: Set Zero Offset L=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "zeroOffsetR") {
		f_potZeroOffsetR = value;
		sprintf(m_log, "Elevator: Set Zero Offset R=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "potCheckDown") {
		f_potCheckDown = value;
		sprintf(m_log, "Elevator: Set Pot Check Down=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "potCheckUp") {
		f_potCheckUp = value;
		sprintf(m_log, "Elevator: Set Pot Check Up=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "maxPosition") {
		f_elevMaxPosition = value;
		sprintf(m_log, "Elevator: Set Max Position=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "toteCenterMin") {
		f_toteCenterMin = value;
		sprintf(m_log, "Elevator: Set Tote Center Min=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "toteEjectAbove") {
		f_toteEjectAbove = value;
		sprintf(m_log, "Elevator: Set Tote Eject Above=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "toteEjectBelow") {
		f_toteEjectBelow = value;
		sprintf(m_log, "Elevator: Set Tote Eject Below=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "binLoad") {
		f_binLoad = value;
		sprintf(m_log, "Elevator: Set Bin Load=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "binRelease") {
		f_binRelease = value;
		sprintf(m_log, "Elevator: Set Bin Release=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "feederLoad") {
		f_feederLoad = value;
		sprintf(m_log, "Elevator: Set Feeder Load=%d" , value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "liftFromTote") {
		f_liftFromTote = value;
		sprintf(m_log, "Elevator: Set Lift From Tote=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if(key == "liftFromFloor") {
		f_liftFromFloor = value;
		sprintf(m_log, "Elevator: Set Lift From Floor=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if (key == "carry") {
		f_carry = value;
		sprintf(m_log, "Elevator: Set Carry=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if (key == "step") {
		f_step = value;
		sprintf(m_log, "Elevator: Set Step=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if (key == "stepUnload") {
		f_stepUnload = value;
		sprintf(m_log, "Elevator: Set Step Unload=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if (key == "stepTote") {
		f_stepTote = value;
		sprintf(m_log, "Elevator: Set Step Tote=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if (key == "stepToteUnload") {
		f_stepToteUnload = value;
		sprintf(m_log, "Elevator: Set Step Tote Unload=%d", value);
		MyRobot::robotLog->Write(m_log);

	} else if (key == "platform") {
		f_platform = value;
		sprintf(m_log, "Elevator: Set Platform =%d", value);
		MyRobot::robotLog->Write(m_log);
	}
}

void Elevator::SetElevPID(ElevDirection direction) {					// Set PID coefficients based on direction of movement
	m_elevDirection = direction;
	m_elevPID->SetInputRange(0, f_elevMaxPosition);

	switch (m_elevDirection) {
		case dUp:														// Elevator moving UP
			m_elevPID->SetCoefficient('P', 0, .002, 0);
			m_elevPID->SetCoefficient('I', 200, 0, 0.0005);				// Add I when near target to increase PWM
			m_elevPID->SetCoefficient('D', 200, 0, 0.004);				// Add D when near target to brake if moving too fast
			m_elevPID->SetOutputRange(-0.4, 0.7);						// -0.4  0.6
			break;

		case dDown:														// Elevator moving DOWN and above target
			m_elevPID->SetCoefficient('P', 0, .002, 0);
			m_elevPID->SetCoefficient('I', 100, 0, 0.0005);				// Add I when near target to increase PWM
			m_elevPID->SetCoefficient('D', 0, 0.015, 0);				// Use D to brake if moving too fast
			m_elevPID->SetOutputRange(-0.4, 0.7);						// -0.4  0.6
			break;

		case dDownToLoad:
			m_elevPID->SetCoefficient('P', 0, .002, 0);
			m_elevPID->SetCoefficient('I', 100, 0, 0.0005);				// Add I when near target to increase PWM
			m_elevPID->SetCoefficient('D', 0, 0.015, 0);				// Use D to brake if moving too fast
			m_elevPID->SetOutputRange(-0.8, 0.8);						//
			break;

		case dDownTooFar:												// Elevator moving DOWN but overshot target
			m_elevPID->SetCoefficient('P', 0, .002, 0);
			m_elevPID->SetCoefficient('I', 200, 0, 0.01);				// Add large I when near target to force upward return to target
			m_elevPID->SetCoefficient('D', 0, 0.015, 0);
			m_elevPID->SetOutputRange(-0.4, 0.7);						// -0.4  0.7
			break;

		default:;
	}
}

void Elevator::SetElevPosition(ElevPosition position) {					// Determine PID setpoint for new position
	switch(position) {
		case pBinLoad:			SetSetpoint(f_binLoad);			break;
		case pBinRelease:		SetSetpoint(f_binRelease); 		break;
		case pFeederLoad:		SetSetpoint(f_feederLoad); 		break;
		case pLiftFromTote: 	SetSetpoint(f_liftFromTote); 	break;
		case pLiftFromFloor:	SetSetpoint(f_liftFromFloor); 	break;
		case pCarry:			SetSetpoint(f_carry); 			break;
		case pStep:				SetSetpoint(f_step); 			break;
		case pStepUnload:		SetSetpoint(f_stepUnload);		break;
		case pStepTote:			SetSetpoint(f_stepTote); 		break;
		case pStepToteUnload:	SetSetpoint(f_stepToteUnload); 	break;
		case pPlatform:			SetSetpoint(f_platform); 		break;
		default:;
	}
}

void Elevator::SetRunPID(bool run) {
	m_runPID = run;
}

void Elevator::SetToteCenter(SolenoidState state) {						// Turn Tote Center On and Off
	if (state == sOn && GetPosition(m_potInUse) < f_toteCenterMin) return;

	if (m_toteCenterState != state) {
		m_toteCenterState = state;
		m_toteCenter->Set(state == sOn);
	}
}

void Elevator::SetToteEject(SolenoidState state) {						// Turn Tote Eject On and Off
	if (state == sOn && GetPosition(m_potInUse) > f_toteEjectBelow && GetPosition(m_potInUse) < f_toteEjectAbove) return;

	if (m_toteEjectState != state) {
		m_toteEjectState = state;
		m_toteEject->Set(state == sOn);
	}
}

void Elevator::StopMotors() {											// Stop Elevator Motors
	if (!m_onTarget) {
		m_elevTarget = GetPosition(m_potInUse);							// Get position from pot in use
		m_onTarget = true;
		MyRobot::powerPanel->SetCapturePower(false);
	}

	m_motor1->Set(0);
	m_motor2->Set(0);
	SetBrake(sOn);
}

void Elevator::TuneElevPID() {
	m_elevPID->SetCoefficient('P', (float)MyRobot::dashboard->GetDashValue(DV_PID_P_THRESHOLD),
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_P_ABOVE) / 10000,
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_P_BELOW) / 10000);
	m_elevPID->SetCoefficient('I', (float)MyRobot::dashboard->GetDashValue(DV_PID_I_THRESHOLD),
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_I_ABOVE) / 10000,
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_I_BELOW) / 10000);
	m_elevPID->SetCoefficient('D', (float)MyRobot::dashboard->GetDashValue(DV_PID_D_THRESHOLD),
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_D_ABOVE) / 10000,
								   (float)MyRobot::dashboard->GetDashValue(DV_PID_D_BELOW) / 10000);

	SetSetpoint(MyRobot::dashboard->GetDashValue(DV_PID_SETPOINT));

	float maxPWM = (float)MyRobot::dashboard->GetDashValue(DV_PID_MAX_PWM) / 100;

	printf("Tune Elev PID: MaxPWM=%5.2f  Setpoint=%d\n", maxPWM, m_elevTarget);
}

// ******************** PRIVATE ********************

void Elevator::ElevPotCheck(CheckMove move) {							// Pot Status Bit Map: 0=Out of Sync; 1=No Change; 2=Failed  Check
	static CheckState		checkState[] = {cDone, cDone};				// Check State: cDone; cUnknown; cExpected; cUnexpected
	static CheckMove	 	checkMove = mStop;							// Check Move: mStop; mDown; mUp
	static int32_t          elevPosition[2];							// Current Elev position
	static int32_t          elevChange[2];								// Change in Elev position from start
	static int32_t			limitLower = 0;								// Lower Limit of Expected pot check range
	static int32_t          limitUpper = 0;								// Upper Limit of Expected pot check range
	static bool				lookForCradle = false;
	static int				moveCounter = 0;							// Counter for determining pot change
	static int32_t          startPosition[2];							// Elev position at start of movement

	return;		// !!!!!!!!!!!!!!! OTIS ONLY !!!!!!!!!!!!!

	if (move == mStop) {												// Reset move flag when stopped
		if (checkMove != mStop) {
			checkMove = mStop;

			if (abs(elevPosition[0] - elevPosition[1]) > 30) {			// Determine whether or not pots are in sync
				if ((m_potStatus[0] & 1) == 0) {						// Set Sync bit if not
					m_potStatus[0] |= 1;
					m_potStatus[1] |= 1;
					MyRobot::robotLog->Write("Elevator: Pots out of Sync");
				}

			} else if ((m_potStatus[0] & 1) == 1) {						// Clear Sync bit if they are
				m_potStatus[0] ^= 1;
				m_potStatus[1] ^= 1;
				MyRobot::robotLog->Write("Elevator: Pots in Sync");
			}
		}
		return;
	}

	elevPosition[0] = GetPosition(potLeft);								// Get current position from each pot
	elevPosition[1] = GetPosition(potRight);

	if (checkMove != move) {											// Start of new move
		checkMove = move;
		moveCounter = 0;												// Reset counter
		startPosition[0] = elevPosition[0];								// Capture current position
		startPosition[1] = elevPosition[1];
		checkState[0] = checkState[1] = cUnknown;						// Reset Check State

		if (checkMove == mDown) {										// Set Limits based on direction of movement
			limitLower = f_potCheckDown - 60;
			limitUpper = f_potCheckDown + 60;
			if (elevPosition[0] < limitUpper) checkState[0] = cDone;	// Set State to done if below sensor range
			if (elevPosition[1] < limitUpper) checkState[1] = cDone;

		} else {														// Move Up
			limitLower = f_potCheckUp - 60;
			limitUpper = f_potCheckUp + 60;
			if (elevPosition[0] > limitLower) checkState[0] = cDone;	// Set State to done if above sensor range
			if (elevPosition[1] > limitLower) checkState[1] = cDone;
		}

		lookForCradle = (checkState[0] != cDone || checkState[1] != cDone);
	}

	if (moveCounter < 25) {												// Look for movement in 1st 500 msec
		elevChange[0] = abs(elevPosition[0] - startPosition[0]);		// Calculate change in position
		elevChange[1] = abs(elevPosition[1] - startPosition[1]);
																		// Increment counter if PWM > 0.2 or either pot has changed
		if (fabs(m_motor1->Get()) > 0.30 || elevChange[0] > 30 || elevChange[1] > 30) moveCounter++;

		if (moveCounter == 25) {										// 500 msec of movement has expired
			if (elevChange[0] <= 35 || elevChange[0] < (int32_t)(elevChange[1] * 0.5)) {	// Left pot has not changed or change < 50% of Right pot
				if ((m_potStatus[0] & 2) == 0) {						// Set No Change bit if not set
					m_potStatus[0] |= 2;
					sprintf(m_log, "Elevator: %s Pot value NOT changing", ElevPotName(0).c_str());
					MyRobot::robotLog->Write(m_log);
					UpdatePotStatus();
				}

			} else if ((m_potStatus[0] & 2) == 2) {						// Left Pot has changed and No Change bit is set
				m_potStatus[0] ^= 2;									// Clear bit
				sprintf(m_log, "Elevator: %s Pot value changing", ElevPotName(0).c_str());
				MyRobot::robotLog->Write(m_log);
				UpdatePotStatus();
			}

			if (elevChange[1] <= 30 || elevChange[1] < (int32_t)(elevChange[0] * 0.5)) {	// Right pot has not changed or change < 50% of Left pot
				if ((m_potStatus[1] & 2) == 0) {						// Set No Change bit if not set
					m_potStatus[1] |= 2;
					sprintf(m_log, "Elevator: %s Pot value NOT changing", ElevPotName(1).c_str());
					MyRobot::robotLog->Write(m_log);
					UpdatePotStatus();
				}

			} else if ((m_potStatus[1] & 2) == 2) {						// Right Pot has changed and No Change bit is set
				m_potStatus[1] ^= 2;									// Clear bit
				sprintf(m_log, "Elevator: %s Pot value changing", ElevPotName(1).c_str());
				MyRobot::robotLog->Write(m_log);
				UpdatePotStatus();
			}
		}
	}

	if (lookForCradle) {
		if (!m_elevPotCheck->Get()){
			lookForCradle = false;
			sprintf(m_log, "Elevator: Cradle detected at Left=%d  Right=%d", elevPosition[0], elevPosition[1]);
			MyRobot::robotLog->Write(m_log);
		}
	}

	for (int i = 0; i < 2; i++) {										// Use sensor to check accuracy of each pot
		if (checkState[i] != cDone) {									// Check not done
			CheckState newState = (elevPosition[i] >= limitLower && elevPosition[i] <= limitUpper) ? cExpected : cUnexpected;  // In range of sensor?

			if (checkState[i] == cExpected && newState == cUnexpected) {	// Moved out of expected range with no sensor detection
				if ((m_potStatus[i] & 4) == 0) {						// Set Check Fail bit if not set
					m_potStatus[i] |= 4;
					sprintf(m_log, "Elevator: %s Pot Failed Check (No detection within range)", ElevPotName(i).c_str());
					MyRobot::robotLog->Write(m_log);
					UpdatePotStatus();
				}

				checkState[i] = cDone;

			} else if (!m_elevPotCheck->Get()) {						// Sensor has detected cradle
				if (newState == cUnexpected) {							// Sensor detection outside of expected range
					if ((m_potStatus[i] & 4) == 0) {					// Set Check Fail bit if not set
						m_potStatus[i] |= 4;
						sprintf(m_log, "Elevator: %s Pot Failed Check at %d (Detection out of range)", ElevPotName(i).c_str(), elevPosition[i]);
						MyRobot::robotLog->Write(m_log);
						UpdatePotStatus();
					}

				} else if ((m_potStatus[i] & 4) == 4) {					// Sensor detection in expected range
					m_potStatus[i] ^= 4;								// Clear Check Fail bit if set
					sprintf(m_log, "Elevator: %s Pot Check OK", ElevPotName(i).c_str());
					MyRobot::robotLog->Write(m_log);
					UpdatePotStatus();
				}

				checkState[i] = cDone;

			} else {													// Set current check state
				checkState[i] = newState;
			}
		}
	}
}

std::string Elevator::ElevPotName(int index) {
	switch(index) {
		case potLeft:	return "Left";
		case potRight:	return "Right";
		case potNone:   return "None";
		default:		return "?";
	}
}

bool Elevator::RampPWM(float& curPWM, float pidPWM) {
	float 	myPWM;
	bool	vReturn = false;

	float direction = (pidPWM < 0 ? -1.0 : 1.0);

	pidPWM = fabs(pidPWM);												// Use absolute value for PWMs
	myPWM = fabs(curPWM);

	if (myPWM == 0.0) {													// If stopped, set speed to minimum value
		if (pidPWM <= 0.15) {											// 0.2
			myPWM = pidPWM;
			vReturn = true;
		} else {
			myPWM = 0.15;
		}

	} else {															// Ramp speed
		if (m_elevDirection == dDownToLoad) {
			myPWM = myPWM + 0.05;										// 0.05
		} else {
			myPWM = myPWM + 0.01;										// 0.05
		}

		if (myPWM >= pidPWM) {
			myPWM = pidPWM;
			vReturn = true;
		}
	}

	curPWM = myPWM * direction;											// Apply direction and set the current PWM

	return vReturn;
}

void Elevator::SetSetpoint(int32_t target) {							// Set PID Setpoint
	target = (target > f_elevMaxPosition) ? f_elevMaxPosition:
							  (target < 0) ? 0: target;

	m_elevTarget = target;
	m_onTarget = m_rampDone = false;
	m_elevPWM = 0;
																		// Set PID coefficients based on direction of travel
	if (m_elevTarget > GetPosition(m_potInUse)) {						// Moving Up
		SetElevPID(dUp);
		MyRobot::powerPanel->SetCapturePower(true);

		sprintf(m_log, "Elevator: Set Setpoint=%4d (Move Up)", m_elevTarget);
		MyRobot::robotLog->Write(m_log);

	} else {															// Moving Down
		if (m_elevTarget == f_binLoad) {
			SetElevPID(dDownToLoad);
		} else {
			SetElevPID(dDown);
		}

		sprintf(m_log, "Elevator: Set Setpoint=%4d (Move Down)", m_elevTarget);
		MyRobot::robotLog->Write(m_log);
	}

	m_elevPID->SetSetpoint((float)target);
	m_elevPID->Reset();
}

void Elevator::UpdatePotStatus() {
	ElevPot newInUse = m_potInUse;										// Initialize new In Use

	switch (m_potInUse) {												// Current pot in use
		case potLeft:
			if ((m_potStatus[0] & 6) != 0) {							// If No Change or Check Fail bit is set
				if ((m_potStatus[1] & 6) == 0) {						// Change to other pot if OK
					newInUse = potRight;
				} else {												// Otherwise Disable elevator
					newInUse = potNone;
				}
			}
			break;

		case potRight:
			if ((m_potStatus[1] & 6) != 0) {							// If No Change or Check Fail bit is set
				if ((m_potStatus[0] & 6) == 0) {						// Change to other pot if OK
					newInUse = potLeft;
				} else {												// Otherwise Disable elevator
					newInUse = potNone;
				}
			}
			break;

		default:;
	}

	if (m_potInUse != newInUse) {										// Log any change in Pot being used
		m_potInUse = newInUse;
		sprintf(m_log, "Elevator: Pot in Use changed to %s", ElevPotName(m_potInUse).c_str());
		MyRobot::robotLog->Write(m_log);
	}
}

