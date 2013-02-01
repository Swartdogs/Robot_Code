// ********** Team 525 (2013) **********
//
// Drive Class Source File

#include "Drive.h"

Drive::Drive(
		UINT8 lfJagModule,		  UINT32 lfJagChannel,		//Left Front Jag
		UINT8 lrJagModule,		  UINT32 lrJagChannel,		//Left Rear Jag
		UINT8 rfJagModule,		  UINT32 rfJagChannel,		//Right Front Jag
		UINT8 rrJagModule,		  UINT32 rrJagChannel,		//Right Rear Jag
		UINT8 laEncoderModule,    UINT32 laEncoderChannel,	//Left Encoder A
		UINT8 lbEncoderModule,    UINT32 lbEncoderChannel,	//Left Encoder B
		UINT8 raEncoderModule,    UINT32 raEncoderChannel,	//Right Encoder A
		UINT8 rbEncoderModule,    UINT32 rbEncoderChannel,	//Right Encoder B
		UINT8 rotateGyroModule,   UINT32 rotateGyroChannel,	//Rotational Gyro
		float rotateDeadband,
		Events *eventHandler,	  UINT8  eventSourceId)
{
	
	m_motorLF = new Talon(lfJagModule, lfJagChannel);
	m_motorLF->SetExpiration(1.0);

	m_motorLR = new Talon(lrJagModule, lrJagChannel);
	m_motorLR->SetExpiration(1.0);
	
	m_motorRF = new Talon(rfJagModule, rfJagChannel);
	m_motorRF->SetExpiration(1.0);
	
	m_motorRR = new Talon(rrJagModule, rrJagChannel);
	m_motorRR->SetExpiration(1.0);
	
	m_lEncoder = new Encoder(laEncoderModule, laEncoderChannel, lbEncoderModule, lbEncoderChannel, true);
	m_lEncoder->SetDistancePerPulse(0.0516);
	
	m_rEncoder = new Encoder(raEncoderModule, raEncoderChannel, rbEncoderModule, rbEncoderChannel);
	m_rEncoder->SetDistancePerPulse(0.0516);
	
	m_rotateGyro = new Gyro(rotateGyroModule, rotateGyroChannel);
	m_rotateGyro->SetSensitivity(0.0069);
	
	m_rotatePID = new PIDLoop(0.075, 0.005, 0.40);
	m_rotatePID->SetInputRange(-360.0, 360.0);
	m_rotatePID->SetOutputRange(-0.6, 0.6);
	
	m_rotateDeadband = rotateDeadband;
	
	//Initialization Settings
	
	m_driveSpeed = 0;
	m_maxSpeed = 0;
	m_targetAngle = 0;
	m_targetDistance = 0;
}

Drive::~Drive(){
	delete m_motorLF;
	delete m_motorLR;
	delete m_motorRF;
	delete m_motorRR;
	delete m_lEncoder;
	delete m_rEncoder;
	delete m_rotateGyro;
	delete m_rotatePID;
}

void Drive::Disable(){
	m_motorLF->Set(0);
	m_motorLF->SetSafetyEnabled(false);
	m_motorLR->Set(0);
	m_motorLR->SetSafetyEnabled(false);
	m_motorRF->Set(0);
	m_motorRF->SetSafetyEnabled(false);
	m_motorRR->Set(0);
	m_motorRR->SetSafetyEnabled(false);
	
	m_lEncoder->Stop();
	m_rEncoder->Stop();
	
	m_rotateGyro->Reset();
	
	m_rotatePID->Reset();
}

void Drive::Enable(){
	m_motorLF->Set(0);
	m_motorLF->SetSafetyEnabled(true);
	m_motorLR->Set(0);
	m_motorLR->SetSafetyEnabled(true);
	m_motorRF->Set(0);
	m_motorRF->SetSafetyEnabled(true);
	m_motorRR->Set(0);
	m_motorRR->SetSafetyEnabled(true);
		
	m_lEncoder->Start();
	m_rEncoder->Start();
	
	m_rotateGyro->Reset();
	m_rotateGyro->GetAngle();
	m_rotateGyro->GetAngle();
	m_rotateGyro->GetAngle();
}

float Drive::GetAngle(){
	return m_rotateGyro->GetAngle();
}

double Drive::GetDistance(){
	return EncoderAverage(m_lEncoder->GetDistance(), m_rEncoder->GetDistance());
}

bool Drive::Periodic(DriveRunMode RunMode, float JoyDrive, float JoyStrafe, float JoyRotate){	
	static int			AimCount = 0;
	static double		LastDistance = 0;
	static int			NoMoveCount = 0;
	static DriveRunMode RunModeNow = dStop;
	static float		vDrive = 0;
	static float		vRotate = 0;
	static float		vStrafe = 0;
	
	double Distance;
	float  GyroAngle;
	bool   vReturn = false;
	
	if(RunModeNow != RunMode){
		RunModeNow = RunMode;
		
		switch(RunMode){
			case dStop:
				vDrive = vRotate = vStrafe = 0;
				break;
				
			case dStrafe:
				m_rotateGyro->Reset();
				m_rotatePID->Reset();
				m_rotatePID->SetSetpoint(0.0);
				break;
				
			case dAim:
			case dAutoRotate:
				AimCount = 0;
				vDrive = vStrafe = 0;
				break;
				
			case dAutoDrive:
				LastDistance = 0;
				NoMoveCount = 0;
				vDrive = vRotate = vStrafe = 0;
				break;
				
			default:;
		}
	}
	
	switch(RunMode){
		case dJoystick:
			vDrive = ApplyDeadband(JoyDrive, 0.05);
			vStrafe = ApplyDeadband(-JoyStrafe, 0.05);
			vRotate = ApplyDeadband(JoyRotate, 0.10);
			break;
			
		case dStrafe:
			vDrive = ApplyDeadband(JoyDrive, 0.05);
			vStrafe = ApplyDeadband(-JoyStrafe, 0.05);
			break;
			
		case dAim:
			GyroAngle = m_rotateGyro->GetAngle();
			vRotate = m_rotatePID->Calculate(GyroAngle);
			
			if(fabs(m_targetAngle - GyroAngle) < m_rotateDeadband){
				if(AimCount < 5) AimCount++; 
			}else{
				AimCount = 0;
			}
			
			vReturn = (AimCount >= 5);
			break;
			
		case dAutoDrive:
			if(m_targetDistance > 0){
				Distance = EncoderAverage(m_lEncoder->GetDistance(), m_rEncoder->GetDistance());
				
				if(fabs(Distance) >= fabs(m_targetDistance)){
					//sprintf(m_Log, "At Target Distance=%6.1f	(Left=%6.1f	Right=%6.1f)", Distance,
					//		  m_lEncoder->GetDistance(), m_rEncoder->GetDistance());
					//m_event->WriteToLog(m_Log);
					vRotate = 0;
					vReturn = true;
				}else if(fabs(Distance) > 5.0){
					if(fabs(Distance - LastDistance) < 0.1){
						if(NoMoveCount < 5){
							NoMoveCount++;
							if(NoMoveCount == 5){
								//sprintf(m_Log, "No Movement Detected	(Distance=%6.1f)", Distance);
								//m_event->WriteToLog(m_Log);
								vReturn = true;
							}
						}
					}else{
						NoMoveCount = 0;
					}
				}
				
				LastDistance = Distance;
			}
			
			if(!vReturn){
				if(m_driveSpeed != m_maxSpeed) m_driveSpeed = RampSpeed(m_driveSpeed, m_maxSpeed);
				vDrive = m_driveSpeed;
				
				if(m_targetAngle != - 1) vRotate = m_rotatePID->Calculate(m_rotateGyro->GetAngle());
			}
			break;
			
		case dAutoRotate:
			GyroAngle = m_rotateGyro->GetAngle();
			vRotate = m_rotatePID->Calculate(GyroAngle);
			
			if(fabs(m_targetAngle - GyroAngle) <= 1.0){
				if(AimCount < 5){
					AimCount++;
					if(AimCount == 5){
						//sprintf(m_Log, "Turn Completed: Gyro=%5.1f \n", GyroAngle);
						//m_event->WriteToLog(m_Log);
						vRotate = 0;
						vReturn = true;
					}
				}
			}else{
				AimCount = 0;
			}
			break;
			
		default:;
	}
	MecanumDrive(vDrive, vStrafe, vRotate, driveAll);
	return vReturn;
}

void Drive::ResetDriveSpeed(){
	m_driveSpeed = 0;
}

void Drive::ResetEncoders(){
	m_lEncoder->Reset();
	m_rEncoder->Reset();
}

void Drive::ResetGyro(){
	m_rotateGyro->Reset();
}

void Drive::SetAngle(float Angle){
	m_targetAngle = Angle;
	m_rotatePID->Reset();
	m_rotatePID->SetSetpoint(m_targetAngle);
}

void Drive::SetDistance(double Distance){
	m_targetDistance = Distance;
}

void Drive::SetMaxSpeed(float Speed){
	m_maxSpeed = Speed;
}

/**************************************|
 |           Private Functions		   |
 |*************************************/

float Drive::ApplyDeadband(float RawValue, float Deadband){
	RawValue = Limit(RawValue);
	
	if(fabs(RawValue) < Deadband) return 0;
	if(RawValue > 0)			  return (RawValue - Deadband) / (1.0 - Deadband);
								  return (RawValue + Deadband) / (1.0 - Deadband);
}

double Drive::EncoderAverage(double Value1, double Value2){
	if(fabs(Value1) < fabs(Value2 / 2)) return Value2;
	if(fabs(Value2) < fabs(Value1 / 2)) return Value1;
										return (Value1 + Value2) / 2;
}

float Drive::Limit(float Value){
	if(Value > 1.0)  return 1.0;
	if(Value < -1.0) return -1.0;
					 return Value;
}

float Drive::Maximum(float Value1, float Value2){
	if(Value1 > Value2) return Value1;
						return Value2;
}

void Drive::MecanumDrive(float Drive, float Strafe, float Rotate, EnumDrive DriveMode){
	int i = 0;
	float MotorAbs;
	float MotorPwm[4];
	float MotorMax = 0;
	SmartDashboard *Dash;
	
	MotorPwm[0] = Rotate + Drive + Strafe;
	MotorPwm[1] = Rotate + Drive - Strafe;
	MotorPwm[2] = Rotate - Drive + Strafe;
	MotorPwm[3] = Rotate - Drive - Strafe;
	
	for(i = 0; i < 4; i++){
		MotorAbs = fabs(MotorPwm[i]);
		if(MotorMax < MotorAbs) MotorMax = MotorAbs;
	}
	
	for(i = 0; i < 4; i++){
		if(MotorMax > 1.0) MotorPwm[i] = Limit(MotorPwm[i] / MotorMax);
	}
	
	switch(DriveMode){
		case driveAll:
			m_motorLF->Set(MotorPwm[0]);
			m_motorLR->Set(MotorPwm[1]);
			m_motorRF->Set(MotorPwm[2]);
			m_motorRR->Set(MotorPwm[3]);
			break;
		case driveRear:
			m_motorLF->Set(0);
			m_motorLR->Set(MotorPwm[1]);
			m_motorRF->Set(0);
			m_motorRR->Set(MotorPwm[3]);
			break;
		case driveFront:
			m_motorLF->Set(0);
			m_motorLR->Set(MotorPwm[1]);
			m_motorRF->Set(0);
			m_motorRR->Set(MotorPwm[3]);
			break;
	}
	
	Dash->PutNumber("Front Left", MotorPwm[0]);
	Dash->PutNumber("Rear Left", MotorPwm[1]);
	Dash->PutNumber("Front Right", MotorPwm[2]);
	Dash->PutNumber("Rear Right", MotorPwm[3]);
}

float Drive::RampSpeed(float Speed, float MaxSpeed){
	float Direction;
	
	if(MaxSpeed < 0){
		Direction = -1.0;
	}else{
		Direction = 1.0;
	}
	
	MaxSpeed = fabs(MaxSpeed);
	Speed = fabs(Speed);
	
	if(Speed == 0.0){
		if(MaxSpeed <= 0.2){
			Speed = MaxSpeed;
		}else{
			Speed = 0.2;
		}
	}else if(fabs(MaxSpeed - Speed) <= 0.015){
		Speed = MaxSpeed;
	}else if(Speed < MaxSpeed){
		Speed = Speed + 0.015;
	}else if(Speed > MaxSpeed){
		Speed = Speed - 0.015;
	}
	
	return Speed * Direction;
}

bool Drive::SameSignAndGreater(float Value1, float Value2){
	if(Value1 > 0 && Value2 < 0) return false;
	if(Value1 < 0 && Value2 > 0) return false;
	return (fabs(Value1) >= fabs(Value2));
}
