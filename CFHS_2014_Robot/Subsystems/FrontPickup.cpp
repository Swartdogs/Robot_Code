#include "FrontPickup.h"
#include "../Robotmap.h"
#include <math.h>
#include "../Commands/AllCommands.h"

const INT32 c_leftArmZeroOffset = 768;		
const INT32 c_leftArmMaxPosition = 730;

const INT32 c_rightArmZeroOffset = 130;		
const INT32 c_rightArmMaxPosition = 730;

const INT32 c_armTargetDeadband = 4;	
const INT32 c_incrementValue = 10;


FrontPickup::FrontPickup(RobotLog* log) : Subsystem("FrontPickup") {
	m_robotLog = log;

	m_leftArm = 	new Victor(PWM_FRONT_PICKUP_LEFT_ARM);
	m_rightArm = 	new Victor(PWM_FRONT_PICKUP_RIGHT_ARM);
	
	m_leftWheels = 	new Relay(RELAY_FRONT_PICKUP_LEFT_ROLLERS, Relay::kBothDirections);
	m_rightWheels = new Relay(RELAY_FRONT_PICKUP_RIGHT_ROLLERS, Relay::kBothDirections);
	
	m_ballLoadedSensor = new DigitalInput(DI_FRONT_PICKUP_BALL_SENSOR);
	
	m_leftArmPot = 	new AnalogChannel(AI_FRONT_PICKUP_LEFT_ARM_POT);
	m_leftArmPot->SetAverageBits(2);
	m_leftArmPot->SetOversampleBits(0);

	m_rightArmPot = new AnalogChannel(AI_FRONT_PICKUP_RIGHT_ARM_POT);
	m_rightArmPot->SetAverageBits(2);
	m_rightArmPot->SetOversampleBits(0);
	
	m_leftArmPID = 	new PIDControl(0.004, 0.0, 0.01);
	m_leftArmPID->SetInputRange(0, 1000);
	m_leftArmPID->SetOutputRange(-0.7, 0.7);
	
	m_rightArmPID = new PIDControl(0.004, 0.0, 0.01);
	m_rightArmPID->SetInputRange(0, 1000);
	m_rightArmPID->SetOutputRange(-0.7, 0.7);
	
	m_leftArmTarget = GetPosition(pLeft);
	m_rightArmTarget = GetPosition(pRight);
	
	m_rightArmPID->SetSetpoint(m_rightArmTarget);
	m_leftArmPID->SetSetpoint(m_leftArmTarget);
	
	m_useJoystickLeft = false;
	m_useJoystickRight = false;
	m_joyLeft = 0;
	m_joyRight = 0;
}

FrontPickup::FrontMode FrontPickup::GetFrontPickupMode() {				// GET MODE
	return m_frontMode;
}

INT32 FrontPickup::GetPosition(Pot pot) {								// GET ARM POSITION
	switch(pot) {														// Arm position relative to offset
		case pLeft:
			return (c_leftArmZeroOffset - m_leftArmPot->GetAverageValue());
		case pRight:
			return (m_rightArmPot->GetAverageValue() - c_rightArmZeroOffset);
	}
	return 0;
}

bool FrontPickup::HasBall() {											// HAS BALL
	return !m_ballLoadedSensor->Get();
}

void FrontPickup::IncrementArm(Pot arm, bool up) {						// INCREMENT ARM TARGETS
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

void FrontPickup::InitDefaultCommand() {
}

bool FrontPickup::OnTarget() {											// ARMS AT TARGET POSITION
	return (m_leftOnTarget && m_rightOnTarget);
}

void FrontPickup::Periodic() { 											// PERIODIC (Called every periodic loop)
	static int delayCount = 0;
	
	float leftSpeed;
	float rightSpeed;
	float syncFactor;
	bool  isTooHigh;
	bool  isTooLow;
	
	INT32 curLeftPosition = GetPosition(pLeft);							// Current arm positions
	INT32 curRightPosition = GetPosition(pRight);
	
	INT32 curLeftError = abs(curLeftPosition - m_leftArmTarget);		// Error between position and target
	INT32 curRightError = abs(curRightPosition - m_rightArmTarget);
																		// Use sync if targets are the same and error > 10
	bool syncArms = (m_leftArmTarget == m_rightArmTarget && abs(curLeftError - curRightError) > 10);
	
	if(curLeftError <= 100) {											// Apply I coefficient if near target
		m_leftArmPID->SetPID(0.004, 0.0005, 0.01);	
	} else {
		m_leftArmPID->SetPID(0.004, 0.0, 0.01);
	}
	leftSpeed = m_leftArmPID->Calculate((float) curLeftPosition);		// Calculate PID speed
	
	if(curRightError <= 100) {											// Apply I coefficent if near target
		m_rightArmPID->SetPID(0.004, 0.0005, 0.01);
	} else {
		m_rightArmPID->SetPID(0.004, 0.0, 0.01);
	}
	rightSpeed = m_rightArmPID->Calculate((float) curRightPosition);	// Calculate PID speed
	
	if(m_useJoystickLeft) {
		m_leftArmTarget = curLeftPosition;												// Set PID target
		m_leftArmPID->SetSetpoint(m_leftArmTarget);
		
		isTooHigh = curLeftPosition > (c_leftArmMaxPosition - c_armTargetDeadband);		// Is position outside limits
		isTooLow = curLeftPosition < c_armTargetDeadband;
		
		if(((m_joyLeft > 0) && isTooHigh) || ((m_joyLeft < 0) && isTooLow)) {			// Inhibit travel outside of limits
			leftSpeed = 0;
		} else if((m_joyLeft * leftSpeed < 0) || (fabs(m_joyLeft) > fabs(leftSpeed))) { // Use joystick value if greater than PID value
			leftSpeed = m_joyLeft;														// or in opposite direction
			m_frontMode = fUnknown;					
		}
	
	} else if(curLeftError <= c_armTargetDeadband) {									// Error within deadband 
		m_leftOnTarget = true;
		if (fabs(leftSpeed) < .05) leftSpeed = 0;
		m_leftArmPID->Reset();

	} else if (syncArms && curRightError > curLeftError && curLeftError > 30) {			// Left arm moving faster and error > 30
		syncFactor = (curRightError - curLeftError) * 0.000; // 0.002					// Calculate and apply sync factor
		if (leftSpeed > 0) {
			leftSpeed = (leftSpeed > syncFactor) ? leftSpeed -= syncFactor : 0; 
		} else {
			leftSpeed = (leftSpeed < syncFactor) ? leftSpeed += syncFactor : 0;
		}
	}
	
	if(m_useJoystickRight) {
		m_rightArmTarget = curRightPosition;											// Set PID target
		m_rightArmPID->SetSetpoint(m_rightArmTarget);
		
		isTooHigh = curRightPosition > (c_rightArmMaxPosition - c_armTargetDeadband);	// Is position outside limits
		isTooLow = curRightPosition < c_armTargetDeadband;
		
		if(((m_joyRight > 0) && isTooHigh) || ((m_joyRight < 0) && isTooLow)) {			// Inhibit tarvel outside of limits
			rightSpeed = 0;
		} else if((m_joyRight * rightSpeed < 0) || (fabs(m_joyRight) > fabs(rightSpeed))) {	// Use joystick value if greater than PID value
			rightSpeed = m_joyRight;														// or in opposite direction
			m_frontMode = fUnknown;
		}
	
	} else if(curRightError <= c_armTargetDeadband) {									// Error within deadband
		m_rightOnTarget = true;
		if (fabs(rightSpeed) < 0.05) rightSpeed = 0;
		m_rightArmPID->Reset();

	} else if (syncArms && curLeftError > curRightError && curRightError > 30) {		// Right arm moving faster and error > 30
		syncFactor = (curLeftError - curRightError) * 0.000; // 0.002					// Calculate and apply sync factor
		if (rightSpeed > 0) {
			rightSpeed = (rightSpeed > syncFactor) ? rightSpeed -= syncFactor : 0; 
		} else {
			rightSpeed = (rightSpeed < syncFactor) ? rightSpeed += syncFactor : 0;
		}
	}
	//printf("Pot Left=%f   Right = %f\n", m_leftArmPot->GetAverageVoltage(), m_rightArmPot->GetAverageVoltage());
	//printf("Left: %d Right: %d\n", curLeftPosition, curRightPosition);
	//printf("Left Arm=%d  Pot=%d  Right Arm=%d  Pot=%d\n",curLeftPosition, m_leftArmPot->GetAverageValue(), curRightPosition, m_rightArmPot->GetAverageValue());
	//printf("Left: %d LeftPWM: %.3f Right: %d RightPWM: %.3f Difference: %d\n", curLeftError, leftSpeed, curRightError, rightSpeed, (curLeftError - curRightError));
	
	m_leftArm->Set(leftSpeed);											// Set motor PWMs
	m_rightArm->Set(rightSpeed);
	
	switch (m_frontMode) {
	case fDeploy:														// Pickup Deployed
		if(!HasBall()) {												// No ball detected
			delayCount = 0;
		} else if(delayCount > 0) {										// Delay expired
			delayCount = 0;
			if(CommandBase::ballShooter->HasBall()) {					// Ball in the shooter
				SetPickupMode(fLowDeploy);								// Move to Low Deploy position
			} else if(CommandBase::ballShooter->GetShootState() != BallShooter::sReady) {	// Shooter not ready
				SetPickupMode(fWaitToStore);							// Wait to store
			} else {													
				SetPickupMode(fStore);									// Move to Store position
			}
		} else {														// Increment delay counter
			delayCount++;
		}
		break;
	
	case fWaitToStore:													// Waiting for shooter
		if(CommandBase::ballShooter->GetShootState() == BallShooter::sReady) SetPickupMode(fStore);		// Move to Store position
		break;

	case fWaitToLoad:													// Waiting for shooter
		if(CommandBase::ballShooter->GetShootState() == BallShooter::sReady) SetPickupMode(fLoad);		// Move to Load position
		break;

	case fLoad:															// Moving to load position
		if(m_leftOnTarget && m_rightOnTarget) SetPickupMode(fDropInShooter);	// Drop in Shooter when in position
		break;

	case fDropInShooter:												// Dropping ball in shooter
		if(!HasBall()) {												// No Ball
			delayCount = 0;
			SetPickupMode(fStore);										// Move to store position
		} else if(delayCount > 50) {									// Timeout
			delayCount = 0;
			SetPickupMode(fStore);										// Move to store position
		} else {
			delayCount++;												// Increment delay counter
		}
		break;
	
	case fLowShoot:														// Shooting ball from Low position
		if(HasBall()) {													// Still has ball
			delayCount = 0;
		} else if(delayCount > 5) {										// Delay expired
			delayCount = 0;
			SetPickupMode(fStore);										// Move to store position
		} else {
			delayCount++;												// Increment delay counter
		}
		break;
		
	case fAutoLoad:														// Loading ball in Autonomous											
		if(HasBall()) SetRollers(wOff);

	default:;
	}
}

void FrontPickup::SetJoystickLeft(float joyLeft) {						// SET LEFT JOYSTICK SPEED
	m_joyLeft = 0.7 * joyLeft;
}

void FrontPickup::SetJoystickRight(float joyRight) {					// SET RIGHT JOYSTICK SPEED
	m_joyRight = 0.7 * joyRight;
}

void FrontPickup::SetPickupMode(FrontMode mode) {						// SET PICKUP MODE
	FrontMode startMode = m_frontMode;
	m_leftWheels->Set(Relay::kOff);
	m_rightWheels->Set(Relay::kOff);
	
	switch (mode) {
	case fDeploy:														// Deploy pickup
		if (!HasBall()) {												// Inhibit if ball already in pickup
			SetSetpoints(5,5);											// Move to position			
			m_rightWheels->Set(Relay::kForward);						// Start rollers
			m_leftWheels->Set(Relay::kForward);
			CommandBase::ballShooter->Load();							// Move shooter to ready position
			CommandBase::backPickup->SetPickupMode(BackPickup::bStore); // Move backpickup to store position
			m_frontMode = fDeploy;
		}
		break;
	
	case fStore:														// Store pickup
		SetSetpoints(570,570);			
		m_frontMode = fStore;
		break;
	
	case fLoad:															// Load ball into Shooter
		if(HasBall() && !CommandBase::ballShooter->HasBall()) {			// Inhibit if no ball or ball already in shooter
			if(CommandBase::ballShooter->GetShootState() == BallShooter::sReady) {		// Shooter in ready position
				SetSetpoints(570, 570);									// Move to load position
				m_frontMode = fLoad;
			} else {
				CommandBase::ballShooter->Load();						// Move shooter to ready position
				m_frontMode = fWaitToLoad;								// Wait for shooter
			}
		}
		break;

	case fDropInShooter:												// Drop ball in the shooter
		SetSetpoints(720, 500);			
		m_frontMode = fDropInShooter;
		break;
	
	case fShoot:														// Move to shoot position
		if(HasBall()) {													// Position dependent on ball possession
			SetSetpoints(116, 116);
		} else {
			SetSetpoints(350, 350);
		}
		m_frontMode = fShoot;
		break;

	case fLowDeploy:													// Move to Low Goal position
		if(HasBall()) {													// Inhibit if no ball
			SetSetpoints(116,116);
			m_frontMode = fLowDeploy;
		}
		break;
		
	case fAutoLoad:														// Loading ball in Autonomous
		if(!HasBall()) SetRollers(wIn);
		SetSetpoints(116, 116);
		CommandBase::ballShooter->Load();								// Make sure shooter is ready to load
		m_frontMode = fAutoLoad;										

	default:
		m_frontMode = mode;
	}

	if (m_frontMode != startMode) {
		sprintf(m_log, "Front Pickup: Mode=%s", GetModeName(mode));
		m_robotLog->LogWrite(m_log);
	
	}
}

void FrontPickup::SetRollers(RollerMode mode) {							// SET ROLLER MODE
	if(mode == wIn) {
		m_leftWheels->Set(Relay::kForward);
		m_rightWheels->Set(Relay::kForward);
	} else if(mode == wOut) {
		m_leftWheels->Set(Relay::kReverse);
		m_rightWheels->Set(Relay::kReverse);
	} else {
		m_leftWheels->Set(Relay::kOff);
		m_rightWheels->Set(Relay::kOff);
	}
}

void FrontPickup::SetUseJoystickLeft(bool use) {						// SET USE LEFT JOYSTICK FLAG
	m_useJoystickLeft = use;
	m_robotLog->LogWrite("Front Pickup: Use joystick on Left");
}

void FrontPickup::SetUseJoystickRight(bool use) {						// SET USE RIGHT JOYSTICK FLAG
	m_useJoystickRight = use;
	m_robotLog->LogWrite("Front Pickup: Use joystick on Right");
}

void FrontPickup::StopMotors() {
	m_leftArm->Set(0.0);
	m_rightArm->Set(0.0);
	m_leftWheels->Set(Relay::kOff);
	m_rightWheels->Set(Relay::kOff);
}

//  ******************** PRIVATE ********************


char* FrontPickup::GetModeName(FrontMode mode) {
	switch(mode) {
	case fUnknown:  		return "Unknown";
	case fDeploy:			return "Deploy";
	case fWaitToStore:		return "WaitToStore";
	case fStore:			return "Store";
	case fWaitToLoad:		return "WaitToLoad";
	case fLoad:				return "Load";
	case fDropInShooter: 	return "DropInShooter";
	case fShoot:			return "Shoot";
	case fLowDeploy:		return "LowDeploy";
	case fLowShoot:			return "LowShoot";
	case fAutoLoad:			return "AutoLoad";
	default:				return "?";
	}
}

INT32 FrontPickup::LimitValue(Pot arm, INT32 position) {				// LIMIT POSITIONS TO WITHIN RANGE
	
	if(arm == pLeft) {
		position = (position > c_leftArmMaxPosition) ? c_leftArmMaxPosition :
				   (position < 0) ? 0  : position;
	} else {
		position = (position > c_rightArmMaxPosition) ? c_rightArmMaxPosition :
				   (position < 0) ? 0  : position;
	}
	
	return position;
}

void FrontPickup::SetSetpoints(INT32 leftPosition, INT32 rightPosition){	// SET POSITION SETPOINTS
	
	m_leftArmTarget = LimitValue(pLeft, leftPosition);
	m_leftArmPID->SetSetpoint(m_leftArmTarget);
	m_leftOnTarget = false;
	
	m_rightArmTarget = LimitValue(pRight, rightPosition);
	m_rightArmPID->SetSetpoint(m_rightArmTarget);
	m_rightOnTarget = false;
}


