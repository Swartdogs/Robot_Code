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

enum EnumAction{actionNone, actionDrive, actionTurn, actionHopperShot, actionPickupShot};
enum EnumShootSeq{sIdle, sMoveHopper, sLoad, sMoveShooter, sWaitForMove, sWait, sFlop, sReady, sFire};

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

	SmartDashboard   *m_dash;
	SendableChooser  *m_autoChooser;
	Drive	  	     *m_drive;
	DiskShooter	     *m_shooter;
	DiskPickup	     *m_pickup;
	FindGoals 	     *m_findGoals;
	Hopper           *m_hopper;
	Joystick  	     *m_driveJoystick;
	Joystick         *m_tiltJoystick;
	Joystick	     *m_buttonBox;
	Solenoid  	     *m_sensorPower;
	StructAuto 		  m_auto;
	StructAutoStep   *m_autoStep;
	FILE		     *m_logFile;
	bool			  m_cameraShot;
	DriverStationLCD *m_DriverMessage;
	char	   		  m_autoModeId0;
	char	   		  m_autoModeId1;
	char              m_autoModeId2;

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
		
		m_autoChooser->AddDefault("Off", &m_autoModeId0);		// Send Autonomous Mode Selections to SmartDashboard
		m_autoChooser->AddObject("Shoot 3, Pickup and Shoot", &m_autoModeId1);
		m_autoChooser->AddObject("Shoot 3", &m_autoModeId2);
		
		m_dash->PutData("Autonomous Mode", m_autoChooser);
		m_dash->PutNumber("Auto Delay", BootCount);				// Send Autonomous Delay to SmartDashboard
		
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
		
		m_pickupMode = DiskPickup::pStore;
		m_sharedSpace = 0;
		m_outsideRobot = 0;
		
		printf("525 2013 Robot Init \n");
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
		
		if (CameraStartDelay > 0) {							// Start Camera after delay
			CameraStartDelay--;								// (Waiting for completion of camera bootup)
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

		AutoMode = 2;
		m_auto.StartDelay = 5;
		
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
	
				case actionHopperShot:
					if((autoHopperFlags & 10) > 0) {
						
						autoHopperFlags = autoShooterFlags = 0;
						m_shootSeq = sMoveHopper;
					} else {
						AutoStepDone = true;
					}
					break;
					
				case actionPickupShot:
					if ((autoPickupFlags & 1) == 1) {
						if (m_sharedSpace == 1){
							m_hopper->SetTiltTarget(166);
							m_shootSeq = sMoveHopper;
						} else {
							m_shootSeq = sLoad;
							m_shooter->Load();
						}
						
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
					
				case actionHopperShot:
				case actionPickupShot:
					m_drive->Periodic(Drive::dStop, 0, 0, 0);
					
					switch(m_shootSeq) {
						case sMoveHopper:
							if ((autoHopperFlags & 3) == 3 && (autoShooterFlags & 35) == 35) {	// Hopper and Shooter ready for load
								m_shootSeq = sLoad;
								m_shooter->Load();									// Initiate Shooter Arm movement
							}
							break;

						case sLoad:
							if ((autoShooterFlags & 4) == 4 ) {						// Shooter arm ready for load
								if (m_autoStep[m_auto.StepIndex].Action == actionPickupShot) {
									m_pickupMode = DiskPickup::pLoad;
									waitCounter = 20;
									m_shootSeq = sMoveShooter;
								} else {
									m_hopper->RELEASETHEFRISBEE();						// Load Frisbee
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
								m_shooter->FIREINTHEHOLE();							// Fire Frisbee
							}
							break;
			
						case sFire:
							if ((autoShooterFlags & 16) == 0) {
								m_shootSeq = sIdle; // No Frisbee in Shooter
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
			case 1: 
				AutoStepInit(6);
				
				m_autoStep[0].Action = actionHopperShot;
				m_autoStep[0].ResetGyro = true;
				
				m_autoStep[1].Action = actionHopperShot;
				m_autoStep[1].ResetGyro = true;
				
				m_autoStep[2].Action = actionHopperShot;
				m_autoStep[2].ResetGyro = true;

				m_autoStep[3].Action = actionDrive;
				
				m_autoStep[4].Action = actionDrive;
				
				m_autoStep[5].Action = actionPickupShot;
			break;
			
			case 2: // Sequence:  TEST FOR AUTOPICKUP
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

			case 3: // Sequence ~~ DRIVE TEST
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
			   
			case 4: // Start Position ~~ Back Right Corner ~~~ Points Possible: 18
				AutoStepInit(3);
				
				m_autoStep[0].Action = actionHopperShot;
				m_autoStep[0].hopperTiltTarget = 337;
				m_autoStep[0].shooterTiltTarget = 165;
				m_autoStep[0].shooterTensionTarget = 124;			// 247
				m_autoStep[0].shootDelay = 40;
				
				m_autoStep[1].Action = actionHopperShot;
				m_autoStep[1].shootDelay = 40;
				
				m_autoStep[2].Action = actionHopperShot;
				m_autoStep[2].shootDelay = 40;
			break;
			
			
			case 5: // Start Position: Back Right ~~ Drive up and shoot from front right corner ~~ Points Possible: 18
				AutoStepInit(6);
				
				m_autoStep[0].Action = actionDrive;
				m_autoStep[0].ResetGyro = true;
				m_autoStep[0].MaxSpeed = 0.5;
				m_autoStep[0].Distance = 50;					
				m_autoStep[0].ResetEncoders = true;
				m_autoStep[0].GyroTarget = 0;
				m_autoStep[0].hopperTiltTarget = 323;
				m_autoStep[0].shooterTiltTarget = 208;
				m_autoStep[0].shooterTensionTarget = 111;			// 223
				
				
				m_autoStep[1].Action = actionDrive;
				m_autoStep[1].MaxSpeed = 0.2;
				m_autoStep[1].Distance = 94;					
				m_autoStep[1].GyroTarget = 0;
				m_autoStep[1].StopRobot = true;
				
				m_autoStep[2].Action = actionTurn;
				m_autoStep[2].GyroTarget = -15;
				
				m_autoStep[3].Action = actionHopperShot;
				
				m_autoStep[4].Action = actionHopperShot;
				
				m_autoStep[5].Action = actionHopperShot;
				m_autoStep[5].shootDelay = 40;
				
				break;
			
			case 6: // Start Position: Back Left ~~ Drive up and shoot all frisbees from front left corner ~~ Points Possible: 18
				AutoStepInit(6);
				
				m_autoStep[0].Action = actionDrive;
				m_autoStep[0].ResetGyro = true;
				m_autoStep[0].MaxSpeed = 0.5;
				m_autoStep[0].Distance = 50;					
				m_autoStep[0].ResetEncoders = true;
				m_autoStep[0].GyroTarget = 0;
				m_autoStep[0].hopperTiltTarget = 324;
				m_autoStep[0].shooterTiltTarget = 204;
				m_autoStep[0].shooterTensionTarget = 111;				// 222
				
				
				m_autoStep[1].Action = actionDrive;
				m_autoStep[1].MaxSpeed = 0.2;
				m_autoStep[1].Distance = 90;					
				m_autoStep[1].GyroTarget = 0;
				m_autoStep[1].StopRobot = true;
				
				m_autoStep[2].Action = actionTurn;
				m_autoStep[2].GyroTarget = 40;
				
				m_autoStep[3].Action = actionHopperShot;
				
				m_autoStep[4].Action = actionHopperShot;
				
				m_autoStep[5].Action = actionHopperShot;
				m_autoStep[5].shootDelay = 40;
			break;
			
			case 7: // Start Position: Back Center ~~ Drive back and shoot all three frisbees through pyramid ~~ Points Possible: 18
				AutoStepInit(5);

				m_autoStep[0].Action = actionDrive;
				m_autoStep[0].ResetGyro = true;
				m_autoStep[0].MaxSpeed = 0.6;
				m_autoStep[0].Distance = -20;					
				m_autoStep[0].ResetEncoders = true;
				m_autoStep[0].GyroTarget = 0;
				m_autoStep[0].hopperTiltTarget = 371;
				m_autoStep[0].shooterTiltTarget = 146;					// 142
				m_autoStep[0].shooterTensionTarget = 85;				// 222

				m_autoStep[1].Action = actionHopperShot;
				m_autoStep[1].shootDelay = 60;
				
				m_autoStep[2].Action = actionHopperShot;
				m_autoStep[2].shootDelay = 60;

				m_autoStep[3].Action = actionHopperShot;
				m_autoStep[3].shootDelay = 60;
				
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
		
		static bool			buttonPushed = false;
		static bool			loadFromPickup = false;
		static bool         newTension = false;

		double TimeNow;
				
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
		
		if(m_driveJoystick->GetRawButton(12)) {
			m_drive->ResetEncoders();
		}
		
		//--------------------------------Hopper Stuff------------------------------------------------------
		
		if(m_shootSeq == sIdle){
			if(m_buttonBox->GetRawButton(8) && shooterFlags >= 3 && (hopperFlags & 4) == 4){// Shoot button pressed, Shooter Tilt and Tension completed 
				if((shooterFlags & 16) == 16){ // Frisbee already in shooter deck
					m_shootSeq = sFire;
				}else{
					m_shootSeq = sMoveHopper;
					READYTHEHOPPER();
				}
				
			} else if(m_buttonBox->GetRawButton(1)) {	
				if (m_outsideRobot == 2) m_pickupMode = DiskPickup::pStore;
				m_hopper->SetTiltTarget(260);							// Feeder load position
			} else if (m_buttonBox->GetRawButton(2)) {
				m_hopper->SetTiltTarget(166);							// Position for driving 
			} else if (m_buttonBox->GetRawButton(3)) {
				m_hopper->SetTiltTarget(4);                            // Position to get under the pyramid
				m_pickupMode = DiskPickup::pUnderPyramid;
			}
		}

		//--------------------------------Shooter Stuff------------------------------------------------------	
		
		if(m_buttonBox->GetRawButton(7)) {
			m_shooter->SetTiltTarget(139);							// Back Right shot mode
			m_shooter->SetTensionTarget(80); 						// 247
			m_hopper->SetTiltTarget(243);
		} else if (m_buttonBox->GetRawButton(6)) {
			m_shooter->SetTiltTarget(212);							// Front Center shot mode
			m_shooter->SetTensionTarget(40);          				// 197
			m_hopper->SetTiltTarget(212);
		} else if (m_buttonBox->GetRawButton(5)) {
			m_shooter->SetTiltTarget(204);							// Front Left shot mode // Could add another preset
			m_shooter->SetTensionTarget(40);						// 222
			m_hopper->SetTiltTarget(212);
		} else if(m_tiltJoystick->GetRawButton(4)) {
			m_shooter->SetTiltTarget(204);							// Front Left shot mode
			m_shooter->SetTensionTarget(40);						// 222
			m_hopper->SetTiltTarget(212);
		} else if(m_tiltJoystick->GetRawButton(5)) {
			m_shooter->SetTiltTarget(185);							// Front Right shot mode
			m_shooter->SetTensionTarget(56);						// 223
			m_hopper->SetTiltTarget(217);
		}

		if(m_tiltJoystick->GetRawButton(8) && m_tiltJoystick->GetRawButton(9)){ // Cancel Shoot Sequence
			m_shootSeq = sIdle;
		}
		
		if(m_tiltJoystick->GetRawButton(6)){ // move shooter deck forward a little bit
			if(!buttonPushed){
				m_shooter->IncrementShooter(true);
				buttonPushed = true;
			}
		} else if (m_tiltJoystick->GetRawButton(7)) { // move shooter deck back a little bit
			if (!buttonPushed){
				m_shooter->IncrementShooter(false);
				buttonPushed = true;
			}
		} else {
			buttonPushed = false;
		}
		
		
		//--------------------------Pickup Stuff------------------------------------------------------
		
		if(m_buttonBox->GetRawButton(11)) { // Deploy Pickup
			m_hopper->SetTiltTarget(250);
			m_pickupMode = DiskPickup::pDeployed;
		} else if(m_buttonBox->GetRawButton(10)) { // Move Pickup to Store position
			m_pickupMode = DiskPickup::pStore;
		} else if(m_buttonBox->GetRawButton(9) && m_shootSeq == sIdle) { // Load Frisbee into shooter deck
			if ((pickupFlags & 1) == 1){
				loadFromPickup = true;
				
				if (m_sharedSpace == 1){
					m_hopper->SetTiltTarget(130);
					m_shootSeq = sMoveHopper;
				}
				else {
					m_shootSeq = sLoad;
					m_shooter->Load();
				}
			}
		}
		//--------------------------Call Periodic Functions--------------------------------------------	
		
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
		
		//--------------------------------Frisbee Shoot Sequence---------------------------------------------	
		
		static bool printDone = false;
		
		if (m_tiltJoystick->GetRawButton(1)) { // Print Various Values
			if (!printDone) {
				printf("Shooter:  Seq=%d Flags=%d  Arm=%d  Tilt=%d  Tension=%d \n", m_shootSeq, shooterFlags, m_shooter->GetShooterPosition(),
						m_shooter->GetTiltTarget(), m_shooter->GetTensionTarget());
				printf("Hooper: Flags=%d  Tilt=%d \n", hopperFlags, m_hopper->GetHopperPosition());
				printDone = true;
			}
//			if ((hopperFlags & 2) == 2){ // Rapid Fire Mode
//				m_shooter->SetTiltTarget(179);							// Back Right shot mode
//				m_shooter->SetTensionTarget(124);						// 247
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
		
		if (m_tiltJoystick->GetRawButton(10)) { // Lower The Tension by a little bit
			if (!newTension) {
				newTension = true;
				m_shooter->SetTensionTarget(m_shooter->GetTensionTarget() - 10);   // 25
				printf("New Tension=%d \n", m_shooter->GetTensionTarget());
			}
		} else if (m_tiltJoystick->GetRawButton(11)) { // Raise the Tension by a little bit
			if (!newTension) {
				newTension = true;
				m_shooter->SetTensionTarget(m_shooter->GetTensionTarget() + 10);	// 25
				printf("New Tension=%d \n", m_shooter->GetTensionTarget());
			}
		} else {
			newTension = false;
		}
		
		if (m_buttonBox->GetRawButton(4)) { // Load the Shooter
			if (m_shootSeq == sIdle  && shooterFlags >= 3 && (hopperFlags & 2) == 2){ // Check to see if Hopper and Shooter are ready
				m_shootSeq = sLoad;
				m_shooter->Load();
			}	
		} else if (m_buttonBox->GetRawButton(8)) { // Fire the shooter
//			printf("Shoot: Seq=%d  shooterFlags=%d  Position=%d \n", m_shootSeq, shooterFlags, m_shooter->GetShooterPosition());
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
					if (!loadFromPickup){
						m_hopper->RELEASETHEFRISBEE();						// Load Frisbee
					} else {
						m_pickupMode = DiskPickup::pLoad;
					}
					m_shootSeq = sReady;
				}
				
//				if (shooterFlags == 31) {								// Shooter loaded and Ready
//					m_shootSeq = sFire;
//					m_shooter->FIREINTHEHOLE();							// Fire Frisbee
//				}
				break;
			
			case sFlop:
				if(hopperFlags & 4 == 4) {
					m_hopper->SetTiltTarget(5);
				} else if(hopperFlags & 1 == 1) {
					m_shootSeq = sReady;
				}
				break;
			
			case sFire:
				if ((shooterFlags & 16) == 0) m_shootSeq = sIdle;			// No Frisbee in Shooter
				loadFromPickup = false;
			default:;
		}
		
		m_DriverMessage->Printf(DriverStationLCD::kUser_Line3, 1, "Tension: %d   ", m_shooter->GetTensionTarget());
		m_DriverMessage->UpdateLCD(); // Print the Tension Target to User messages
		
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
		if (EventSourceId == 4) {				// Hopper Event
			if (EventNumber == 1) {
				if (m_pickupMode != DiskPickup::pUnderPyramid) m_pickupMode = DiskPickup::pUnderPyramid;
			} else if (EventNumber == 2) {
				if (m_pickupMode != DiskPickup::pStore) m_pickupMode = DiskPickup::pStore;
			}
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
