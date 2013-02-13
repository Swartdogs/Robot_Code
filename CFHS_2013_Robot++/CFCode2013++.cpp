//ButtonBox:
//	1 - Toggle Switch for Tilt Joystick (false = Hopper, true = Shooter)
//  2 - Move Hopper to feeder position
//  3 - Move Hopper to drive position
//  4 - Move Hopper to pyramid position
//  5 - Move Shooter bed to long position
//  6 - Move Shooter bed to short position
//  7 - Move Shooter bed to flop position 
//  8 - Start Shoot Sequence
//  9 - Move Hopper to Load Position
//  10- 
//
//Tilt Joystick Buttons:
//	1 - 
//	2 - 
//  3 - 
//  4 - 
//	5 -
//	6 - 
//	7 - 
//  8 - Cancel Shoot (w/ button 9)
//	9 - Cancel Shoot (w/ button 8)

#include "WPILib.h"
#include "Events.h"
#include "FindGoals.h"
#include "Hopper.h"
#include "DiskPickup.h"
#include "Drive.h"
#include "DiskShooter.h"

enum EnumAction{actionNone, actionDrive, actionTurn, actionShoot, actionShootCamera, actionFrisbeePickup, actionDumpFrisbees};
enum EnumShootSeq{sIdle, sMoveHopper, sLoad, sReady, sFire};

typedef struct {									// Autonomous
	bool        InitStep;							//    Initialize Step
	int         StartDelay;							//    Delay Count prior to beginning of Sequence
	int         StepCount;							//    Number of Steps in Sequence
	int         StepIndex;							//    Current Step
} StructAuto;

typedef struct {									// Autonomous Sequence Step Data Structure
	EnumAction  Action;								//    Action performed by step	
	float       MaxSpeed;							//    Maximum allowed speed			(Drive)
	double      Distance;							//    Maximum distance				(Drive)
	float       GyroTarget;							//    Gyro target in degrees  		(Drive or Turn)
	INT16 		Timeout;							//    Failsafe Step Timeout			(All)
	bool 		ResetGyro;							//    Reset Gyro to 0				(Drive or Turn)
	bool 		ResetEncoders;						//    Reset Encoder				    (Drive)
	bool 		StopRobot;							//    Stop robot (Speed = 0)		(Drive or Turn)
	bool		PickupFrisbees;
} StructAutoStep;


class Team525: public IterativeRobot, public Events
{

	SmartDashboard *m_dash;
	Drive	  	   *m_drive;
	DiskShooter	   *m_shooter;
	DiskPickup	   *m_pickup;
	FindGoals 	   *m_findGoals;
	Hopper         *m_hopper;
	Joystick  	   *m_driveJoystick;
	Joystick       *m_tiltJoystick;
	Joystick	   *m_buttonBox;
	Solenoid  	   *m_sensorPower;
	StructAuto 		m_auto;
	StructAutoStep *m_autoStep;
	FILE		   *m_logFile;
	bool			m_cameraShot;
	
	char	   		m_autoModeId0;
	char	   		m_autoModeId1;
	char            m_autoModeId2;

	EnumShootSeq    m_shootSeq;
	double	   	    m_periodBeginTime;
	INT32	   		m_periodicCount;
	double	   		m_lastPeriodStart;
	double	   		m_lastPeriodicEnd;
	char            m_log[100];						// Buffer for Log Messages
	
public:

	Team525(void)	{
		
		m_drive = new Drive(1, 1,					// Left Front Motor 	DM 1: PWM 1
							1, 2,					// Left Rear Motor 		DM 1: PWM 2
							1, 3,					// Right Front Motor 	DM 1: PWM 3
							1, 4, 					// Right Rear Motor 	DM 1: PWM 4
							1, 1, 					// Left Encoder A 		DM 1: Digital 1
							1, 2,					// Left Encoder B 		DM 1: Digital 2
							1, 3,					// Right Encoder A 		DM 1: Digital 3
							1, 4,					// Right Encoder B 		DM 1: Digital 4
							1, 1,					// Rotational Gyro 		AM 1: Analog 1
							this, 1);
		
		m_findGoals = new FindGoals();
		
		m_sensorPower = new Solenoid(1, 8);			// Sensor Power         SM 1: Solenoid 8
		
		m_pickup = new DiskPickup(1, 8, 			// Pickup Motor 		DM 1: PWM: 8
							      1, 9, 			// Flip Motor 			DM 1: PWM: 9
							      1, 7,		 		// Disk Sensor  		DM 1: Digital 7
							      1, 6, 			// Flip Pot 			AM 1: Analog 6
							      this, 2);
		
		m_driveJoystick = new Joystick(1);
		m_tiltJoystick = new Joystick(2);
		m_buttonBox = new Joystick(3);
		
		m_shooter = new DiskShooter(1, 7,			// Shoot Motor 			DM 1: PWM 7
									1, 6,			// Tilt Motor 			DM 1: PWM 6
									1, 2,			// Tension Motor 	    DM 1: Relay 2
									1, 4,			// Shoot Pot 			AM 1: Analog 4
									1, 3,			// Tilt Pot 			AM 1: Analog 3
									1, 5,			// Tension Pot 			AM 1: Analog 5
									1, 5,			// Disk Sensor 			DM 1: Digital 5
									this, 3);	
		
		m_hopper = new Hopper(1, 1,   				// Shoot Gate Motor	    DM 1: Relay 1
						 	  1, 5,  				// Tilt Motor		    DM 1: PWM 5
						 	  1, 2,  			 	// Tilt Pot             AM 1: Analog 2
						 	  1, 8,   				// Before Sensor        DM 1: Digital 8
						 	  1, 6,                 // After Sensor         DM 1: Digital 6  
						 	  this, 4);
		
		Team525::SetPeriod(0.02);
		
		m_logFile = fopen("Log525.txt", "a");
		
		m_periodicCount = -1;
		WriteToLog("");
		WriteToLog("525 2013 Constructor");
		
		printf("525 2013 Constructor \n");
	}
	
	void RobotInit(void) {
		
		FILE *BootFile;
		int BootCount = 0;
		
		GetWatchdog().SetEnabled(false);
		
		m_autoModeId0 = 0;
		m_autoModeId1 = 1;
		m_autoModeId2 = 2;
		
		BootFile = fopen("Boot525.txt", "rb");
			if (!feof(BootFile)) fread(&BootCount, sizeof(int), 1, BootFile);
		fclose(BootFile);
		
		BootCount++;

		BootFile = fopen("Boot525.txt", "wb");
			fwrite (&BootCount, sizeof(int), 1, BootFile);
		fclose(BootFile);
		
		WriteToLog("525 2013 Robot Init (Build 1)");
		sprintf(m_log, "525 2013 Robot Boot %d", BootCount);
		WriteToLog(m_log);
		
		printf("525 2013 Robot Init \n");
	}
	
/**************************************** DISABLED *******************************************/

	void DisabledInit(void) {
		
		m_sensorPower->Set(false);
		m_periodicCount = -1;
		
		m_drive->Disable();
		m_hopper->Disable();
		m_shooter->Disable();
		
		WriteToLog("525 2013 Disabled Init");
		fclose(m_logFile);
		
		printf("525 2013 Disabled Init \n");
	}

	void DisabledPeriodic(void)  {
		
		static int CameraStartDelay = 250;
		
		if (CameraStartDelay > 0) {							// Start Camera after delay
			CameraStartDelay--;								// (Waiting for completion of camera bootup)
			if (CameraStartDelay == 0) {
				//m_findGoals->StartCamera("10.5.25.9");		// Start Camera
				//fprintf(m_logFile, "Start Camera \n");
			}
		}
	}

/*************************************** AUTONOMOUS ******************************************/

	void AutonomousInit(void) {
		
		int AutoDelay;
		char AutoMode;
		
		m_periodicCount = 0;
		m_lastPeriodStart = GetClock() * 1000;
		m_lastPeriodicEnd = m_lastPeriodStart;
		m_periodBeginTime = m_lastPeriodStart;
		
		AutoDelay = 0;
		m_auto.StartDelay = AutoDelay / 20;
		
		AutoStepLoad(AutoMode);
		m_auto.StepIndex = 0;
		m_auto.InitStep = true;
		
		m_sensorPower->Set(true);
		m_drive->Enable();
		m_hopper->Enable();
		m_shooter->Enable();
		
		m_logFile = fopen("Log525.txt", "a");
		
		sprintf(m_log, "Autonomous Init: AutoMode=%d  Delay=%d", AutoMode, AutoDelay);
		WriteToLog("m_log");
		
		printf("525 2013 Auto Init: Mode=%d  Delay=%d \n", AutoMode, AutoDelay);
	}

	void AutonomousPeriodic(void) {
		
		static bool	AimReady = false;
		static bool	AutoStepDone = false;
		static bool DumpFrisbees = false;
		static bool FrisbeeLoadEnabled = false;
//		static int	GoalFound = 0;
//		static char	StartFrisbeeCount = 0;
		static int  autoHopperFlags = 0;
		static int  autoShooterFlags = 0;
		
		double TimeNow;
		
		TimeNow = GetClock() * 1000;
		
		if((TimeNow - m_lastPeriodStart) > 100){
			fprintf(m_logFile, "%5d:	Loop Delay	Start=%f  End=%f \n", m_periodicCount, TimeNow - m_lastPeriodStart,
					TimeNow - m_lastPeriodicEnd);
			printf("Loop Delay:  Start=%f  End=%f \n", TimeNow - m_lastPeriodStart, TimeNow - m_lastPeriodicEnd);
		}
		
		m_lastPeriodStart = TimeNow;
		
		if(m_auto.StartDelay > 0){
			m_auto.StartDelay--;
			m_drive->Periodic(Drive::dStop, 0, 0, 0);
			m_pickup->FeedSafety();
			
			m_periodicCount++;
			return;
		}
		
		if(m_auto.StepIndex >= m_auto.StepCount){
			m_drive->Periodic(Drive::dStop, 0, 0, 0);
			m_pickup->FeedSafety();
			
			m_periodicCount++;
			return;
		}
		
		// ------------------------- INITIALIZE NEW STEP -------------------------------------------

		if(m_auto.InitStep){
			m_auto.InitStep = false;
			AutoStepDone = false;
			
			AimReady = false;
			FrisbeeLoadEnabled = m_autoStep[m_auto.StepIndex].PickupFrisbees;
			m_cameraShot = false;
			DumpFrisbees = false;
			
			m_drive->SetAngle(m_autoStep[m_auto.StepIndex].GyroTarget);
			m_drive->SetDistance(m_autoStep[m_auto.StepIndex].Distance);
			m_drive->SetMaxSpeed(m_autoStep[m_auto.StepIndex].MaxSpeed);
			
			if(m_autoStep[m_auto.StepIndex].ResetGyro) m_drive->ResetGyro();
			if(m_autoStep[m_auto.StepIndex].ResetEncoders) m_drive->ResetEncoders();
			if(m_autoStep[m_auto.StepIndex].Timeout <= 0) m_autoStep[m_auto.StepIndex].Timeout = -1;
			
			switch(m_autoStep[m_auto.StepIndex].Action){
				case actionShoot:
						
					break;
					
				case actionShootCamera:
					
					break;
					
				case actionFrisbeePickup:
					
					break;
					
				case actionDumpFrisbees:
					
					break;
					
				case actionDrive:
				case actionTurn:
					
					break;
					
				default:;
			}
			
			fprintf(m_logFile, "%5d: Step %d  Step Action=%d  Gyro=%5.1f \n", m_periodicCount, m_auto.StepIndex,
					m_autoStep[m_auto.StepIndex].Action, m_drive->GetAngle());
		}

		//------------------------- EXECUTE STEP --------------------------------------------------
		
		if(m_autoStep[m_auto.StepIndex].Timeout > 0) m_autoStep[m_auto.StepIndex].Timeout--;
		
		if(m_autoStep[m_auto.StepIndex].Timeout == 0){
			if(m_autoStep[m_auto.StepIndex].Distance != 0){
				fprintf(m_logFile, "%5d: Step Timeout  Distance=%6.1f \n", m_periodicCount, m_drive->GetDistance());
			}else{
				fprintf(m_logFile, "%5d: Step Timeout \n", m_periodicCount);
			}
			
			AutoStepDone = true;
		}else{
			switch(m_autoStep[m_auto.StepIndex].Action){
				case actionDrive:
					AutoStepDone = m_drive->Periodic(Drive::dAutoDrive, 0, 0, 0);
					break;
					
				case actionTurn:
					AutoStepDone = m_drive->Periodic(Drive::dAutoRotate, 0, 0, 0);
					break;
					
				case actionShoot:
					m_drive->Periodic(Drive::dStop, 0, 0, 0);
					m_shooter->SetTiltTarget(50);
					m_shooter->SetTensionTarget(400);
					autoShooterFlags = m_shooter->Periodic(0);
					autoHopperFlags = m_hopper->Periodic(0);
					break;
				case actionShootCamera:
					
					break;
					
				case actionFrisbeePickup:
					m_drive->Periodic(Drive::dStop, 0, 0, 0);
					//ADD STUFF!!!!!!!!!!!!!!!!!!!!!!!!!!!
					break;
					
				case actionDumpFrisbees:
					m_drive->Periodic(Drive::dStop, 0, 0, 0);
					//ADD STUFF!!!!!!!!!!!!!!!!!!!!!!!!!!!
					break;
					
				default:
					AutoStepDone = true;
			}
		}
		
		if(AutoStepDone){
			if(m_autoStep[m_auto.StepIndex].StopRobot){
				m_drive->Periodic(Drive::dStop, 0, 0, 0);
				m_drive->ResetDriveSpeed();
			}else{
				m_drive->Periodic(Drive::dCoast, 0, 0, 0);
			}
			
			fprintf(m_logFile, "%5d: Step %d Done  Time = %5.0f \n", m_periodicCount, m_auto.StepIndex, TimeNow - m_periodBeginTime);
			
			m_auto.InitStep = true;
			FrisbeeLoadEnabled = false;
			DumpFrisbees = false;
			
			m_auto.StepIndex++;
			
			if(m_auto.StepIndex >= m_auto.StepCount){
				fprintf(m_logFile, "%5d:  Autonomous Completed  Time=%5.0f \n", m_periodicCount, TimeNow - m_periodBeginTime);
			}
		}
	
		m_periodicCount++;
		m_lastPeriodicEnd = GetClock() * 1000;
	
	}

	void AutoStepInit(int NumberOfSteps){
		
		m_auto.StepCount = NumberOfSteps;
		m_autoStep = new StructAutoStep[m_auto.StepCount];
		
		for(int i = 0; i < m_auto.StepCount; i++){
			m_autoStep[i].Action = actionNone;
			m_autoStep[i].MaxSpeed = 0;
			m_autoStep[i].Distance = 0;
			m_autoStep[i].GyroTarget = -1;
			m_autoStep[i].Timeout = 0;
			m_autoStep[i].ResetGyro = false;
			m_autoStep[i].ResetEncoders = false;
			m_autoStep[i].StopRobot = false;
		}
	}
	
	void AutoStepLoad(char AutoSwitch){
		
		switch(AutoSwitch){
			case 1:
				AutoStepInit(4);
				
				m_autoStep[0].Action = actionShoot;
				m_autoStep[0].ResetGyro = true;
				
				m_autoStep[1].Action = actionFrisbeePickup;
				
				m_autoStep[2].Action = actionShoot;
				
				m_autoStep[3].Action = actionFrisbeePickup;
			break;
			
			case 2:
				AutoStepInit(4);
				
				m_autoStep[0].Action = actionDrive;
				
				m_autoStep[1].Action = actionTurn;
				
				m_autoStep[2].Action = actionShoot;
				m_autoStep[2].ResetGyro = true;
				
				m_autoStep[3].Action = actionFrisbeePickup;
			break;
			case 3:
			
			break;
			case 4:
			
			break;
			case 5:
				
			break;
			case 6:
				
			break;
			case 7:

			break;	
			default:
				m_auto.StepCount = 0;
		}
	}

/***************************************** TELEOP ********************************************/

	void TeleopInit(void) {
		
		m_sensorPower->Set(true);
		
		m_hopper->Enable();
		m_shooter->Enable();
		m_drive->Enable();
		m_shootSeq = sIdle;
		
		m_logFile = fopen("Log525.txt", "a");
		WriteToLog("525 Teleop Init");
		
		printf("Teleop Init \n");
	}

	void TeleopPeriodic(void) {
		
		static int          hopperFlags = 0;
		static int          shooterFlags = 0;
		
//		float  tiltValue = -m_tiltJoystick->GetY();
		double TimeNow;
				
		TimeNow = GetClock() * 1000;
		
		if((TimeNow - m_lastPeriodStart) > 100){
			fprintf(m_logFile, "%5d:	Loop Delay	Start=%f  End=%f \n", m_periodicCount, TimeNow - m_lastPeriodStart,
					TimeNow - m_lastPeriodicEnd);
			printf("Loop Delay:  Start=%f  End=%f \n", TimeNow - m_lastPeriodStart, TimeNow - m_lastPeriodicEnd);
		}
		
		m_lastPeriodStart = TimeNow;
		
		//--------------------------------Drive Stuff-------------------------------------------------------
		
//		if(m_shootSeq != sFire){
//			if(m_driveJoystick->GetRawButton(3) || m_driveJoystick->GetRawButton(4)){
//				m_drive->Periodic(Drive::dStrafe, m_driveJoystick->GetY(), m_driveJoystick->GetX(), 0);
//			}else{
//				m_drive->Periodic(Drive::dJoystick, m_driveJoystick->GetY(), m_driveJoystick->GetX(), m_driveJoystick->GetZ());
//			}
//		}
		
		m_drive->Periodic(Drive::dJoystick, 0, 0, 0);
		
		//--------------------------------Hopper Stuff------------------------------------------------------
		
		if(m_shootSeq == sIdle){
			if(m_buttonBox->GetRawButton(8) && shooterFlags >= 3 && (hopperFlags & 4) == 4){		// Shoot button pressed, Shooter Tilt and Tension completed 
				if((shooterFlags & 16) == 16){
					m_shootSeq = sFire;
				}else{
					m_shootSeq = sMoveHopper;
					READYTHEHOPPER();
				}
				
			} else if(m_buttonBox->GetRawButton(2)) {					
				m_hopper->SetTiltTarget(50);							// Feeder load position
			} else if (m_buttonBox->GetRawButton(3)) {
				m_hopper->SetTiltTarget(25);							// Position for driving 
			} else if (m_buttonBox->GetRawButton(4)) {
				m_hopper->SetTiltTarget(0);								// Position to get under the pyramid
			} else if (m_buttonBox->GetRawButton(9)){
				READYTHEHOPPER();
			}
		}

		//--------------------------------Shooter Stuff------------------------------------------------------	
		
		if(m_shootSeq == sIdle){
			if(m_buttonBox->GetRawButton(5)) {
				m_shooter->SetTiltTarget(50);							// Long shot mode
				m_shooter->SetTensionTarget(400);
			} else if (m_buttonBox->GetRawButton(6)) {
				m_shooter->SetTiltTarget(300);							// Short shot mode
				m_shooter->SetTensionTarget(250);
			} else if (m_buttonBox->GetRawButton(7)) {
				m_shooter->SetTiltTarget(600);							// Pyramid flop mode
				m_shooter->SetTensionTarget(100);
			}

		} else {
			if(m_tiltJoystick->GetRawButton(8) && m_tiltJoystick->GetRawButton(9)){
				m_shootSeq = sIdle;
			}
		}
		
		//--------------------------------Call Periodic Functions--------------------------------------------	
		
		float  tiltValue = -m_driveJoystick->GetY();

		if(fabs(tiltValue) > 0.1 && m_shootSeq == sIdle){				// Use Tilt Joystick		
			if (m_driveJoystick->GetRawButton(3)) {
//			if(m_buttonBox->GetRawButton(1)){							// Manual Shooter Tilt
				hopperFlags = m_hopper->Periodic(0);
				shooterFlags = m_shooter->Periodic(tiltValue);
			} else if (m_driveJoystick->GetRawButton(4)) {	
//			}else{														// Manual Hopper Tilt
				hopperFlags = m_hopper->Periodic(tiltValue);
				shooterFlags = m_shooter->Periodic(0);
			} else {
				hopperFlags = m_hopper->Periodic(0);	
				shooterFlags = m_shooter->Periodic(0);
			}
			
		}else{															// Ignore Joystick
			hopperFlags = m_hopper->Periodic(0);	
			shooterFlags = m_shooter->Periodic(0);
		}
		
		//--------------------------------Frisbee Shoot Sequence---------------------------------------------	
		
		static bool printDone = false;
		
		if (m_driveJoystick->GetRawButton(9)) {
			if (!printDone) {
				printf("Hopper Flags=%d  Shooter: Flags=%d  Position=%d  Seq=%d \n", hopperFlags, shooterFlags, m_shooter->GetShooterPosition(), m_shootSeq);
				printDone = true;
			}
		} else {
			printDone = false;
		}
		
		if (m_driveJoystick->GetRawButton(7)) {
			if ((hopperFlags & 2) == 2) {
				m_hopper->RELEASETHEFRISBEE();
			}
		}
		
		
		if (m_driveJoystick->GetRawButton(11)) {
			if (m_shootSeq == sIdle  && shooterFlags >= 3 && (hopperFlags & 2) == 2){
				m_shootSeq = sLoad;
				m_shooter->Load();
			}	
		} else if (m_driveJoystick->GetRawButton(12)) {
			printf("Shoot: Seq=%d  shooterFlags=%d  Position=%d \n", m_shootSeq, shooterFlags, m_shooter->GetShooterPosition());
			if (m_shootSeq == sReady && shooterFlags == 31) {
				m_shootSeq = sFire;
				m_shooter->FIREINTHEHOLE();
			}
		}
		
		switch(m_shootSeq) {
			case sMoveHopper:
				if ((hopperFlags & 1) == 1) {							// Hopper in position
					m_shootSeq = sLoad;
					m_shooter->Load();									// Initiate Shooter Arm movement
				}
				break;

			case sLoad:
				if ((shooterFlags & 4) == 4 ) {							// Shooter arm ready for load
					m_hopper->RELEASETHEFRISBEE();						// Load Frisbee
					m_shootSeq = sReady;
				}
				
//				if (shooterFlags == 31) {								// Shooter loaded and Ready
//					m_shootSeq = sFire;
//					m_shooter->FIREINTHEHOLE();							// Fire Frisbee
//				}
				break;
				
			case sFire:
				if ((shooterFlags & 16) == 0) m_shootSeq = sIdle;			// No Frisbee in Shooter
			default:;
		}
		
		m_periodicCount++;
		m_lastPeriodicEnd = GetClock() * 1000;
	}

	
	
/****************************************** TEST *********************************************/

	void TestInit(void){
		
		m_sensorPower->Set(true);
		m_drive->Enable();
		
		printf("Test Init");
	}
	
	void TestPeriodic(void){
		
		static int  GoalFound = 0;
		static bool buttonPressed = false;
		
		m_hopper->Periodic(0);
		m_drive->Periodic(Drive::dJoystick, m_driveJoystick->GetY(), m_driveJoystick->GetX(), m_driveJoystick->GetZ());
		
		if(m_driveJoystick->GetRawButton(11)){
			if(!buttonPressed){
				GoalFound = m_findGoals->Find(0);
				buttonPressed = true;
				printf("Looking For Wall Targets\n");
			}
		}else if(m_driveJoystick->GetRawButton(12)){
			if(!buttonPressed){
				GoalFound = m_findGoals->Find(1);
				buttonPressed = true;
				printf("Looking For Pyramid Goal\n");
			}
		}else if(GoalFound != -1){
			buttonPressed = false;
		}
		
		if(GoalFound < 0){
			GoalFound = m_findGoals->Find();
			if(GoalFound == 1){
				GoalFound = 0;
				printf("Distance = %d \nAngle = %f\n", m_findGoals->GetDistance(), m_findGoals->GetAngle());
			}
		}
	}
	
	void READYTHEHOPPER(){
		if(m_shooter->GetTiltTarget() < 300){					// Move hopper to load position
			m_hopper->SetTiltTarget(600);
		}else if (m_shooter->GetTiltTarget() < 600){
			m_hopper->SetTiltTarget(400);
		}else {
			m_hopper->SetTiltTarget(200);
		}
	}

/***************************************** EVENTS ********************************************/
	
	void RaiseEvent(UINT8 EventSourceId, UINT32 EventNumber){
		
	}
	
	
	void WriteToLog(char *LogEntry){
		
		if (m_periodicCount >= 0) {
			fprintf(m_logFile, "%5d: %s \r\n", m_periodicCount, LogEntry);
		} else {
			fprintf(m_logFile, "%s \r\n", LogEntry);
		}
	}
	
};

START_ROBOT_CLASS(Team525);
