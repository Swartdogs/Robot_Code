#include "WPILib.h"
#include "Events.h"
#include "FindGoals.h"
#include "Hopper.h"
#include "Pickup.h"
#include "Drive.h"
#include "DiskShooter.h"

float const ROTATE_Deadband = 0.75;

enum EnumAction{actionNone, actionDrive, actionTurn, actionShoot, actionShootCamera, actionFrisbeePickup, actionDumpFrisbees};
enum EnumShoot{shootOff, shootCamera, shootFront, shootBack};

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

	SmartDashboard *m_Dash;
	Drive	  	   *m_Drive;
	DiskShooter	   *m_DiskShooter;
	Pickup	  	   *m_Pickup;
	FindGoals 	   *m_FindGoals;
	Hopper         *m_Hopper;
	Joystick  	   *m_joystick;
	Solenoid  	   *m_CameraLED;
	StructAuto 		Auto;
	StructAutoStep *AutoStep;
	FILE		   *LogFile;
	bool			CameraShot;
	char	   		AutoModeId0;
	char	   		AutoModeId1;
	char	   		AutoModeId2;
	char	   		AutoModeId3;
	char	   		AutoModeId4;
	char	   		AutoModeId5;
	char	   		AutoModeId6;
	char	   		AutoModeId7;
	
	double	   	    PeriodBeginTime;
	INT32	   		PeriodicCount;
	double	   		LastPeriodicStart;
	double	   		LastPeriodicEnd;
	
public:

	Team525(void)	{
		m_Drive = new Drive(1, 1,					//Left Front Motor 		PWM 1
							1, 2,					//Left Rear Motor 		PWM 2
							1, 3,					//Right Front Motor 	PWM 3
							1, 4, 					//Right Rear Motor 		PWM 4
							1, 1, 					//Left Encoder A 		DM: 1
							1, 2,					//Left Encoder B 		DM: 2
							1, 3,					//Right Encoder A 		DM: 3
							1, 4,					//Right Encoder B 		DM: 4
							1, 1,					//Rotational Gyro 		AM: 1
							this, 1);
		m_FindGoals = new FindGoals();
		m_CameraLED = new Solenoid(1, 5);
		m_Pickup = new Pickup(1, 5, 				//Pickup Motor 			PWM 5
							  1, 6, 				//Flip Motor 			PWM 6
							  1, 5, 				//Light Sensor 			DM: 5
							  1, 2, 				//Pot 					AM: 2
							  this, 2);
		
		m_joystick = new Joystick(1);
		
		m_DiskShooter = new DiskShooter(1, 7,		//Shoot Motor 			PWM 7
										1, 8,		//Tilt Motor 			PWM 8
										1, 1,		//Tension Motor 		Relay 1
										1, 3,		//Shoot Pot 			AM: 3
										1, 4,		//Tilt Pot 				AM: 4
										1, 5,		//Tension Pot 			AM: 5
										1, 6,		//Disk Sensor 			DM: 6
										this, 3);	
		m_Hopper = new Hopper(1,9,   				//Shoot Gate Servo	    PWM 9
						 	  1,10,  				//Hopper Gate Servo     PWM 10
						 	  1,11,  				//Hopper Tilt Moter     PWM 11
						 	  1,6,  			 	//Tilt Pot              AM: 6
						 	  1,7,   				//Disk Sensor           DM: 7
						 	  this,4);
		
		Team525::SetPeriod(0.02);
		
		printf("525 Constructor \n");
	}
	
	
	/********************************** Init Routines *************************************/

	void RobotInit(void) {
		GetWatchdog().SetEnabled(false);
		
		AutoModeId0 = 0;
		AutoModeId1 = 1;
		AutoModeId2 = 2;
		AutoModeId3 = 3;
		AutoModeId4 = 4;
		AutoModeId5 = 5;
		AutoModeId6 = 6;
		AutoModeId7 = 7;
		PeriodicCount = -1;
		
		printf("Robot Init \n");
	}
	
	void TestInit(void){
		m_CameraLED->Set(true);
		m_Drive->Enable();
		
		printf("Test Init");
	}
	
	void DisabledInit(void) {
		m_CameraLED->Set(false);
		m_Drive->Disable();
		
		printf("Disabled Init \n");
	}

	void AutonomousInit(void) {
		
		int AutoDelay;
		char AutoMode;
		
		PeriodicCount = 0;
		LastPeriodicStart = GetClock() * 1000;
		LastPeriodicEnd = LastPeriodicStart;
		PeriodBeginTime = LastPeriodicStart;
		
		AutoDelay = 0;
		Auto.StartDelay = AutoDelay / 20;
		
		AutoStepLoad(AutoMode);
		Auto.StepIndex = 0;
		Auto.InitStep = true;
		
		printf("Auto Init \n");
	}

	void TeleopInit(void) {
		printf("Teleop Init \n");
	}

	/********************************** Periodic Routines *************************************/
	
	void DisabledPeriodic(void)  {
		
		static int CameraStartDelay = 250;
		
		if (CameraStartDelay > 0) {							// Start Camera after delay
			CameraStartDelay--;								// (Waiting for completion of camera bootup)
			if (CameraStartDelay == 0) {
				//m_FindGoals->StartCamera("10.5.25.9");		// Start Camera
				//fprintf(LogFile, "Start Camera \n");
			}
		}
	}
	
	void TestPeriodic(void){
		static int  GoalFound = 0;
		static bool buttonPressed = false;
		
		m_Hopper->Periodic();
		m_Drive->Periodic(Drive::dJoystick, m_joystick->GetY(), m_joystick->GetX(), m_joystick->GetZ());
		
		if(m_joystick->GetRawButton(11)){
			if(!buttonPressed){
				GoalFound = m_FindGoals->Find(0);
				buttonPressed = true;
				printf("Looking For Wall Targets\n");
			}
		}else if(m_joystick->GetRawButton(12)){
			if(!buttonPressed){
				GoalFound = m_FindGoals->Find(1);
				buttonPressed = true;
				printf("Looking For Pyramid Goal\n");
			}
		}else if(GoalFound != -1){
			buttonPressed = false;
		}
		
		if(GoalFound < 0){
			GoalFound = m_FindGoals->Find();
			if(GoalFound == 1){
				GoalFound = 0;
				printf("Distance = %d \nAngle = %f\n", m_FindGoals->GetDistance(), m_FindGoals->GetAngle());
			}
		}
	}

	void AutonomousPeriodic(void) {
		
		static bool	AimReady = false;
		static bool	AutoStepDone = false;
		static bool DumpFrisbees = false;
		static bool FrisbeeLoadEnabled = false;
//		static int	GoalFound = 0;
//		static char	StartFrisbeeCount = 0;
		
		double TimeNow;
		
		TimeNow = GetClock() * 1000;
		LastPeriodicStart = TimeNow;
		
		if((TimeNow - LastPeriodicStart) > 100){
			fprintf(LogFile, "%5d:	Loop Delay	Start=%f  End=%f \n", PeriodicCount, TimeNow - LastPeriodicStart,
					TimeNow - LastPeriodicEnd);
			printf("Loop Delay:  Start=%f  End=%f \n", TimeNow - LastPeriodicStart, TimeNow - LastPeriodicEnd);
		}
		
		if(Auto.StartDelay > 0){
			Auto.StartDelay--;
			m_Drive->Periodic(Drive::dStop, 0, 0, 0);
			m_Pickup->FeedSafety();
			
			PeriodicCount++;
			return;
		}
		
		if(Auto.StepIndex >= Auto.StepCount){
			m_Drive->Periodic(Drive::dStop, 0, 0, 0);
			m_Pickup->FeedSafety();
			
			PeriodicCount++;
			return;
		}
		
// ------------------------- INITIALIZE NEW STEP -------------------------------------------

		if(Auto.InitStep){
			Auto.InitStep = false;
			AutoStepDone = false;
			
			AimReady = false;
			FrisbeeLoadEnabled = AutoStep[Auto.StepIndex].PickupFrisbees;
			CameraShot = false;
			DumpFrisbees = false;
			
			m_Drive->SetAngle(AutoStep[Auto.StepIndex].GyroTarget);
			m_Drive->SetDistance(AutoStep[Auto.StepIndex].Distance);
			m_Drive->SetMaxSpeed(AutoStep[Auto.StepIndex].MaxSpeed);
			
			if(AutoStep[Auto.StepIndex].ResetGyro) m_Drive->ResetGyro();
			if(AutoStep[Auto.StepIndex].ResetEncoders) m_Drive->ResetEncoders();
			if(AutoStep[Auto.StepIndex].Timeout <= 0) AutoStep[Auto.StepIndex].Timeout = -1;
			
			switch(AutoStep[Auto.StepIndex].Action){
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
			
			fprintf(LogFile, "%5d: Step %d  Step Action=%d  Gyro=%5.1f \n", PeriodicCount, Auto.StepIndex,
					AutoStep[Auto.StepIndex].Action, m_Drive->GetAngle());
		}

//------------------------- EXECUTE STEP --------------------------------------------------
		
		if(AutoStep[Auto.StepIndex].Timeout > 0) AutoStep[Auto.StepIndex].Timeout--;
		
		if(AutoStep[Auto.StepIndex].Timeout == 0){
			if(AutoStep[Auto.StepIndex].Distance != 0){
				fprintf(LogFile, "%5d: Step Timeout  Distance=%6.1f \n", PeriodicCount, m_Drive->GetDistance());
			}else{
				fprintf(LogFile, "%5d: Step Timeout \n", PeriodicCount);
			}
			
			AutoStepDone = true;
		}else{
			switch(AutoStep[Auto.StepIndex].Action){
				case actionDrive:
					AutoStepDone = m_Drive->Periodic(Drive::dAutoDrive, 0, 0, 0);
					break;
					
				case actionTurn:
					AutoStepDone = m_Drive->Periodic(Drive::dAutoRotate, 0, 0, 0);
					break;
					
				case actionShoot:
					//ADD STUFF!!!!!!!!!!!!!!!!!!!!!!!!!!!
					m_Drive->Periodic(Drive::dStop, 0, 0, 0);
					
				case actionShootCamera:
					
					break;
					
				case actionFrisbeePickup:
					m_Drive->Periodic(Drive::dStop, 0, 0, 0);
					//ADD STUFF!!!!!!!!!!!!!!!!!!!!!!!!!!!
					break;
					
				case actionDumpFrisbees:
					m_Drive->Periodic(Drive::dStop, 0, 0, 0);
					//ADD STUFF!!!!!!!!!!!!!!!!!!!!!!!!!!!
					break;
					
				default:
					AutoStepDone = true;
			}
		}
		
		if(AutoStepDone){
			if(AutoStep[Auto.StepIndex].StopRobot){
				m_Drive->Periodic(Drive::dStop, 0, 0, 0);
				m_Drive->ResetDriveSpeed();
			}else{
				m_Drive->Periodic(Drive::dCoast, 0, 0, 0);
			}
			
			fprintf(LogFile, "%5d: Step %d Done  Time = %5.0f \n", PeriodicCount, Auto.StepIndex, TimeNow - PeriodBeginTime);
			
			Auto.InitStep = true;
			FrisbeeLoadEnabled = false;
			DumpFrisbees = false;
			
			Auto.StepIndex++;
			
			if(Auto.StepIndex >= Auto.StepCount){
				fprintf(LogFile, "%5d:  Autonomous Completed  Time=%5.0f \n", PeriodicCount, TimeNow - PeriodBeginTime);
			}
		}
	
		PeriodicCount++;
		LastPeriodicEnd = GetClock() * 1000;
	
	}
	
	void TeleopPeriodic(void) {
	
	}

	void RaiseEvent(UINT8 EventSourceId, UINT32 EventNumber){
		
	}
	
	void WriteToLog(char *LogEntry){
		
	}
	
	void AutoStepInit(int NumberOfSteps){
		Auto.StepCount = NumberOfSteps;
		AutoStep = new StructAutoStep[Auto.StepCount];
		
		for(int i = 0; i < Auto.StepCount; i++){
			AutoStep[i].Action = actionNone;
			AutoStep[i].MaxSpeed = 0;
			AutoStep[i].Distance = 0;
			AutoStep[i].GyroTarget = -1;
			AutoStep[i].Timeout = 0;
			AutoStep[i].ResetGyro = false;
			AutoStep[i].ResetEncoders = false;
			AutoStep[i].StopRobot = false;
		}
	}
	
	void AutoStepLoad(char AutoSwitch){
		
		switch(AutoSwitch){
			case 1:
				AutoStepInit(4);
				
				AutoStep[0].Action = actionShoot;
				AutoStep[0].ResetGyro = true;
				
				AutoStep[1].Action = actionFrisbeePickup;
				
				AutoStep[2].Action = actionShoot;
				
				AutoStep[3].Action = actionFrisbeePickup;
			break;
			
			case 2:

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
				Auto.StepCount = 0;
		}
	}
	
};

START_ROBOT_CLASS(Team525);
