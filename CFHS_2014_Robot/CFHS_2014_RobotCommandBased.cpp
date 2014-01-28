#include "WPILib.h"
#include "IterativeRobot525.h"
#include "Commands/Command.h"
#include "CommandBase.h"
#include "RobotLog.h"

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

	virtual void RobotInit() {
		m_runMode = mInit;
		
		CommandBasedRobot::SetPeriod(0.02);
		CommandBase::Init(this);

		m_DriverMessage = DriverStationLCD::GetInstance();	

		m_runMode = mStart;
		m_periodicCount = 0;
		m_autoDelay = m_autoSelect = 0;
		m_autoCommand = NULL;

		m_logFile = fopen("Log525.txt", "a");
		LogWrite("");
		LogWrite("2014 Robot Init (Build 1)");
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

		m_DriverMessage->UpdateLCD();
		
		LogWrite("2014 Robot Disabled Init");
		fclose(m_logFile);										// Close Log File
		m_runMode = mDisabled;
	}

	virtual void DisabledPeriodic() {
		
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
		}
		
		if (m_autoCommand != NULL) m_autoCommand->Start();		// Start Autonomous Command Group

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

		Scheduler::GetInstance()->Run();
		EndOfPeriodic();
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
	}
};

START_ROBOT_CLASS(CommandBasedRobot);

