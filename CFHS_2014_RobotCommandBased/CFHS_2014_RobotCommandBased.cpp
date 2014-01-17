#include "WPILib.h"
#include "IterativeRobot525.h"
#include "Commands/Command.h"
#include "CommandBase.h"
#include "RobotLog.h"

class CommandBasedRobot : public IterativeRobot525, public RobotLog {
private:
	
	typedef enum {mInit, mStart, mDisabled, mAutonomous, mTeleop, mTest}RunMode;
	
	Command *autonomousCommand;
	LiveWindow *lw;
	INT32      m_periodicCount;
	FILE*      m_logFile;
	RunMode    m_runMode;
	char       m_log[100];
	double     m_periodicBeginTime;
	double     m_periodicLastEnd;
	double     m_periodicLastStart;
	double     m_periodicTotalTime;
	
	void EndOfPeriodic() {
		m_periodicCount++;
		m_periodicLastEnd = GetClock()*1000;
		
		double runTime = m_periodicLastEnd - m_periodicLastStart;
		
		m_periodicTotalTime += runTime;
		
		if(runTime > 10) {
			sprintf(m_log, "Long Periodic Duration=%f", runTime);
			LogWrite(m_log);
		}
	}
	
	void LogWrite(char* LogEntry) {
		if(m_runMode == mInit) return;
		
		if(m_periodicCount > 0) {
			fprintf(m_logFile, "%5d %5d: %s \r\n", m_periodicCount, (int)(((GetClock()*1000) - m_periodicBeginTime) / 20), LogEntry);
			printf("%5d %5d: %s \r\n", m_periodicCount, (int)(((GetClock()*1000) - m_periodicBeginTime) / 20), LogEntry);	
		} else if(m_runMode == mDisabled) {
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
		
		CommandBase::init();
		lw = LiveWindow::GetInstance();
	}
	
	virtual void DisabledInit() {
		if(m_runMode != mStart) {
			sprintf(m_log, "Periodic Usage=%5.1f %%", (m_periodicTotalTime / (GetClock()*1000 - m_periodicBeginTime)) * 100);
			LogWrite(m_log);
		}
		
		m_periodicCount = 0;
		
		
		LogWrite("-----(Name TBD)----- Disabled Init");
		fclose(m_logFile);
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
		
		autonomousCommand->Start();
		
		m_logFile = fopen("Log525.txt", "a");					// Open Log File

		//sprintf(m_log, "(Name TBD) Auto Init: Command=%d  Delay=%d", m_autoSelect, m_autoDelay * 250);
		//LogWrite(m_log);
		LogWrite("(Name TBD) Autonomous Init");
	}
	
	virtual void AutonomousPeriodic() {
		double timeNow = GetClock() * 1000;
		
		if((timeNow - m_periodicLastStart) > 100) {
			sprintf(m_log, "Delay    Last Start=%f  Last End=%f", timeNow - m_periodicLastStart, timeNow - m_periodicLastEnd);
			LogWrite(m_log);
		}
		
		m_periodicLastStart = timeNow;
		
		
		Scheduler::GetInstance()->Run();
		
		EndOfPeriodic();
	}
	
	virtual void TeleopInit() {
		// This makes sure that the autonomous stops running when
		// teleop starts running. If you want the autonomous to 
		// continue until interrupted by another command, remove
		// this line or comment it out.	
		//autonomousCommand->Cancel();
		
		m_runMode = mTeleop;									// Set Run Mode and Initialize Variables
		m_periodicCount = 0;
		m_periodicLastStart = GetClock() * 1000;
		m_periodicLastEnd = m_periodicLastStart;
		m_periodicBeginTime = m_periodicLastStart;
		m_periodicTotalTime = 0;
		
		m_logFile = fopen("Log525.txt", "a");					// Open Log File
		LogWrite("(Name TBD) Teleop Init");
	}
	
	virtual void TeleopPeriodic() {
		double timeNow = GetClock()*1000;
		
		if((timeNow - m_periodicLastStart) > 100){				// Log Periodic() Intervals > 100ms
			sprintf(m_log, "Delay    Last Start=%f  Last End=%f", timeNow - m_periodicLastStart, timeNow - m_periodicLastEnd);
			LogWrite(m_log);
		}
		
		m_periodicLastStart = timeNow;
		
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
		
		m_logFile = fopen("Log525.txt", "a");					// Open Log File
		LogWrite("(Name TBD) Test Init");
	}
	
	virtual void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(CommandBasedRobot);

