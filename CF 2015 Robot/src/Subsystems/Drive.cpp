#include "Drive.h"
#include "../RobotMap.h"
#include "../Commands/AllCommands.h"
#include <math.h>

float const c_drivePID_P 		= 0.025;					// Drive PID Coefficients
float const c_drivePID_I 		= 0.0;
float const c_drivePID_Da 		= 0.0;
float const c_drivePID_Db		= 0.3;

float const c_rotatePID_P 		= 0.04;						// Rotate PID Coefficients
float const c_rotatePID_It      = 10.0;
float const c_rotatePID_Ia 		= 0.0;
float const c_rotatePID_Ib      = 0.006;
float const c_rotatePID_D 		= 0.2;

Drive::Drive() : Subsystem("Drive") {						// Create objects and initialize variables
	m_motorLF = new Victor(PWM_DRIVE_LF);
	m_motorLR = new Victor(PWM_DRIVE_LR);
	m_motorRF = new Victor(PWM_DRIVE_RF);
	m_motorRR = new Victor(PWM_DRIVE_RR);

	m_gyro = new Gyro(AI_GYRO);
	m_gyro->SetSensitivity(0.007);

	m_leftEncoder = new Encoder(DI_ENCODER_LEFT_A, DI_ENCODER_LEFT_B, true);
	m_leftEncoder->SetDistancePerPulse(0.036);

	m_rightEncoder = new Encoder(DI_ENCODER_RIGHT_A, DI_ENCODER_RIGHT_B);
	m_rightEncoder->SetDistancePerPulse(0.036);

	m_drivePID = new PIDControl();
	SetDrivePID();

	m_rotatePID = new PIDControl();
	SetRotatePID();

	m_onTarget = m_rampDone = false;

	m_drivePWM = 0;
	m_lastDistance = 0;
	m_markAngle = 0;
	m_maxPWM = 0;
	m_targetAngle = 0;
	m_targetDistance = 0;
}

Drive::~Drive() {
	delete m_motorLF;
	delete m_motorLR;
	delete m_motorRF;
	delete m_motorRR;
	delete m_gyro;
	delete m_leftEncoder;
	delete m_rightEncoder;
	delete m_drivePID;
	delete m_rotatePID;
}

void Drive::DriveWithJoystick(float drive, float rotate) {
	ArcadeDrive (drive, rotate);
}

void Drive::DriveWithJoystick(float drive, float strafe, float rotate, DriveWheels driveWheels) {
	MecanumDrive(drive, strafe, rotate, driveWheels);
}

void Drive::ExecuteDistance(bool showPID) {
	float curDistance = EncoderDistance(m_leftEncoder->GetDistance(), m_rightEncoder->GetDistance());

	if (m_rampDone){										// Use PID speed
		m_drivePWM = m_drivePID->Calculate(curDistance, showPID);
	} else {												// Use Ramp speed until >= PID speed
		m_rampDone = RampPWM(m_drivePWM, m_drivePID->Calculate(curDistance, showPID));
	}

	float error = RotateError();							// Use Rotate PID to maintain heading
	float rotatePWM = m_rotatePID->Calculate(error, false);
	curDistance = fabs(curDistance);

	if (curDistance > fabs(m_targetDistance / 2)) {			// More than half way to target
		if (curDistance - m_lastDistance <= 0) {			// Robot has stopped of is moving in opposite direction
			m_drivePWM = 0;

			if (fabs(error) <= 1.0 ) {						// Rotation is within 1 degree of heading
				rotatePWM = 0;
				m_onTarget = true;
				sprintf(m_log, "Drive:   Drive Done: Distance=%6.1f  Angle=%5.1f", curDistance, GetGyroAngle());
				MyRobot::robotLog->Write(m_log);
			}
		}
	}

	m_lastDistance = curDistance;							// Save current distance for next execution

	if (GetDriveMode() == mArcade){
		ArcadeDrive(m_drivePWM, rotatePWM);
	} else {
		MecanumDrive(m_drivePWM, 0, rotatePWM, wAll);
	}
}

void Drive::ExecuteRotate(bool showPID){
	static int counter = 5;
	float error = RotateError();							// Difference between current angle and target
	float rotatePWM = m_rotatePID->Calculate(error, showPID);		// Get rotate speed from PID

	if (fabs(error) <= 1.0 ) {								// Rotation with 1 degree of target
		if (counter > 0){
			counter--;
		} else {											// Within 1 degree of target for at least 100 ms
			rotatePWM = 0;
			if(!m_onTarget) {
				m_onTarget = true;
				sprintf(m_log, "Drive:   Rotate Done: Angle=%5.1f", GetGyroAngle());
				MyRobot::robotLog->Write(m_log);
			}
		}
	} else {
		counter = 5;
	}

	if (GetDriveMode() == mArcade){
		ArcadeDrive(0, rotatePWM);
	} else {
		MecanumDrive(0, 0, rotatePWM, wAll);
	}
}

Drive::DriveMode Drive::GetDriveMode(){
	return mArcade;
}

double Drive::GetEncoderDistance(WhichEncoder which){
	return which == eRight ? m_rightEncoder->GetDistance() :
							 m_leftEncoder->GetDistance();
}

float Drive::GetGyroAngle() {								// Determine angle of robot in range of 0 to 360 degrees
	float angle = ((int)(m_gyro->GetAngle() * 10) % 3600) / 10.0;
	if (angle < 0) angle += 360;
	return angle;
}

void Drive::InitDefaultCommand()
{
	SetDefaultCommand(new DriveJoystick());
}

void Drive::InitDistance(double distance, float maxPWM, bool resetEncoders, bool useBrake, float angle, AngleFrom angleFrom) {
	m_targetDistance = distance;
	m_maxPWM = maxPWM;

	m_drivePID->Reset();									// Configure Drive PID
	m_drivePID->SetSetpoint(m_targetDistance);
	m_drivePID->SetOutputRange(-m_maxPWM, m_maxPWM);

	if(resetEncoders) {
		m_leftEncoder->Reset();								// Reset encoder distance measurements to 0
		m_rightEncoder->Reset();
	}

	if (useBrake) {
		m_drivePID->SetCoefficient('D', (float)(m_targetDistance * 0.15), c_drivePID_Da, c_drivePID_Db);
	} else {
		m_drivePID->SetCoefficient('D', 0, c_drivePID_Da, c_drivePID_Db);
	}

	if (angleFrom == fNewMark) {							// Determine heading and reset Rotate PID
		m_markAngle = GetGyroAngle();
		angle += m_markAngle;
	} else if (angleFrom == fMark) {
		angle += m_markAngle;
	}

	m_targetAngle = ((int)(angle * 10) % 3600) / 10.0;		// Limit heading to range of 0 to 360 degrees
	if (m_targetAngle < 0) m_targetAngle += 360;

	m_rotatePID->Reset();

	m_drivePWM = 0;
	m_lastDistance = 0;
	m_onTarget = m_rampDone = false;

	sprintf(m_log, "Drive:   Initiate Drive: Distance=%6.1f  Heading=%5.1f", m_targetDistance, m_targetAngle);
	MyRobot::robotLog->Write(m_log);
}

void Drive::InitRotate(float angle, AngleFrom angleFrom) {
	if (angleFrom == fNewMark) {							// Determine angle relative to robot start position
		m_markAngle = GetGyroAngle();
		angle += m_markAngle;
	} else if (angleFrom == fMark) {
		angle += m_markAngle;
	}

	m_targetAngle = ((int)(angle * 10) % 3600) / 10.0;		// Limit angle to range of 0 to 360 degrees
	if (m_targetAngle < 0) m_targetAngle += 360;

	m_rotatePID->Reset();									// Reset Rotate PID
	m_onTarget = false;

	sprintf(m_log, "Drive:   Initiate Rotate: From=%5.1f  To=%5.1f", GetGyroAngle(), m_targetAngle);
	MyRobot::robotLog->Write(m_log);
}

bool Drive::IsOnTarget() {									// Drive and/or Rotate positions are at target
	return m_onTarget;
}

void Drive::SetConstant(std::string key, int32_t value) {

}
void Drive::SetDrivePID() {
	m_drivePID->SetCoefficient('P', 0, c_drivePID_P, 0);
	m_drivePID->SetCoefficient('I', 0, c_drivePID_I, 0);
	m_drivePID->SetCoefficient('D', 0, c_drivePID_Da, c_drivePID_Db);

	m_drivePID->SetInputRange(-200, 200);
	m_drivePID->SetOutputRange(-0.6, 0.6);
}

void Drive::SetRotatePID() {
	m_rotatePID->SetCoefficient('P', 0, c_rotatePID_P, 0);
	m_rotatePID->SetCoefficient('I', c_rotatePID_It, c_rotatePID_Ia, c_rotatePID_Ib);
	m_rotatePID->SetCoefficient('D', 0, c_rotatePID_D, 0);

	m_rotatePID->SetInputRange(-360.0, 360.0);
	m_rotatePID->SetOutputRange(-0.7, 0.7);
	m_rotatePID->SetSetpoint(0);
}

void Drive::TuneDrivePID() {
	m_drivePID->SetCoefficient('P', (float)MyRobot::dashboard->GetDashValue(DV_PID_P_THRESHOLD),
									(float)MyRobot::dashboard->GetDashValue(DV_PID_P_ABOVE) / 1000,
									(float)MyRobot::dashboard->GetDashValue(DV_PID_P_BELOW) / 1000);
	m_drivePID->SetCoefficient('I', (float)MyRobot::dashboard->GetDashValue(DV_PID_I_THRESHOLD),
									(float)MyRobot::dashboard->GetDashValue(DV_PID_I_ABOVE) / 1000,
									(float)MyRobot::dashboard->GetDashValue(DV_PID_I_BELOW) / 1000);
	m_drivePID->SetCoefficient('D', (float)MyRobot::dashboard->GetDashValue(DV_PID_D_THRESHOLD),
									(float)MyRobot::dashboard->GetDashValue(DV_PID_D_ABOVE) / 1000,
									(float)MyRobot::dashboard->GetDashValue(DV_PID_D_BELOW) / 1000);

	InitDistance((double)MyRobot::dashboard->GetDashValue(DV_PID_SETPOINT),
				 (float)MyRobot::dashboard->GetDashValue(DV_PID_MAX_PWM) / 100, true, true, 0, Drive::fNewMark);
}

void Drive::TuneRotatePID() {
	m_rotatePID->SetCoefficient('P', (float)MyRobot::dashboard->GetDashValue(DV_PID_P_THRESHOLD),
									 (float)MyRobot::dashboard->GetDashValue(DV_PID_P_ABOVE) / 1000,
									 (float)MyRobot::dashboard->GetDashValue(DV_PID_P_BELOW) / 1000);
	m_rotatePID->SetCoefficient('I', (float)MyRobot::dashboard->GetDashValue(DV_PID_I_THRESHOLD),
									 (float)MyRobot::dashboard->GetDashValue(DV_PID_I_ABOVE) / 1000,
									 (float)MyRobot::dashboard->GetDashValue(DV_PID_I_BELOW) / 1000);
	m_rotatePID->SetCoefficient('D', (float)MyRobot::dashboard->GetDashValue(DV_PID_D_THRESHOLD),
									 (float)MyRobot::dashboard->GetDashValue(DV_PID_D_ABOVE) / 1000,
									 (float)MyRobot::dashboard->GetDashValue(DV_PID_D_BELOW) / 1000);

	float maxPWM = (float)MyRobot::dashboard->GetDashValue(DV_PID_MAX_PWM) / 100;
	m_rotatePID->SetOutputRange(-maxPWM, maxPWM);

	InitRotate((float)MyRobot::dashboard->GetDashValue(DV_PID_SETPOINT), fNewMark);
}

void Drive::StopMotors() {									// Stop drive motors
	m_motorLF->Set(0);
	m_motorLR->Set(0);
	m_motorRF->Set(0);
	m_motorRR->Set(0);
}

// ******************** PRIVATE ********************

void Drive::ArcadeDrive (float drive, float rotate) {
	//    drive
	//		|			Joystick Quadrants
	//  1	|	2
	//		|
	// ------------ rotate
	//		|
	//	3	|	4
	//		|

	float leftPwm;
	float rightPwm;

	if (drive > 0.0) {										// Quadrant 1 or 2
		if(rotate > 0.0) { 									// Quadrant 2
			leftPwm = drive - rotate;
			rightPwm = (drive > rotate ? drive : rotate);
		} else { 											// Quadrant 1 or drive axis
			leftPwm = (drive > -rotate ? drive : -rotate);
			rightPwm = drive + rotate;
		}

	} else {												// Quadrant 3, 4, or rotate axis
		if(rotate > 0.0) { 									// Quadrant 4
			leftPwm = -(-drive > rotate ? -drive: rotate);
			rightPwm = drive + rotate;
		} else { 											// Quadrant 3 or drive axis
			leftPwm = drive - rotate;
			rightPwm = -(-drive > -rotate ? -drive : -rotate);
		}
	}

	m_motorLF->Set(leftPwm);
	m_motorLR->Set(leftPwm);
	m_motorRF->Set(-rightPwm);
	m_motorRR->Set(-rightPwm);

	//printf("Drive=%f  Rotate=%f Left=%f Right=%f\n", drive, rotate, leftPwm, -rightPwm);
}

double Drive::EncoderDistance(double value1, double value2) {	// Return encoder distance with the greatest error from target
	if (fabs(m_targetDistance - value1) > fabs(m_targetDistance - value2)) return value1;
	return value2;
}

float Drive::Limit(float Value){							// Limit Value to range of -1.0 to +1.0
	if(Value > 1.0)  return 1.0;
	if(Value < -1.0) return -1.0;
					 return Value;
}

void Drive::MecanumDrive(float drive, float strafe, float rotate, DriveWheels driveWheels){
	float MotorAbs;
	float MotorPwm[4];
	float MotorMax = 0;

	MotorPwm[0] = rotate + drive + strafe;
	MotorPwm[1] = rotate + drive - strafe;
	MotorPwm[2] = rotate - drive + strafe;
	MotorPwm[3] = rotate - drive - strafe;

	for(int i = 0; i < 4; i++) {
		MotorAbs = fabs(MotorPwm[i]);
		if(MotorMax < MotorAbs) MotorMax = MotorAbs;
	}

	for(int i = 0; i < 4; i++) {
		if(MotorMax > 1.0) MotorPwm[i] = Limit(MotorPwm[i] / MotorMax);
	}

	switch(driveWheels) {
		case wAll:
			m_motorLF->Set(MotorPwm[0]);
			m_motorLR->Set(MotorPwm[1]);
			m_motorRF->Set(MotorPwm[2]);
			m_motorRR->Set(MotorPwm[3]);
			break;
		case wRear:
			m_motorLF->Set(0);
			m_motorLR->Set(MotorPwm[1]);
			m_motorRF->Set(0);
			m_motorRR->Set(MotorPwm[3]);
			break;
		case wFront:
			m_motorLF->Set(MotorPwm[0]);
			m_motorLR->Set(0);
			m_motorRF->Set(MotorPwm[2]);
			m_motorRR->Set(0);
			break;
		default:;
	}

	//printf("LF = %f  LR = %f  RF = %f  RR = %f\n", MotorPwm[0], MotorPwm[1], MotorPwm[2], MotorPwm[3]);
}

bool Drive::RampPWM(float& curPWM, float pidPWM) {			// Ramp current PWM until >= to PID PWM
	float	direction;
	float	myPWM;
	bool	vReturn = false;

	direction = (pidPWM < 0 ? -1.0 : 1.0);

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

	return vReturn;											// Return True when current PWM >= PID PWM
}

float Drive::RotateError() {								// Calculate rotation error in -180 to +180 range
	float error = m_targetAngle - GetGyroAngle();

	if (error > 180) {
		error -= 360;
	} else if (error < -180) {
		error += 360;
	}

	return error;
}








