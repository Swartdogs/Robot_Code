#include "BackPickup.h"
#include "../CommandBase.h"
#include "../Robotmap.h"
#include <math.h>

BackPickup::BackPickup(RobotLog* log) : Subsystem("BackPickup") {
	f_baseMotorDeadband = 3;
	f_baseZeroOffset = 830;	
	f_baseMaxPosition = 800;
	f_incrementValue = 10;
	
	f_storeSetpoint = 520;
	f_deploySetpoint = 265;
	f_passSetpoint = 570;
	f_shootSetpoint = 420;
	f_catchSetpoint = 420;
	f_startSetpoint = 610;	
	
	m_baseMotor = new Victor(PWM_BACK_PICKUP_BASE_MOTOR);
	m_rollers = new Victor(PWM_BACK_PICKUP_ROLLERS);
	
	m_baseMotorPot = new AnalogChannel(AI_BACK_PICKUP_BASE_POT);
	m_baseMotorPot->SetAverageBits(2);
	m_baseMotorPot->SetOversampleBits(0);
	
	m_baseMotorPID = new PIDControl(0.003, 0.0, 0.01);
	m_baseMotorPID->SetInputRange(0, 1024);
	m_baseMotorPID->SetOutputRange(-0.7, 0.7);
	
	m_ballSensor = new DigitalInput(DI_BACK_PICKUP_SENSOR);
	
	m_robotLog = log;
	
	m_useJoystick = false;
	m_onTarget = false;
	m_joySpeed = 0;
	
	
	SetPickupMode(bStore);
}

void BackPickup::IncrementArm(AdjustMode mode){
	if (mode == aUp) {
		m_baseTarget += f_incrementValue;
	} else {
		m_baseTarget -= f_incrementValue;
	}
	
	m_baseTarget = LimitValue(m_baseTarget);
	m_baseMotorPID->SetSetpoint(m_baseTarget);
	
	sprintf(m_log, "BackPickup:  Increment Arm to %d", m_baseTarget);
	m_robotLog->LogWrite(m_log);
}

BackPickup::BackMode BackPickup::GetBackPickupMode(){					// GET MODE
	return m_backMode;
}

INT32 BackPickup::GetPosition() {										// GET ARM POSITION
	return (f_baseZeroOffset - m_baseMotorPot->GetAverageValue());		// Arm position relative to offset
}

bool BackPickup::HasBall() {
	return (!m_ballSensor->Get());
}

void BackPickup::InitDefaultCommand() {
}

bool BackPickup::OnTarget(){											// ARM AT TARGET POSITION
	return m_onTarget;
}

void BackPickup::Periodic(){											// PERIODIC (Called every periodic loop)
	static int ballTimerCount = 0;

	float motorPower;
	bool  isTooHigh;
	bool  isTooLow;
	
	INT32 curPosition = GetPosition();									// Get arm position	
	INT32 curError = abs(curPosition - m_baseTarget);					// Error between position and target
	
	if(curError <= 50) {												// Add I coefficient if close to target
		m_baseMotorPID->SetPID(0.003, 0.0002, 0.01);
	} else {
		m_baseMotorPID->SetPID(0.003, 0.0, 0.01);
	}
	
	motorPower = -m_baseMotorPID->Calculate(curPosition);				// Get PID calculated motor power
	
	if(m_useJoystick) {
		m_baseTarget = curPosition;										// Set PID target
		m_baseMotorPID->SetSetpoint(m_baseTarget);
		isTooHigh = curPosition > (f_baseMaxPosition - f_baseMotorDeadband);
		isTooLow = curPosition < f_baseMotorDeadband;
		
		if(((m_joySpeed > 0) && isTooHigh) || ((m_joySpeed < 0) && isTooLow)) {					// Inhibit travel outside of limits
			motorPower = 0;
		} else if((m_joySpeed * motorPower < 0) || (fabs(m_joySpeed) > fabs(motorPower))){		// Use joystick value if greater than PID value
			motorPower = -m_joySpeed;															// or in opposite direction
			m_backMode = bUnknown;
		}
		//printf("BackPickup Pot: %d JoyStick Speed: %.3f PID Speed: %.3f Motor Speed: %.3f\n", curPosition, m_joySpeed, m_baseMotorPID->Calculate(curPosition), motorPower);
		//printf("Back Position: %d\n", curPosition);
		
	} else {
		if(abs(curPosition - m_baseTarget) <= f_baseMotorDeadband) {	// Error within deadband
			m_onTarget = true;											
			motorPower = 0;
			m_baseMotorPID->Reset();
		}
	}
	
//	printf("Back Pickup: Position=%d  PWM=%f\n", curPosition, motorPower);
	
	m_baseMotor->Set(motorPower);										// Set motor PWM
	
	switch (m_backMode) {
	case bStore:
		if(m_onTarget && m_rollerMode != rManualIn) SetRollerMode(rOff);
		break;
	
	case bDeploy:
		if (HasBall() || ballTimerCount > 0) {
			if (ballTimerCount > 80 || ShooterHasBall()) {
				ballTimerCount = 0;
				SetPickupMode(bStore);
			} else {
				ballTimerCount++;
			}
		}
		break;
		
	case bWaitToDeploy:													// Wait until shooter is ready and front pickup not in the way
		if(CommandBase::ballShooter->GetShootState() == BallShooter::sReady && CommandBase::frontPickup->OnTarget()) {
			SetPickupMode(bDeploy);										// Deploy arm
		}
		break;

	case bPass:															
		if(m_onTarget) {															// Arm is in pass position
			if(!ShooterHasBall() || ballTimerCount > 0) {							// Move to Store after ball is gone and short delay
				if(ballTimerCount > 25) {										// Due to ball bounce, ignore sensor after delay starts
					ballTimerCount = 0;
					SetPickupMode(bStore);
				} else {
					ballTimerCount++;
				}
			}
		}
		break;
		
	default:;
	}
	
	// Roller Mode
	switch(m_rollerMode) {
	case rIn:
	case rManualIn:
		m_rollers->Set(1.0);
		break;
	case rOut:
		m_rollers->Set(-1.0);
		break;
	default:
		m_rollers->Set(0.0);
	}
}

void BackPickup::SetConstant(const char* key, INT32 value) {
	if(strcmp(key,"baseMotorDeadband") == 0) {
		f_baseMotorDeadband = value;
		sprintf(m_log, "BackPickup:  Set BaseMotorDeadband=%d", value);
		m_robotLog->LogWrite(m_log);
	
	} else if(strcmp(key,"baseZeroOffset") == 0) {
		f_baseZeroOffset = value;
		sprintf(m_log, "BackPickup:  Set BaseZeroOffset=%d", value);
		m_robotLog->LogWrite(m_log);
		
	} else if(strcmp(key,"baseMaxPosition") == 0) {
		f_baseMaxPosition = value;
		sprintf(m_log, "BackPickup:  Set BaseMaxPosition=%d", value);
		m_robotLog->LogWrite(m_log);
		
	} else if(strcmp(key,"incrementValue") == 0) {
		f_incrementValue = value;
		sprintf(m_log, "BackPickup:  Set IncrementValue=%d", value);
		m_robotLog->LogWrite(m_log);
		
	} else if(strcmp(key,"storeSetpoint") == 0) {
		f_storeSetpoint = value;
		sprintf(m_log, "BackPickup:  Set StoreSetpoint=%d", value);
		m_robotLog->LogWrite(m_log);
		
	} else if(strcmp(key,"deploySetpoint") == 0) {
		f_deploySetpoint = value;
		sprintf(m_log, "BackPickup:  Set DeploySetpoint=%d", value);
		m_robotLog->LogWrite(m_log);
		
	} else if(strcmp(key,"passSetpoint") == 0) {
		f_passSetpoint = value;
		sprintf(m_log, "BackPickup:  Set PassSetpoint=%d", value);
		m_robotLog->LogWrite(m_log);
		
	} else if(strcmp(key,"shootSetpoint") == 0) {
		f_shootSetpoint = value;
		sprintf(m_log, "BackPickup:  Set ShootSetpoint=%d", value);
		m_robotLog->LogWrite(m_log);
		
	} else if(strcmp(key,"catchSetpoint") == 0) {
		f_catchSetpoint = value;
		sprintf(m_log, "BackPickup:  Set CatchSetpoint=%d", value);
		m_robotLog->LogWrite(m_log);
		
	} else if(strcmp(key,"startSetpoint") == 0) {
		f_startSetpoint = value;
		sprintf(m_log, "BackPickup:  Set StartSetpoint=%d", value);
		m_robotLog->LogWrite(m_log);
	}
}

void BackPickup::SetJoystickSpeed(float speed) {						// SET JOYSTICK SPEED
	m_joySpeed = 0.7 * speed;
}


void BackPickup::SetPickupMode(BackMode mode){							// SET PICKUP MODE
	BackMode startMode = m_backMode;
	
	if(mode != bStore) {
		SetRollerMode(rOff);
	}
	
	switch (mode) {
	case bDeploy:
		if(!ShooterHasBall()) {											// No ball in Shooter
			if(CommandBase::ballShooter->GetShootState() == BallShooter::sReady && 
			  (CommandBase::frontPickup->GetFrontPickupMode() == ((CommandBase::frontPickup->HasBall()) ? FrontPickup::fShoot : FrontPickup::fStore))) { // Shooter is ready and Front Pickup not in the way
				SetRollerMode(rIn);										// Start rollers and deploy pickup
				SetSetpoint(f_deploySetpoint);	// 272
				m_backMode = bDeploy;
					
			} else {
				CommandBase::ballShooter->Load();								// Move Shooter to ready
				CommandBase::frontPickup->SetPickupMode((CommandBase::frontPickup->HasBall()) ? FrontPickup::fShoot : FrontPickup::fStore);	// Move Front Pickup out of the way
				m_backMode = bWaitToDeploy;						
			}
		}
		break;
		
	case bPass:
		if(ShooterHasBall()) {											// Ball in the Shooter
			SetRollerMode(rOut);		
			SetSetpoint(f_passSetpoint);	// 570
			m_backMode = bPass;
		}
		break;
		
	case bStore:
		SetSetpoint(f_storeSetpoint);	// 520
		m_backMode = bStore;
		break;
		
	case bShoot:
		SetSetpoint(f_shootSetpoint);	// 420
		m_backMode = bShoot;
		break;
		
	case bCatch:
		SetSetpoint(f_catchSetpoint);	// 420
		m_backMode = bCatch;
		break;
		
	case bStart:
		SetSetpoint(f_startSetpoint);	// 610
		m_backMode = bStart;
		break;
		
	default:
		m_backMode = mode;
	}

	if (m_backMode != startMode) {
		sprintf(m_log, "Back Pickup: Mode=%s", GetModeName(mode));
		m_robotLog->LogWrite(m_log);
	}
}

void BackPickup::SetRollerMode(RollerMode mode) {							// SET ROLLER STATE
	m_rollerMode = mode;
}

void BackPickup::SetUseJoystick(bool use) {								// SET USE JOYSTICK FLAG
	m_useJoystick = use;
}

void BackPickup::StopMotors() {
	m_baseMotor->Set(0.0);
	m_rollers->Set(0.0);
}




//  ******************** PRIVATE ********************

char* BackPickup::GetModeName(BackMode mode) {
	switch(mode) {
	case bUnknown:			return "Unknown";
	case bStore:			return "Store";
	case bDeploy:			return "Deploy";
	case bWaitToDeploy:		return "WaitToDeploy";
	case bPass:				return "Pass";
	case bShoot:			return "Shoot";
	case bStart:			return "Start";
	case bCatch:			return "Catch";
	default: 				return "?";
	}
}

INT32 BackPickup::LimitValue(INT32 position){
	position = (position > f_baseMaxPosition) ? f_baseMaxPosition :
			   (position < 0) ? 0 : position;
	
	return position;
}

void BackPickup::SetSetpoint(INT32 target){								// SET ARM SETPOINT
	
	target = (target > f_baseMaxPosition) ? f_baseMaxPosition:			// Verify within limits
			 	 	 	 	 (target < 0) ? 0:
										    target;
	
	m_baseTarget = target;
	m_onTarget = false;
	m_baseMotorPID->SetSetpoint(target);								// Set PID setpoint
}

bool BackPickup::ShooterHasBall() {										// GET BALL IN SHOOTER STATUS
	return CommandBase::ballShooter->HasBall();
}

