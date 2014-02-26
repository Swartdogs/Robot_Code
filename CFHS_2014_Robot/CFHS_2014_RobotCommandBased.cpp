#include "WPILib.h"
#include "IterativeRobot525.h"
#include "Commands/Command.h"
#include "CommandBase.h"
#include "RobotMap.h"
#include "RobotLog.h"
#include "Commands/AllCommands.h"
#include <string>
#include <cstdlib>

class CommandBasedRobot : public IterativeRobot525, public RobotLog {
private:
	typedef enum {mInit, mStart, mDisabled, mAutonomous, mTeleop, mTest}RunMode;
	
	CommandGroup*		m_autoCommand;
	DriverStationLCD*	m_DriverMessage;
	FILE*      			m_logFile;
	
	int      			m_autoDelay;				// Autonomous Start Delay Count
	int					m_autoSelect;				// Autonomous Command Group Selection
	char       			m_log[100];
	INT32      			m_periodicCount;
	double     			m_periodicBeginTime;
	double     			m_periodicLastEnd;
	double     			m_periodicLastStart;
	double     			m_periodicTotalTime;
	RunMode    			m_runMode;
	
	Solenoid*           m_sensorPower1;
	Solenoid*  			m_sensorPower2;

	void EndOfPeriodic() {										
		m_periodicCount++;										
		m_periodicLastEnd = GetClock() * 1000;					
		
		double runTime = m_periodicLastEnd - m_periodicLastStart;	
		
		m_periodicTotalTime += runTime;							
		
		if(runTime  > 10){										
			sprintf(m_log, "Long Periodic Duration=%f", runTime);
			LogWrite(m_log);
		}
	}
	
	void LogWrite(char *LogEntry){		
		if (m_runMode == mInit) return;
		
		if (m_periodicCount > 0) {
			fprintf(m_logFile, "%5d  %5d: %s \r\n", m_periodicCount, (int)(((GetClock()*1000) - m_periodicBeginTime) / 20), LogEntry);
			printf("%5d  %5d: %s \n", m_periodicCount, (int)(((GetClock()*1000) - m_periodicBeginTime) / 20), LogEntry);
		} else if (m_runMode == mDisabled) {
			m_logFile = fopen("Log525.txt", "a");
			fprintf(m_logFile, "%s \r\n", LogEntry);
			printf("%s \n", LogEntry);
			fclose(m_logFile);
		} else {
			fprintf(m_logFile, "%s \r\n", LogEntry);
			printf("%s \n", LogEntry);
		}
	}

	void IniParser() {
		char 	temp[100];
		int   	subsystemIndex;
		char* 	subsystemName[3];
		int     equalSign;
		string  key;
		INT32   keyValue;
		
		subsystemName[0] = "[BACKPICKUP]";
		subsystemName[1] = "[FRONTPICKUP]";
		subsystemName[2] = "[BALLSHOOTER]";
		
		FILE* iniFile = fopen("525Init.ini", "r");
		
		if (iniFile == NULL) {
			LogWrite("IniParser: File not found");
			fclose(iniFile);
			return;
		}
			
		while(!feof(iniFile)) {
			fgets(temp, 100, iniFile);
			string line(temp);
			line.erase(line.length() - 2, 2);
			
			if(line[0] == '[') {
				subsystemIndex = -1;
				
				for(int i = 0; i < 3; i++) {
					if(!line.compare(0, strlen(subsystemName[i]), subsystemName[i])) {
						subsystemIndex = i;
						break;
					}
				}

			} else if(line.length() > 0 && line[0] != ' ' && line[0] != '!') {
				equalSign = line.find('=');
				if(equalSign != 0) {
					key = line.substr(0, equalSign);
					keyValue = atoi(line.substr(equalSign + 1).c_str());
//					printf("Subsystem=%d  Key=%s  Value=%d\n", subsystemIndex, key.c_str(), keyValue);

					switch(subsystemIndex) {
					case 0:								// Back Pickup
						CommandBase::backPickup->SetConstant(key.c_str(), keyValue);
						break;
					case 1:								// Front Pickup
						CommandBase::frontPickup->SetConstant(key.c_str(), keyValue);
						break;
					case 2:								// Ball Shooter
						CommandBase::ballShooter->SetConstant(key.c_str(), keyValue);
						break;
					default:;
					}
				}
			}
			
		}
		fclose(iniFile);
	}
	
	
	virtual void RobotInit() {
		m_runMode = mInit;
		
		CommandBasedRobot::SetPeriod(0.02);
		
		m_sensorPower1 = new Solenoid(SOLENOID_SENSOR_POWER1);
		m_sensorPower2 = new Solenoid(SOLENOID_SENSOR_POWER2);
		
		m_sensorPower1->Set(true);
		m_sensorPower2->Set(true);

		m_DriverMessage = DriverStationLCD::GetInstance();	

		m_runMode = mStart;
		m_autoDelay = m_autoSelect = m_periodicCount = 0;
		m_autoCommand = NULL;

		m_logFile = fopen("Log525.txt", "a");
		LogWrite("");
		LogWrite("2014 Robot Init (Build Pekin)");
		
		CommandBase::Init(this);
		IniParser();
	}

	virtual void DisabledInit() {
		if(m_runMode != mStart){								// Log Usage at end of Autonomous and Telelop
			sprintf(m_log, "Periodic Usage=%5.1f %%", (m_periodicTotalTime / (GetClock() * 1000 - m_periodicBeginTime)) * 100);
			LogWrite(m_log);
		}

		CommandBase::StopMotors();
		CommandBase::drive->SetSafetyEnabled(false);
		
		m_periodicCount = 0;
		
		switch (m_runMode) {									// Display Run and Drive Mode in Classmate User Messages 
			case mStart:
				m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Select Autonomous   ");
				break;
			case mAutonomous:
				if (m_autoCommand != NULL) m_autoCommand->Cancel();
				m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Auto: Disabled      ");
				break;
			case mTeleop:
				m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Teleop: Disabled    ");
				break;
			case mTest:
				m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Test: Disabled      ");
				break;
			default:
				m_DriverMessage->Clear();
		}

		m_DriverMessage->UpdateLCD();
		
		LogWrite("2014 Robot Disabled Init");
		fclose(m_logFile);												// Close Log File
		m_runMode = mDisabled;
		
		CommandBase::backPickup->SetPickupMode(BackPickup::bStore);
	}

	virtual void DisabledPeriodic() {
		static int CameraStartDelay = 250;
		static int printIndex = 0;
		
		if (CameraStartDelay > 0) {										// Start Camera after delay
			CameraStartDelay--;											// (Waiting for completion of camera bootup)
			if (CameraStartDelay == 0) {
				CommandBase::findTarget->StartCamera("10.5.25.9");		// Start Camera
				LogWrite("Start Camera");
			}
		}

		if (CommandBase::oi->GetButtonPress(9)) {						// Change Autonomous Selection
			if (m_autoSelect < 4) {
				m_autoSelect++;
			} else {
				m_autoSelect = 0;
			}
			
			m_DriverMessage->Printf(DriverStationLCD::kUser_Line5, 1, "Auto=%d  Delay=%d    ", m_autoSelect, m_autoDelay * 250);
			m_DriverMessage->UpdateLCD();
		
		} else if (CommandBase::oi->GetButtonPress(10)) {				// Change Autonomous Delay		
			if (m_autoDelay < 20) {
				m_autoDelay++;
			} else {
				m_autoDelay = 0;
			}
			
			m_DriverMessage->Printf(DriverStationLCD::kUser_Line5, 1, "Auto=%d  Delay=%d    ", m_autoSelect, m_autoDelay * 250);
			m_DriverMessage->UpdateLCD();
		
		} else if (CommandBase::oi->GetButtonPress(11)) {				// Read Ini File
			IniParser();
		
		} else if (CommandBase::oi->GetButtonPress(6)) {				// Select option data printing
			if (printIndex < 5) {
				printIndex++;
			} else {
				printIndex = 0;
			}
		}

		switch(printIndex) {
		case 1:
			printf("Front Pickup: Left=%d, Right=%d\n", CommandBase::frontPickup->GetPosition(FrontPickup::pLeft), CommandBase::frontPickup->GetPosition(FrontPickup::pRight));
			break;
		case 2:
			printf("Back Pickup: %d\n", CommandBase::backPickup->GetPosition());
			break;
		case 3:
			printf("Shooter Position=%d\n", CommandBase::ballShooter->GetShooterPosition());
			break;
		case 4:
			printf("Range: %.3f\n", CommandBase::drive->GetRange());
			break;
		default:;
		}
		
	}
	
	virtual void AutonomousInit() {
		m_runMode = mAutonomous;								// Set Run Mode and Initialize Variables
		m_periodicCount = 0;
		m_periodicLastStart = GetClock() * 1000;
		m_periodicLastEnd = m_periodicLastStart;
		m_periodicBeginTime = m_periodicLastStart;
		m_periodicTotalTime = 0;

		switch(m_autoSelect) {									// Create instance of selected Autonomous Command Group
			case 1: m_autoCommand = new OneBallAuto();
				break;
			case 2: m_autoCommand = new TwoBallAuto();
				break;
			case 3: m_autoCommand = new TwoBallHotAuto();
				break;
			default: m_autoCommand = NULL;
		}
		
		CommandBase::drive->ResetGyro();
		
		if (m_autoCommand != NULL) m_autoCommand->Start();		// Start Autonomous Command Group

		CommandBase::ballShooter->Load();
		
		m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Auto: Executing %d   ", m_autoSelect);
		m_DriverMessage->UpdateLCD();

		m_logFile = fopen("Log525.txt", "a");					// Open Log File

		sprintf(m_log, "2014 Robot Auto Init: Command=%d  Delay=%d", m_autoSelect, m_autoDelay * 250);
		LogWrite(m_log);
		LogWrite("2014 Robot Autonomous Init");
	}
	
	virtual void AutonomousPeriodic() {
		double timeNow = GetClock() * 1000;
		
		if((timeNow - m_periodicLastStart) > 100){				// Log Periodic() Intervals > 100ms
			sprintf(m_log, "Delay    Last Start=%f  Last End=%f", timeNow - m_periodicLastStart, timeNow - m_periodicLastEnd);
			LogWrite(m_log);
		}
		
		m_periodicLastStart = timeNow;							// Set Periodic Start Time

		CommandBase::Periodic();
		Scheduler::GetInstance()->Run();
		EndOfPeriodic();
	}
	
	virtual void TeleopInit() {
		if (m_autoCommand != NULL) m_autoCommand->Cancel();
		
		m_runMode = mTeleop;									// Set Run Mode and Initialize Variables
		m_periodicCount = 0;
		m_periodicLastStart = GetClock() * 1000;
		m_periodicLastEnd = m_periodicLastStart;
		m_periodicBeginTime = m_periodicLastStart;
		m_periodicTotalTime = 0;

		m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Teleop: Executing   ");
		m_DriverMessage->UpdateLCD();
	
		CommandBase::ballShooter->Load();
		
		m_logFile = fopen("Log525.txt", "a");					// Open Log File
		LogWrite("Robot 2014 Teleop Init");
	}
	
	virtual void TeleopPeriodic() {
		double timeNow = GetClock() * 1000;
		
		if((timeNow - m_periodicLastStart) > 100){				// Log Periodic() Intervals > 100ms
			sprintf(m_log, "Delay    Last Start=%f  Last End=%f", timeNow - m_periodicLastStart, timeNow - m_periodicLastEnd);
			LogWrite(m_log);
		}
		
		m_periodicLastStart = timeNow;							// Set Periodic Start Time

		if ((timeNow - m_periodicBeginTime) > 138000) {			// 2 seconds left in 140 second period
//			if (!CommandBase::ballShooter->HasBall()) CommandBase::ballShooter->Release();
		}
		
		CommandBase::Periodic();
		Scheduler::GetInstance()->Run();
		EndOfPeriodic();
	}
	
	virtual void TestInit() {
		if (m_autoCommand != NULL) m_autoCommand->Cancel();
		Scheduler::GetInstance()->Run();
		
		m_runMode = mTest;										
		m_periodicCount = 0;
		m_periodicLastStart = GetClock() * 1000;
		m_periodicLastEnd = m_periodicLastStart;
		m_periodicBeginTime = m_periodicLastStart;
		m_periodicTotalTime = 0;

		m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Test: Executing     ");
		m_DriverMessage->UpdateLCD();

		m_logFile = fopen("Log525.txt", "a");					// Open Log File
		LogWrite("2014 Robot Test Init");
	}

	virtual void TestPeriodic() {
		if (CommandBase::oi->GetButtonPress(11)) {				// Release the shooter
			CommandBase::ballShooter->Release();
		}
		
		
		if(CommandBase::oi->GetButtonPress(10)) {				// Put robot in starting configuration
			CommandBase::frontPickup->SetPickupMode(FrontPickup::fStart);
			CommandBase::backPickup->SetPickupMode(BackPickup::bStart);
		}
		
//		if(CommandBase::backPickup->GetBackPickupMode() == BackPickup::bStart && !CommandBase::backPickup->OnTarget()) {
//			CommandBase::backPickup->Periodic();
//		}
//		
//		if(CommandBase::frontPickup->GetFrontPickupMode() == FrontPickup::fStart && !CommandBase::frontPickup->OnTarget()) {
//			CommandBase::frontPickup->Periodic();
//		}
//
//		if (CommandBase::ballShooter->GetShootState() != BallShooter::sIdle) {
//			CommandBase::ballShooter->Periodic();
//		}
		
		CommandBase::Periodic();
		
//		printf("BackPickup ball detected %d\n", CommandBase::backPickup->HasBall());
	
	}
};

START_ROBOT_CLASS(CommandBasedRobot);

