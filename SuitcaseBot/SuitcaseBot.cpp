#include "WPILib.h"
#include "Commands/Command.h"
#include "CommandBase.h"
#include "IterativeRobot525.h"
#include "RobotLog.h"
#include "Commands/AllCommands.h"

class SuitcaseBot : public IterativeRobot525, public RobotLog {

private:
	typedef enum{mInit, mStart, mDisabled, mAutonomous, mTeleop, mTest}RunMode;
	
	CommandGroup*		m_autoCommand;				// Autonomous Command Group
	DriverStationLCD*	m_DriverMessage;			// Driver Station User Messages
	FILE*				m_logFile;					// Log File
	
	int      			m_autoDelay;				// Autonomous Start Delay Count
	int					m_autoSelect;				// Autonomous Command Group Selection
	char       			m_log[100];					// Buffer for Log Messages
	double	   			m_periodicBeginTime;		// Time at beginning of Auto/Teleop Period	
	INT32	   			m_periodicCount;			// Count of Periodic() executions
	double	   			m_periodicLastEnd;			// Time at end of last Periodic() execution
	double	   			m_periodicLastStart;		// Time at start of last Periodic() execution 
	double				m_periodicTotalTime;		// Accumulated Periodic() execution time
	RunMode             m_runMode;					// Current run mode
	
	virtual void RobotInit() {
		m_runMode = mInit;
		
		CommandBase::Init(this);								// Initialize Command Base
		SuitcaseBot::SetPeriod(0.02);							// Set Periodic() interval to 20ms
		
		m_DriverMessage = DriverStationLCD::GetInstance();		// Initialize variables	
		
		m_runMode = mStart;										
		m_periodicCount = 0;
		m_autoDelay = m_autoSelect = 0;
		m_autoCommand = NULL;
		
		m_logFile = fopen("Log525.txt", "a");					// Open Log File
		LogWrite("");
		LogWrite("SuitcaseBot Init (Build 1)");
	}
	
	virtual void DisabledInit() {
		if(m_runMode != mStart){								// Log Usage at end of Autonomous and Telelop
			sprintf(m_log, "Periodic Usage=%5.1f %%", (m_periodicTotalTime / (GetClock() * 1000 - m_periodicBeginTime)) * 100);
			LogWrite(m_log);
		}

		CommandBase::drive->StopMotors();
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

		m_DriverMessage->Printf(DriverStationLCD::kUser_Line2, 1, "Drive=%s", CommandBase::drive->GetDriveModeInfo());
		m_DriverMessage->Printf(DriverStationLCD::kUser_Line3, 1, "Dial=%s", CommandBase::dial->GetDialModeName());
		m_DriverMessage->UpdateLCD();
		
		LogWrite("SuitcaseBot Disabled Init");
		fclose(m_logFile);										// Close Log File
		m_runMode = mDisabled;
	}
	
	virtual void DisabledPeriodic() {
		if (CommandBase::oi->GetButtonPress(11)) {				// Change Drive Mode
			CommandBase::drive->SwitchDriveMode();
			m_DriverMessage->Printf(DriverStationLCD::kUser_Line2, 1, "Drive=%s", CommandBase::drive->GetDriveModeInfo());
			m_DriverMessage->UpdateLCD();
		
		} else if (CommandBase::oi->GetButtonPress(12)) {		// Change Power Level
			CommandBase::drive->ChangePowerLevel();
			m_DriverMessage->Printf(DriverStationLCD::kUser_Line2, 1, "Drive=%s", CommandBase::drive->GetDriveModeInfo());
			m_DriverMessage->UpdateLCD();
			
		} else if (CommandBase::oi->GetButtonPress(4)) {		// Change Dial Mode
			CommandBase::dial->SwitchDialMode();
			m_DriverMessage->Printf(DriverStationLCD::kUser_Line3, 1, "Dial=%s", CommandBase::dial->GetDialModeName());
			m_DriverMessage->UpdateLCD();
			
		} else if (CommandBase::oi->GetButtonPress(9)) {		// Change Autonomous Selection
			if (m_autoSelect < 4) {
				m_autoSelect++;
			} else {
				m_autoSelect = 0;
			}
			
			m_DriverMessage->Printf(DriverStationLCD::kUser_Line5, 1, "Auto=%d  Delay=%d    ", m_autoSelect, m_autoDelay * 250);
			m_DriverMessage->UpdateLCD();
		
		} else if (CommandBase::oi->GetButtonPress(10)) {		// Change Autonomous Delay		
			if (m_autoDelay < 20) {
				m_autoDelay++;
			} else {
				m_autoDelay = 0;
			}
			
			m_DriverMessage->Printf(DriverStationLCD::kUser_Line5, 1, "Auto=%d  Delay=%d    ", m_autoSelect, m_autoDelay * 250);
			m_DriverMessage->UpdateLCD();
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
			case 0: m_autoCommand = NULL;
					break;
			case 1: m_autoCommand = new AutoSquare(m_autoDelay * 0.25, 48);
					break;
			case 2: m_autoCommand = new AutoSquareDial(m_autoDelay * 0.25, 48);
					break;
		}

		if (m_autoCommand != NULL) m_autoCommand->Start();		// Start Autonomous Command Group

		m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Auto: Executing %d   ", m_autoSelect);
		m_DriverMessage->UpdateLCD();

		m_logFile = fopen("Log525.txt", "a");					// Open Log File

		sprintf(m_log, "SuitcaseBot Auto Init: Command=%d  Delay=%d", m_autoSelect, m_autoDelay * 250);
		LogWrite(m_log);
		LogWrite("SuitcaseBot Autonomous Init");
	}
	
	virtual void AutonomousPeriodic() {
		double timeNow = GetClock() * 1000;
		
		if((timeNow - m_periodicLastStart) > 100){				// Log Periodic() Intervals > 100ms
			sprintf(m_log, "Delay    Last Start=%f  Last End=%f", timeNow - m_periodicLastStart, timeNow - m_periodicLastEnd);
			LogWrite(m_log);
		}
		
		m_periodicLastStart = timeNow;							// Set Periodic Start Time

		CommandBase::dial->Periodic();
		Scheduler::GetInstance()->Run();						// Run Scheduler
		EndOfPeriodic();										// Update Periodic Variables
	}
	
	virtual void TeleopInit() {
		m_runMode = mTeleop;									// Set Run Mode and Initialize Variables
		m_periodicCount = 0;
		m_periodicLastStart = GetClock() * 1000;
		m_periodicLastEnd = m_periodicLastStart;
		m_periodicBeginTime = m_periodicLastStart;
		m_periodicTotalTime = 0;

		m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Teleop: Executing   ");
		m_DriverMessage->UpdateLCD();
	
		m_logFile = fopen("Log525.txt", "a");					// Open Log File
		LogWrite("SuitcaseBot Teleop Init");
	}
	
	virtual void TeleopPeriodic() {
		double timeNow = GetClock() * 1000;
		
		if((timeNow - m_periodicLastStart) > 100){				// Log Periodic() Intervals > 100ms
			sprintf(m_log, "Delay    Last Start=%f  Last End=%f", timeNow - m_periodicLastStart, timeNow - m_periodicLastEnd);
			LogWrite(m_log);
		}
		
		m_periodicLastStart = timeNow;							// Set Periodic Start Time

		CommandBase::oi->Periodic();
		CommandBase::dial->Periodic();
		Scheduler::GetInstance()->Run();						// Run Scheduler
		EndOfPeriodic();										// Update Periodic Variables
	}
	
	virtual void TestInit() {
		m_runMode = mTest;										
		m_periodicCount = 0;
		m_periodicLastStart = GetClock() * 1000;
		m_periodicLastEnd = m_periodicLastStart;
		m_periodicBeginTime = m_periodicLastStart;
		m_periodicTotalTime = 0;

		m_DriverMessage->Printf(DriverStationLCD::kUser_Line1, 1, "Test: Executing     ");
		m_DriverMessage->UpdateLCD();

		m_logFile = fopen("Log525.txt", "a");					// Open Log File
		LogWrite("SuitcaseBot Test Init");
	}
	
	virtual void TestPeriodic() {
		PneumaticDemo();
//		TestDial();
//		TuneRotatePID();
	}

	void EndOfPeriodic() {										
		m_periodicCount++;										// Increment Periodic Counter
		m_periodicLastEnd = GetClock() * 1000;					// Set Periodic End Time
		
		double runTime = m_periodicLastEnd - m_periodicLastStart;	// Calculate Periodic Run Time
		
		m_periodicTotalTime += runTime;							// Add Run Time to Total
		
		if(runTime  > 10){										// Log Run Times > 10ms
			sprintf(m_log, "Long Periodic Duration=%f", runTime);
			LogWrite(m_log);
		}
	}

	void LogWrite(char *LogEntry){								// Write Data to Robot Log
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
	
	void PneumaticDemo() {
		double timeNow = GetClock() * 1000;
		
		if((timeNow - m_periodicLastStart) > 100){				// Log Periodic() Intervals > 100ms
			sprintf(m_log, "Delay    Last Start=%f  Last End=%f", timeNow - m_periodicLastStart, timeNow - m_periodicLastEnd);
			LogWrite(m_log);
		}
		
		m_periodicLastStart = timeNow;							// Set Periodic Start Time

		if (CommandBase::oi->GetButtonPress(9)) {				// Switch Compressor Off
			CommandBase::pneumatic->StopCompressor();

		} else if (CommandBase::oi->GetButtonPress(10)) {		// Switch Compressor On
			CommandBase::pneumatic->StartCompressor();

		} else if (CommandBase::oi->GetButtonPress(2))	{		// Cylinder Rod In
			CommandBase::pneumatic->SetRodOut(false);
			CommandBase::pneumatic->SetRodIn(true);

		} else if (CommandBase::oi->GetButtonPress(4))  {		// Cylinder Rod Out
			CommandBase::pneumatic->SetRodIn(false);
			CommandBase::pneumatic->SetRodOut(true);
		}
		
		EndOfPeriodic();										// Update Periodic Variables
	}
	
	void TestDial() {
		float angle = CommandBase::dial->GetAngle();
		CommandBase::dial->DriveMotor(CommandBase::oi->GetTankLeft());
		printf("Dial Angle=%f \n", angle);
	}
	
	
	void TuneRotatePID() {
		static UINT8 Coefficient = 1;
		static float Dc = 0.2;			
		static float Ic = 0.005;			
		static float Pc = 0.05;
		static bool runTest = false;

		if (CommandBase::oi->GetButtonPress(10)) {
			CommandBase::drive->ResetGyro();
			printf("Gyro Reset\n");
			
		} else if (CommandBase::oi->GetButtonPress(4)) {
			Coefficient++;
			if (Coefficient > 3) Coefficient = 1;
			switch (Coefficient) {
				case 1:
					printf("Adjust P coefficient\n");
					break;
				case 2:
					printf("Adjust I coefficient\n");
					break;
				case 3:
					printf("Adjust D coefficient\n");
					break;
			}
			
		} else if (CommandBase::oi->GetButtonPress(3)) {
			switch(Coefficient) {
				case 1: Pc = Pc + 0.001;
						break;
				case 2: Ic = Ic + 0.0001;
						break;
				case 3: Dc = Dc + 0.01;
						break;
			}

			printf("Coefficients: P=%f  I=%f  D=%f \n", Pc, Ic, Dc);
				
		} else if (CommandBase::oi->GetButtonPress(1)) {
			switch(Coefficient) {
				case 1: Pc = Pc - 0.001;
						break;
				case 2: Ic = Ic - 0.0001;
						break;
				case 3: Dc = Dc - 0.01;
						break;
			}

			printf("Coefficients: P=%f  I=%f  D=%f \n", Pc, Ic, Dc);
		
		} else if (CommandBase::oi->GetButton(2)) {
			if (!runTest) {
				runTest = true;
				CommandBase::drive->SetPID(Pc, Ic, Dc);
				CommandBase::drive->InitRotate(0, false);
			}
			
			CommandBase::drive->ExecuteRotate();
			printf("Gyro Angle=%f\n", CommandBase::drive->GetGyroAngle());
			
		} else {
			if (runTest) {
				runTest = false;
				CommandBase::drive->DriveArcade(0, 0);
				CommandBase::drive->SetSafetyEnabled(false);
			}
			
			CommandBase::drive->DriveTank(CommandBase::oi->GetTankLeft(), CommandBase::oi->GetTankRight());
		}
	}
};

START_ROBOT_CLASS(SuitcaseBot);

