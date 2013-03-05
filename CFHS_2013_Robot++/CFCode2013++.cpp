
// DRIVE Joystick Button Assignments
//  3 - Activate Gyro assisted strafing
//  4 - Activate Gyro assisted strafing

// TILT Joystick Button Assignments:
//	1 - Print settings to Console 
//	2 - Change Shooter Tilt  {Use Joystick Y-axis)  
//  3 - Change Hopper Tilt   {Use Joystick Y-axis)
//  4 - Front Left Shot 
//	5 - Front Right Shot
//	6 - Increment Shooter Tilt Forward 
//	7 - Increment Shooter Tilt Backward
//  8 - Cancel Shoot (w/ button 9)
//	9 - Cancel Shoot (w/ button 8)
// 10 - Increment Shooter Tension Down
// 11 - Increment Shooter Tension Up

// BOX Button Assignments:						   	  Button Configuration
//	1 - Hopper Feeder Load position					 HOPPER	 SHOOTER  PICKUP
//  2 - Hopper Drive position							1		5		
//  3 - Hopper Under Pyramid position									9
//  4 - Load Shooter from Hopper						2		6	
//  5 - Front Left Shot													10
//  6 - Front Center Shot								3		7	
//  7 - Back Right Shot													11
//  8 - Fire Shooter									4		8
//  9 - Load Shooter from Pickup
// 10 - Store Pickup
// 11 - Deploy Pickup


// cRIO Assignments
//       ANALOG                          DIGITAL						     PWM OUTPUTS					     RELAY OUTPUTS
//  1 - Hopper:  Tilt Pot    		1 - Drive:  Left Encoder A			1 - Drive:   Left Front Motor		1 - Hopper:  Gate Motor
//  2 - Drive:   Rotational Gyro	2 - Drive:  Left Encoder B			2 - Drive:   Left Rear Motor		2 - Shooter: Tension Motor
//  3 - Shooter: Tilt Pot			3 - Drive:  Right Encoder A			3 - Drive:   Right Front Motor		3 - Pickup:  Disc Motor
//  4 - Shooter: Arm Pot			4 - Drive:  Right Encoder B			4 - Drive:   Right Rear Motor
//  5 - Shooter: Tension Pot		5 - Hopper: Before Gate Sensor		5 - Hopper:  Tilt Motor
//  6 - Pickup:  Arm Pot			6 - Hopper: After Gate Sensor		6 - Shooter: Tilt Motor
//  7 - Pickup:  Wrist Pot			7 - Unused							7 - Shooter: Arm Motor
//									8 - Pickup: Disc Sensor				8 - Pickup:  Arm Motor
//																		9 - Pickup:  Wirst Motor

#include "WPILib.h"
#include "Events.h"
#include "FindGoals.h"
#include "Hopper.h"
#include "DiskPickup.h"
#include "Drive.h"
#include "DiskShooter.h"

enum EnumAction{actionNone, actionDrive, actionTurn, actionDeployPickup, actionHopperShot, actionPickupShot};
enum EnumShootSeq{sIdle, sPrepareToLoad, sLoad, sMoveShooter, sWaitForMove, sWait, sFlop, sReady, sFire};

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
	double  	PickupDistance;
	INT32		shooterTensionTarget;
	INT32		shooterTiltTarget;
	INT32       hopperTiltTarget;
	int		    shootDelay;
} StructAutoStep;


class Team525: public IterativeRobot, public Events
{

	Drive	  	      *m_drive;
	DiskShooter	      *m_shooter;
	DiskPickup	      *m_pickup;
	DriverStationLCD  *m_DriverMessage;
	FILE		      *m_logFile;
	FindGoals 	      *m_findGoals;
	Hopper            *m_hopper;
	Joystick  	      *m_driveJoystick;
	Joystick          *m_tiltJoystick;
	Joystick	      *m_buttonBox;
	SendableChooser   *m_autoChooser;
	SmartDashboard    *m_dash;
	Solenoid  	      *m_sensorPower;
	StructAutoStep    *m_autoStep;

	StructAuto 		   m_auto;
	bool			   m_cameraShot;
	char	   		   m_autoModeId0;
	char	   		   m_autoModeId1;
	char               m_autoModeId2;
	char               m_autoModeId3;
	char               m_autoModeId4;
	
	EnumShootSeq                m_shootSeq;
	DiskPickup::PickupRunMode   m_pickupMode;
	
	int            			    m_sharedSpace;					// Hopper-Pickup Shared Space and Outside Robot Flags
	int                         m_outsideRobot;		  			// 0=Empty  1=Occupied by Hopper  2=Occupied by Pickup
	
	double	   	    		    m_periodBeginTime;
	INT32	   					m_periodicCount;
	double	   					m_lastPeriodStart;
	double	   					m_lastPeriodEnd;
	char         			    m_log[100];						// Buffer for Log Messages
	
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
							1, 2,					// Rotational Gyro 		AM 1: Analog 2
							this, 1);
		
		m_findGoals = new FindGoals();
		
		m_sensorPower = new Solenoid(1, 1);			// Sensor Power         SM 1: Solenoid 1
		
		m_pickup = new DiskPickup(1, 3, 			// Pickup Motor 		DM 1: Relay 3
							      1, 8, 			// Arm Motor 			DM 1: PWM 8
							      1, 9,				// Wrist Motor			DM 1: PWM 9
							      1, 8,		 		// Disk Sensor  		DM 1: Digital 8
							      1, 6, 			// Arm Pot	 			AM 1: Analog 6
							      1, 7,				// Wrist Pot			AM 1: Analog 7
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
									this, 3);	
		
		m_hopper = new Hopper(1, 1,   				// Shoot Gate Motor	    DM 1: Relay 1
						 	  1, 5,  				// Tilt Motor		    DM 1: PWM 5
						 	  1, 1,  			 	// Tilt Pot             AM 1: Analog 1
						 	  1, 5,   				// Before Sensor        DM 1: Digital 5
						 	  1, 6,                 // After Sensor         DM 1: Digital 6  
						 	  this, 4);
		
		m_DriverMessage = DriverStationLCD::GetInstance();

		m_autoChooser = new SendableChooser();
		
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
		m_autoModeId3 = 3;
		m_autoModeId4 = 4;
		
		m_autoChooser->AddDefault("0: Off", &m_autoModeId0);		// Send Autonomous Mode Selections to SmartDashboard
		m_autoChooser->AddObject("1: Back Right-Shoot 3", &m_autoModeId1);
		m_autoChooser->AddObject("2: Back Right-Drive-Shoot 3", &m_autoModeId2);
		m_autoChooser->AddObject("3: Back Left-Drive-Shoot 3", &m_autoModeId3);
		m_autoChooser->AddObject("4: Back Center-Shoot 3", &m_autoModeId4);
		
		m_dash->PutData("Autonomous Mode", m_autoChooser);
		m_dash->PutNumber("Auto Delay  ", BootCount);				// Send Autonomous Delay to SmartDashboard
		
		BootFile = fopen("Boot525.txt", "rb");
			if (!feof(BootFile)) fread(&BootCount, sizeof(int), 1, BootFile);
		fclose(BootFile);
		
		BootCount++;

		BootFile = fopen("Boot525.txt", "wb");
			fwrite (&BootCount, sizeof(int), 1, BootFile);
		fclose(BootFile);
		
		WriteToLog("525 2013 Robot Init (Build 2)");
		sprintf(m_log, "525 2013 Robot Boot %d", BootCount);
		WriteToLog(m_log);
		
		m_pickupMode = DiskPickup::pStore;
		m_sharedSpace = 0;
		m_outsideRobot = 0;
		
		printf("525 2013 Robot Init (Build 2) \n");
	}
	
/**************************************** DISABLED *******************************************/

	void DisabledInit(void) {
		
		m_sensorPower->Set(false);
		m_periodicCount = -1;
		
		m_drive->Disable();
		m_hopper->Disable();
		m_shooter->Disable();
		m_pickup->Disable();
		
		m_pickupMode = DiskPickup::pStore;
		WriteToLog("525 2013 Disabled Init");
		fclose(m_logFile);
		
		printf("525 2013 Disabled Init \n");
	}

	void DisabledPeriodic(void)  {
		
		static int CameraStartDelay = 250;
		
		if (CameraStartDelay > 0) {								// Start Camera after delay
			CameraStartDelay--;									// (Waiting for completion of camera bootup)
			if (CameraStartDelay == 0) {
				//m_findGoals->StartCamera("10.5.25.9");		// Start Camera
				//fprintf(m_logFile, "Start Camera \n");
			}
		}
		
//		printf("Hopper Tilt=%d\n", m_hopper->GetHopperPosition());
	}

/*************************************** AUTONOMOUS ******************************************/

	void AutonomousInit(void) {
		
		int AutoDelay = 0;
		char AutoMode = 0;
		
		m_periodicCount = 0;
		m_lastPeriodStart = GetClock() * 1000;
		m_lastPeriodEnd = m_lastPeriodStart;
		m_periodBeginTime = m_lastPeriodStart;
		
		AutoDelay = (int)m_dash->GetNumber("Auto Delay");
		m_auto.StartDelay = AutoDelay / 20;
		if (m_auto.StartDelay < 5) m_auto.StartDelay = 5;
		
		AutoMode = *((char *) m_autoChooser->GetSelected());	

		AutoStepLoad(AutoMode);
		m_auto.StepIndex = 0;
		m_auto.InitStep = true;
		
		m_sensorPower->Set(true);
		m_drive->Enable();
		m_hopper->Enable();
		m_shooter->Enable();
		m_pickup->Enable();
		
		m_shootSeq = sIdle;
		
		m_logFile = fopen("Log525.txt", "a");
		
		m_lastPeriodStart = GetClock() * 1000;
		m_lastPeriodEnd = m_lastPeriodStart;
		m_periodBeginTime = m_lastPeriodStart;
		
		sprintf(m_log, "Autonomous Init: AutoMode=%d  Delay=%d", AutoMode, AutoDelay);
		WriteToLog("m_log");
		
		printf("525 2013 Auto Init: Mode=%d  Delay=%d \n", AutoMode, AutoDelay);
	}

	
	void AutonomousPeriodic(void) {
		
		static bool	AutoStepDone = false;
		static int  waitCounter = 0;
		static int  autoHopperFlags = 0;
		static int  autoShooterFlags = 0;
		static int  autoPickupFlags = 0;
		
		double TimeNow;
		
		TimeNow = GetClock() * 1000;
		
		if((TimeNow - m_lastPeriodStart) > 100){
			sprintf(m_log, "Loop Delay	Start=%f  End=%f", TimeNow - m_lastPeriodStart,TimeNow - m_lastPeriodEnd);
			WriteToLog(m_log);
			printf("Loop Delay:  Start=%f  End=%f \r\n", TimeNow - m_lastPeriodStart, TimeNow - m_lastPeriodEnd);
		}
		
		m_lastPeriodStart = TimeNow;
		
		if(m_auto.StartDelay > 0){
			m_auto.StartDelay--;

			m_drive->Periodic(Drive::dStop, 0, 0, 0);
			autoHopperFlags = m_hopper->Periodic(0, &m_sharedSpace, &m_outsideRobot);
			autoShooterFlags = m_shooter->Periodic(0);
			autoPickupFlags = m_pickup->Periodic(&m_pickupMode, &m_sharedSpace, &m_outsideRobot);
		
			m_periodicCount++;
			return;
		}
		
		if(m_auto.StepIndex >= m_auto.StepCount){
			m_drive->Periodic(Drive::dStop, 0, 0, 0);
			autoHopperFlags = m_hopper->Periodic(0, &m_sharedSpace, &m_outsideRobot);
			autoShooterFlags = m_shooter->Periodic(0);
			autoPickupFlags = m_pickup->Periodic(&m_pickupMode, &m_sharedSpace, &m_outsideRobot);
			
			m_periodicCount++;
			return;
		}
		
		autoHopperFlags = m_hopper->Periodic(0, &m_sharedSpace, &m_outsideRobot);
		autoShooterFlags = m_shooter->Periodic(0);
		autoPickupFlags = m_pickup->Periodic(&m_pickupMode, &m_sharedSpace, &m_outsideRobot);
		
		
		// ------------------------- INITIALIZE NEW STEP -------------------------------------------

		if(m_auto.InitStep){
			m_auto.InitStep = false;
			AutoStepDone = false;
			m_cameraShot = false;
			
			m_drive->SetAngle(m_autoStep[m_auto.StepIndex].GyroTarget);
			m_drive->SetDistance(m_autoStep[m_auto.StepIndex].Distance);
			m_drive->SetMaxSpeed(m_autoStep[m_auto.StepIndex].MaxSpeed);
			
			if(m_autoStep[m_auto.StepIndex].ResetGyro) m_drive->ResetGyro();
			if(m_autoStep[m_auto.StepIndex].ResetEncoders) m_drive->ResetEncoders();
			if(m_autoStep[m_auto.StepIndex].Timeout <= 0) m_autoStep[m_auto.StepIndex].Timeout = -1;
			
			if(m_autoStep[m_auto.StepIndex].shooterTensionTarget >= 0) 
				m_shooter->SetTensionTarget(m_autoStep[m_auto.StepIndex].shooterTensionTarget);

			if (m_autoStep[m_auto.StepIndex].shooterTiltTarget >= 0 && m_autoStep[m_auto.StepIndex].Action != actionPickupShot)
					m_shooter->SetTiltTarget(m_autoStep[m_auto.StepIndex].shooterTiltTarget);

			if (m_autoStep[m_auto.StepIndex].hopperTiltTarget >= 0)
					m_hopper->SetTiltTarget(m_autoStep[m_auto.StepIndex].hopperTiltTarget);
			
			switch(m_autoStep[m_auto.StepIndex].Action){
				case actionDrive:
					if(m_autoStep[m_auto.StepIndex].Distance == 0) {
						AutoStepDone = true;
					} else if(m_autoStep[m_auto.StepIndex].PickupDistance != 0) {
						m_hopper->SetTiltTarget(166);
					}
					break;
	
				case actionDeployPickup:
					m_hopper->SetTiltTarget(166);
					m_pickupMode = DiskPickup::pDeployed;
					break;
					
				case actionHopperShot:
					if((autoHopperFlags & 10) > 0) {
						autoHopperFlags = autoShooterFlags = 0;
						m_shootSeq = sPrepareToLoad;

					} else {
						AutoStepDone = true;
					}
					break;
					
				case actionPickupShot:
					if ((autoPickupFlags & 1) == 1) {
						if (m_sharedSpace == 1) m_hopper->SetTiltTarget(130);
						autoHopperFlags = autoShooterFlags = 0;
						m_shootSeq = sPrepareToLoad;
						
					} else {
						AutoStepDone = true;
					}
					break;
					
				default:;
			}
			
			sprintf(m_log, "Step %d  Step Action=%d  Gyro=%5.1f", m_auto.StepIndex,
					m_autoStep[m_auto.StepIndex].Action, m_drive->GetAngle());
			WriteToLog(m_log);
		}

		//------------------------- EXECUTE STEP --------------------------------------------------
		
		if(m_autoStep[m_auto.StepIndex].Timeout > 0) m_autoStep[m_auto.StepIndex].Timeout--;
		
		if(m_autoStep[m_auto.StepIndex].Timeout == 0){
			if(m_autoStep[m_auto.StepIndex].Distance != 0){
				sprintf(m_log, "Step Timeout  Distance=%6.1f", m_drive->GetDistance());
				WriteToLog(m_log);
			}else{
				WriteToLog("Step Timeout");
			}
			
			AutoStepDone = true;
			
		}else{
			switch(m_autoStep[m_auto.StepIndex].Action){
				case actionDrive:
					if(m_autoStep[m_auto.StepIndex].PickupDistance < 0) {
						if(m_drive->GetDistance() < m_autoStep[m_auto.StepIndex].PickupDistance) {
							
							if(m_pickupMode != DiskPickup::pDeployed) {
								m_pickupMode = DiskPickup::pDeployed;
							} else if(m_pickupMode == DiskPickup::pDeployed) {
								if(autoHopperFlags & 4 == 4) {
									if(autoPickupFlags & 1 == 1) {
										m_pickupMode = DiskPickup::pStore;
										AutoStepDone = true;
									}
								}
							} 
						}
					}

					if (!AutoStepDone) {
						if(m_drive->Periodic(Drive::dAutoDrive, 0, 0, 0)) {
							AutoStepDone = true;
							m_pickupMode = DiskPickup::pStore;
						}
					}
					
					break;
					
				case actionTurn:
					AutoStepDone = m_drive->Periodic(Drive::dAutoRotate, 0, 0, 0);
					break;
					
				case actionDeployPickup:
					m_drive->Periodic(Drive::dStop, 0, 0, 0);
					AutoStepDone = ((autoPickupFlags & 2) == 2);	
					break;
					
				case actionHopperShot:
				case actionPickupShot:
					m_drive->Periodic(Drive::dStop, 0, 0, 0);
					
					switch(m_shootSeq) {
						case sPrepareToLoad:
							if ((autoHopperFlags & 1) == 1 && (autoShooterFlags & 3) == 3) {	// Hopper and Shooter ready for load
								m_shootSeq = sLoad;
								m_shooter->Load();							// Initiate Shooter Arm movement
							}
							break;

						case sLoad:
							if ((autoShooterFlags & 4) == 4 ) {				// Shooter arm ready for load
								if (m_autoStep[m_auto.StepIndex].Action == actionPickupShot) {
									m_pickupMode = DiskPickup::pLoad;
									waitCounter = 20;
									m_shootSeq = sMoveShooter;
								} else {
									m_hopper->RELEASETHEFRISBEE();			// Load Frisbee
									waitCounter = m_autoStep[m_auto.StepIndex].shootDelay;
									m_shootSeq = sWait;
								}
							}
							break;

						case sMoveShooter:
							if ((autoPickupFlags & 1) == 0) {
								waitCounter--;
								
								if (waitCounter <= 0) {
									if (m_autoStep[m_auto.StepIndex].shooterTiltTarget > 0) {
										m_shooter->SetTiltTarget(m_autoStep[m_auto.StepIndex].shooterTiltTarget);
										m_shootSeq = sWaitForMove;
									} else {
										waitCounter = m_autoStep[m_auto.StepIndex].shootDelay;
										m_shootSeq = sWait;
									}
								}
							}
							break;
							
						case sWaitForMove:
							if ((autoShooterFlags & 9) == 9) {
								waitCounter = m_autoStep[m_auto.StepIndex].shootDelay;
								m_shootSeq = sWait;
							}
							break;
							
						case sWait:
							waitCounter--;
							
							if (waitCounter <= 0) {							// Shooter loaded and Ready
								m_shootSeq = sFire;
								m_shooter->FIREINTHEHOLE();					// Fire Frisbee
							}
							break;
			
						case sFire:
							if ((autoShooterFlags & 16) == 16) {			// Shooter is idle
								m_shootSeq = sIdle; 						
								AutoStepDone = true;
							}
						default:;
					}
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
			
			sprintf(m_log, "Step %d Done  Time = %5.0f", m_auto.StepIndex, TimeNow - m_periodBeginTime);
			WriteToLog(m_log);
			
			m_auto.InitStep = true;
			
			m_auto.StepIndex++;
			
			if(m_auto.StepIndex >= m_auto.StepCount){
				sprintf(m_log, "Autonomous Completed  Time=%5.0f", TimeNow - m_periodBeginTime);
				WriteToLog(m_log);
			}
		}
	
		m_periodicCount++;
		m_lastPeriodEnd = GetClock() * 1000;
	
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
			m_autoStep[i].PickupDistance = 0;
			m_autoStep[i].shooterTensionTarget = -1;
			m_autoStep[i].shooterTiltTarget = -1;
			m_autoStep[i].hopperTiltTarget = -1;
			m_autoStep[i].shootDelay = 35;
		}
	}
	
	void AutoStepLoad(char AutoSwitch){
		
		switch(AutoSwitch){
			case 1:  // Start Back Right, Shoot 3  (Points=18)
				AutoStepInit(3);
				
				m_autoStep[0].Action = actionHopperShot;
				m_autoStep[0].hopperTiltTarget = 243;				// 337
				m_autoStep[0].shooterTiltTarget = 139;				// 165
				m_autoStep[0].shooterTensionTarget = 80;			// 124
				m_autoStep[0].shootDelay = 40;
				
				m_autoStep[1].Action = actionHopperShot;
				m_autoStep[1].shootDelay = 40;
				
				m_autoStep[2].Action = actionHopperShot;
				m_autoStep[2].shootDelay = 40;
			break;

			case 2:  // Start Back Right, Drive Forward, Shoot 3  (Points = 18)
				AutoStepInit(5);
				
				m_autoStep[0].Action = actionDrive;
				m_autoStep[0].ResetGyro = true;
				m_autoStep[0].MaxSpeed = 0.6;						// 0.5
				m_autoStep[0].Distance = 94;						// 50					
				m_autoStep[0].ResetEncoders = true;
				m_autoStep[0].GyroTarget = 0;
				m_autoStep[0].hopperTiltTarget = 217;				// 323
				m_autoStep[0].shooterTiltTarget = 185;				// 208
				m_autoStep[0].shooterTensionTarget = 56;			// 223
				m_autoStep[0].StopRobot = true;
				
//				m_autoStep[1].Action = actionDrive;
//				m_autoStep[1].MaxSpeed = 0.2;
//				m_autoStep[1].Distance = 94;					
//				m_autoStep[1].GyroTarget = 0;
//				m_autoStep[1].StopRobot = true;
				
				m_autoStep[1].Action = actionTurn;
				m_autoStep[1].GyroTarget = -15;
				
				m_autoStep[2].Action = actionHopperShot;
				
				m_autoStep[3].Action = actionHopperShot;
				
				m_autoStep[4].Action = actionHopperShot;
				m_autoStep[4].shootDelay = 40;
				
				break;
			
			case 3: // Start Back Left, Drive Forward, Shoot 3    (Points = 18)
				AutoStepInit(5);
				
				m_autoStep[0].Action = actionDrive;
				m_autoStep[0].ResetGyro = true;
				m_autoStep[0].MaxSpeed = 0.6;						// 0.5
				m_autoStep[0].Distance = 90;						// 50
				m_autoStep[0].ResetEncoders = true;
				m_autoStep[0].GyroTarget = 0;
				m_autoStep[0].hopperTiltTarget = 212;				// 324
				m_autoStep[0].shooterTiltTarget = 204;				// 204
				m_autoStep[0].shooterTensionTarget = 40;			// 111
				m_autoStep[0].StopRobot = true;
				
//				m_autoStep[1].Action = actionDrive;
//				m_autoStep[1].MaxSpeed = 0.2;
//				m_autoStep[1].Distance = 90;					
//				m_autoStep[1].GyroTarget = 0;
//				m_autoStep[1].StopRobot = true;
				
				m_autoStep[1].Action = actionTurn;
				m_autoStep[1].GyroTarget = 40;
				
				m_autoStep[2].Action = actionHopperShot;
				
				m_autoStep[3].Action = actionHopperShot;
				
				m_autoStep[4].Action = actionHopperShot;
				m_autoStep[4].shootDelay = 40;
				break;

			case 4: // Start Back Center, Shoot 3                 (Points = 18)
				AutoStepInit(5);

				m_autoStep[0].Action = actionDrive;
				m_autoStep[0].ResetGyro = true;
				m_autoStep[0].MaxSpeed = 0.6;
				m_autoStep[0].Distance = -20;					
				m_autoStep[0].ResetEncoders = true;
				m_autoStep[0].GyroTarget = 0;
				m_autoStep[0].hopperTiltTarget = 243;				// 371
				m_autoStep[0].shooterTiltTarget = 146;				// 146
				m_autoStep[0].shooterTensionTarget = 85;			// 85

				m_autoStep[1].Action = actionHopperShot;
				m_autoStep[1].shootDelay = 60;
				
				m_autoStep[2].Action = actionHopperShot;
				m_autoStep[2].shootDelay = 60;

				m_autoStep[3].Action = actionHopperShot;
				m_autoStep[3].shootDelay = 60;
				
				break;	

			case 5: // Sequence:  TEST FOR AUTOPICKUP
				AutoStepInit(3);
				
				
				m_autoStep[0].Action = actionDrive;
				m_autoStep[0].ResetGyro = true;
				m_autoStep[0].Distance = -90;
				m_autoStep[0].PickupDistance = -5;
				m_autoStep[0].MaxSpeed = 0.35;
				m_autoStep[0].ResetEncoders = true;
				m_autoStep[0].GyroTarget = 0;
				m_autoStep[0].hopperTiltTarget = 130;
				m_autoStep[0].shooterTiltTarget = 0;
				m_autoStep[0].shooterTensionTarget = 70;
				m_autoStep[0].Timeout = 150;
				
				m_autoStep[1].Action = actionTurn;
				m_autoStep[1].GyroTarget = -15;
				
				m_autoStep[2].Action = actionPickupShot;
				m_autoStep[2].shootDelay = 20;
			    m_autoStep[2].shooterTiltTarget = 150;
			    break;

			case 6: // Sequence ~~ DRIVE TEST
				AutoStepInit(3);
				
			    m_autoStep[0].Action = actionDrive;
			    m_autoStep[0].ResetGyro = true;
			    m_autoStep[0].MaxSpeed = 0.5;
			    m_autoStep[0].Distance = 50;					
			    m_autoStep[0].ResetEncoders = true;
			    m_autoStep[0].GyroTarget = 0;
			

			    m_autoStep[1].Action = actionDrive;
			    m_autoStep[1].MaxSpeed = 0.2;
			    m_autoStep[1].Distance = 80;					
			    m_autoStep[1].GyroTarget = 0;
			    m_autoStep[1].StopRobot = true;
			   
				m_autoStep[2].Action = actionTurn;
				m_autoStep[2].GyroTarget = 180;
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
		m_pickup->Enable();
		m_shootSeq = sIdle;
		
		m_lastPeriodStart = GetClock() * 1000;
		m_lastPeriodEnd = m_lastPeriodStart;
		m_periodBeginTime = m_lastPeriodStart;

		m_logFile = fopen("Log525.txt", "a");
		WriteToLog("525 Teleop Init");
		
		printf("Teleop Init \n");
	}

	
	void TeleopPeriodic(void) {
		
		static int          hopperFlags = 0;
		static int          shooterFlags = 0;
		static int			pickupFlags = 0;
		static int          minimumFireDelay = 0;
		
		static bool			loadFromPickup = false;
		static bool         newTension = false;
		static bool         newTilt = false;
		
		double			 	TimeNow;
				
		TimeNow = GetClock() * 1000;
		
		if((TimeNow - m_lastPeriodStart) > 100){
			sprintf(m_log, "Loop Delay	Start=%f  End=%f", TimeNow - m_lastPeriodStart, TimeNow - m_lastPeriodEnd);
			WriteToLog(m_log);
			printf("Loop Delay:  Start=%f  End=%f \n", TimeNow - m_lastPeriodStart, TimeNow - m_lastPeriodEnd);
		}
		
		m_lastPeriodStart = TimeNow;
		
		//--------------------------------Drive Stuff-------------------------------------------------------
		
		if(m_shootSeq != sFire){
			if(m_driveJoystick->GetRawButton(3) || m_driveJoystick->GetRawButton(4)){
				m_drive->Periodic(Drive::dStrafe, -m_driveJoystick->GetY(), -m_driveJoystick->GetX(), 0);
			}else{
				m_drive->Periodic(Drive::dJoystick, -m_driveJoystick->GetY(), -m_driveJoystick->GetX(), m_driveJoystick->GetZ());
			}
		}
		
		//--------------------------------Hopper Buttons-----------------------------------------------------
		
		if(m_buttonBox->GetRawButton(1)) {	
			if (m_outsideRobot == 2) m_pickupMode = DiskPickup::pStore;
			m_hopper->SetTiltTarget(260);									// Feeder load position
		
		} else if (m_buttonBox->GetRawButton(2)) {
			m_hopper->SetTiltTarget(166);									// Position for driving 
		
		} else if (m_buttonBox->GetRawButton(3)) {
			m_hopper->SetTiltTarget(4);                            			// Position to get under the pyramid
			m_pickupMode = DiskPickup::pUnderPyramid;
		}

		//--------------------------------Shooter Buttons----------------------------------------------------	
		
		if(m_buttonBox->GetRawButton(7)) {									// Back Right shot mode
			m_shooter->SetTiltTarget(139);									
			m_shooter->SetTensionTarget(80); 								
			m_hopper->SetTiltTarget(243);
	
		} else if (m_buttonBox->GetRawButton(6)) {							// Front Center shot mode
			m_shooter->SetTiltTarget(212);									
			m_shooter->SetTensionTarget(40);          					
			m_hopper->SetTiltTarget(212);
		
		} else if (m_buttonBox->GetRawButton(5)) {							// Front Left shot mode
			m_shooter->SetTiltTarget(204);									
			m_shooter->SetTensionTarget(40);							
			m_hopper->SetTiltTarget(212);

//			m_shooter->SetTiltTarget(146);									// Back Center shot mode
//			m_shooter->SetTensionTarget(85);							
//			m_hopper->SetTiltTarget(243);
		
		} else if(m_tiltJoystick->GetRawButton(4)) {						// Front Left shot mode
			m_shooter->SetTiltTarget(204);									
			m_shooter->SetTensionTarget(40);							
			m_hopper->SetTiltTarget(212);
		
		} else if(m_tiltJoystick->GetRawButton(5)) {						// Front Right shot mode
			m_shooter->SetTiltTarget(185);									
			m_shooter->SetTensionTarget(56);								
			m_hopper->SetTiltTarget(217);
		}

		if(m_tiltJoystick->GetRawButton(8) && m_tiltJoystick->GetRawButton(9)){ // Cancel Shoot Sequence
			m_shootSeq = sIdle;
		}
		
		if(m_tiltJoystick->GetRawButton(6)){ 								// Move shooter deck forward a little bit
			if(!newTilt){
				newTilt = true;
				m_shooter->IncrementShooter(true);
			}
		
		} else if (m_tiltJoystick->GetRawButton(7)) { 						// Move shooter deck back a little bit
			if (!newTilt){
				newTilt = true;
				m_shooter->IncrementShooter(false);
			}
		
		} else {
			newTilt = false;
		}
		
		if (m_tiltJoystick->GetRawButton(10)) { 							// Lower The Tension by a little bit
			if (!newTension) {
				newTension = true;
				m_shooter->SetTensionTarget(m_shooter->GetTensionTarget() - 10);   
				printf("New Tension=%d \n", m_shooter->GetTensionTarget());
			}
			
		} else if (m_tiltJoystick->GetRawButton(11)) { 						// Raise the Tension by a little bit
			if (!newTension) {
				newTension = true;
				m_shooter->SetTensionTarget(m_shooter->GetTensionTarget() + 10);
				printf("New Tension=%d \n", m_shooter->GetTensionTarget());
			}
			
		} else {
			newTension = false;
		}
		
		//--------------------------Pickup Buttons-----------------------------------------------------------
		
		if(m_buttonBox->GetRawButton(11)) {			 						// Deploy Pickup
			m_hopper->SetTiltTarget(250);
			m_pickupMode = DiskPickup::pDeployed;
			
		} else if(m_buttonBox->GetRawButton(10)) { 							// Move Pickup to Store position
			m_pickupMode = DiskPickup::pStore;
		}

		//--------------------------Call Periodic Functions--------------------------------------------------	
		
		float  tiltValue = -m_tiltJoystick->GetY();
		
		if (fabs(tiltValue) <= 0.1) tiltValue = 0;							// Joystick Tilt Value
		
		if (tiltValue == 0) {												// No Joystick value
			hopperFlags = m_hopper->Periodic(0, &m_sharedSpace, &m_outsideRobot);	
			shooterFlags = m_shooter->Periodic(0);
		
		} else if (m_tiltJoystick->GetRawButton(2)) {						// Manual Shooter Tilt
			hopperFlags = m_hopper->Periodic(0, &m_sharedSpace, &m_outsideRobot);
			shooterFlags = m_shooter->Periodic(tiltValue);
		
		} else if (m_tiltJoystick->GetRawButton(3)) {						// Manual Hopper Tilt
			hopperFlags = m_hopper->Periodic(tiltValue, &m_sharedSpace, &m_outsideRobot);
			shooterFlags = m_shooter->Periodic(0);
		
		} else {															// Ignore Joystick
			hopperFlags = m_hopper->Periodic(0, &m_sharedSpace, &m_outsideRobot);	
			shooterFlags = m_shooter->Periodic(0);
		}

		pickupFlags = m_pickup->Periodic(&m_pickupMode, &m_sharedSpace, &m_outsideRobot);
		
		static bool printDone = false;
		
		if (m_tiltJoystick->GetRawButton(1)) { 								// Print Various Values
			if (!printDone) {
				printf("Shooter:  Seq=%d Flags=%d  Arm=%d  Tilt=%d  Tension=%d \n", m_shootSeq, shooterFlags, m_shooter->GetShooterPosition(),
						m_shooter->GetTiltTarget(), m_shooter->GetTensionTarget());
				printf("Hooper: Flags=%d  Tilt=%d \n", hopperFlags, m_hopper->GetHopperPosition());
				printDone = true;
			}
			
//			if ((hopperFlags & 2) == 2){ // Rapid Fire Mode
//				m_shooter->SetTiltTarget(179);								// Back Right shot mode
//				m_shooter->SetTensionTarget(124);							// 247
//				m_hopper->SetTiltTarget(334);
//				if ((hopperFlags & 1) == 1){
//					m_shooter->Load();
//					m_shootSeq = sLoad;
//				}
//				if (m_shootSeq == sReady){
//					
//				}
//			}
		} else {
			printDone = false;
		}
		
		//--------------------------------Frisbee Shooting---------------------------------------------------	

		if (m_buttonBox->GetRawButton(4) && m_shootSeq == sIdle) { 			// Load Shooter from Hopper
			if ((hopperFlags & 8) == 8) {									// Frisbee in Hopper
				loadFromPickup = false;
				m_shootSeq = sPrepareToLoad;
			}
		
		} else if (m_buttonBox->GetRawButton(9) && m_shootSeq == sIdle) {	// Load Shooter from Pickup
			if ((pickupFlags & 1) == 1){									// Frisbee in pickup
				if (m_shooter->GetTiltTarget() != 0) {						// Shooter not in load position	
					m_shooter->SetTiltTarget(0);
					shooterFlags = 0;
				}
				
				if (m_sharedSpace == 1) {									// Hopper in shared space
					m_hopper->SetTiltTarget(130);
					hopperFlags = 0;
				}
				
				loadFromPickup = true;
				m_shootSeq = sPrepareToLoad;
			}
			
		} else if (m_buttonBox->GetRawButton(8)) { 							// Fire the shooter
//			printf("Shoot: Seq=%d  shooterFlags=%d  Position=%d \n", m_shootSeq, shooterFlags, m_shooter->GetShooterPosition());
			if (m_shootSeq == sReady && shooterFlags == 15) {
				m_shootSeq = sFire;
			}
		}
		
		switch(m_shootSeq) {
			case sPrepareToLoad:
				if ((hopperFlags & 1) == 1 && (shooterFlags & 3) == 3) {	// Hopper and Shooter ready for load
					m_shootSeq = sLoad;
					m_shooter->Load();										// Initiate Shooter Arm movement
				}
				break;

			case sLoad:
				if ((shooterFlags & 4) == 4 ) {								// Shooter arm ready for load
					if (loadFromPickup) {
						minimumFireDelay = 40;
						m_pickupMode = DiskPickup::pLoad;
						
					} else {
						if ((hopperFlags & 16) == 16) {
							minimumFireDelay = 35;
						} else {
							minimumFireDelay = 40;
						}
						
						m_hopper->RELEASETHEFRISBEE();						// Load Frisbee
					}
					
					m_shootSeq = sReady;
				}
				break;
			
			case sFlop:
				if(hopperFlags & 4 == 4) {
					m_hopper->SetTiltTarget(5);
				} else if(hopperFlags & 1 == 1) {
					minimumFireDelay = 0;
					m_shootSeq = sReady;
				}
				break;
			
			case sReady:
				if (minimumFireDelay > 0) minimumFireDelay--;
				break;
				
			case sFire:
				if (minimumFireDelay > 0) minimumFireDelay--;
				
				if (minimumFireDelay == 0) {
					m_shooter->FIREINTHEHOLE();
					minimumFireDelay = -1;
				} else if ((shooterFlags & 16) == 16) {
					m_shootSeq = sIdle;										// No Frisbee in Shooter
				}
			default:;
		}
		
		m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Hopper:  Tilt = %d   ", m_hopper->GetHopperPosition());
		m_DriverMessage->Printf(DriverStationLCD::kUser_Line2, 1, "Shooter: Tilt = %d   ", m_shooter->GetShooterPosition());
		m_DriverMessage->Printf(DriverStationLCD::kUser_Line3, 1, "      Tension = %d   ", m_shooter->GetTensionTarget());
		m_DriverMessage->UpdateLCD(); 										// Print the Tension Target to User messages
		
		m_periodicCount++;
		m_lastPeriodEnd = GetClock() * 1000;
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
		
		m_hopper->Periodic(0, &m_sharedSpace, &m_outsideRobot);
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

	
/***************************************** EVENTS ********************************************/
	
	void RaiseEvent(UINT8 EventSourceId, UINT32 EventNumber){
		switch (EventSourceId) {
			case 2:									// Pickup Event
				if (EventNumber == 1) {				// Storing deployed pickup with Frisbee loaded
					if (m_shootSeq == sIdle) m_shooter->SetTiltTarget(0);
				}
				break;
				
			case 4:									// Hopper Event
				if (EventNumber == 1) {				// Lowering hopper and Pickup is in shared space
					if (m_pickupMode != DiskPickup::pUnderPyramid) m_pickupMode = DiskPickup::pUnderPyramid;
					
				} else if (EventNumber == 2) {		// Raising hopper and Pickup is outside robot
					if (m_pickupMode != DiskPickup::pStore) m_pickupMode = DiskPickup::pStore;
				}
				break;
			
		}
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
