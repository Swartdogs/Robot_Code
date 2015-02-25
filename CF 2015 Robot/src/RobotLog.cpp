#include "RobotLog.h"
#include "WPILib.h"

RobotLog::RobotLog() {
	m_robotMode = mInit;
	m_periodicBeginTime = 0;
	m_periodicCount = 0;
	m_periodicLastEnd = 0;
	m_periodicLastStart = 0;
	m_periodicTotalTime = 0;
	m_logFile = NULL;
}
RobotLog::~RobotLog() {
}

void RobotLog::Close() {
	fclose(m_logFile);
	m_logFile = NULL;
}

void RobotLog::EndPeriodic() {
	m_periodicCount++;
	m_periodicLastEnd = GetClock() *1000;

	double runTime = m_periodicLastEnd - m_periodicLastStart;

	m_periodicTotalTime += runTime;

	if(runTime > 10.0) {
		sprintf(m_log, "Schumacher: Long Periodic Execution Time=%5.1f (Start-to-End)", runTime);
		Write(m_log);
	}
}

RobotLog::RobotMode RobotLog::GetMode() {
	return m_robotMode;
}

void RobotLog::SetMode(RobotMode mode) {
	m_periodicCount = 0;

	if(m_robotMode == mAutonomous || m_robotMode == mTeleop) {
		sprintf(m_log, "Schumacher: Periodic Usage=%5.1f %%", (m_periodicTotalTime / (GetClock() *1000 - m_periodicBeginTime)) *100);
		Write(m_log);
	}

	m_robotMode = mode;
	m_periodicLastStart = GetClock() *1000;
	m_periodicLastEnd = m_periodicLastStart;
	m_periodicBeginTime = m_periodicLastStart;
	m_periodicTotalTime = 0;
}

void RobotLog::StartPeriodic() {
	static bool inBrownOut = false;
	double timeNow = GetClock() *1000;

	if((timeNow- m_periodicLastStart) > 100.0){
		sprintf(m_log, "Schumacher: Long Periodic Interval=%5.1f (Start-to-Start)", timeNow - m_periodicLastStart);
		Write(m_log);
	}

	m_periodicLastStart = timeNow;

	bool brownOut = DriverStation::GetInstance()->IsSysBrownedOut();

	if (inBrownOut != brownOut) {
		inBrownOut = brownOut;
		if (inBrownOut) Write("Schumacher: System Brown Out");
	}
}

void RobotLog::Write(std::string entry) {
	const char* cEntry=entry.c_str();
	int timeDiff = m_periodicCount - (int)(((GetClock() * 1000) - m_periodicBeginTime) / 20);

	if (m_periodicCount > 0) {
		printf("%5d %+4d: %s \n", m_periodicCount, timeDiff, cEntry );
	} else {
		printf("%s \n", cEntry);
	}

	if (m_logFile == NULL) m_logFile = fopen("/home/lvuser/Log525.txt", "a");

	if (m_logFile != NULL) {
		if(m_periodicCount > 0) {
			fprintf(m_logFile, "%5d %+4d: %s \r\n", m_periodicCount, timeDiff, cEntry );
		} else {
			fprintf(m_logFile,"%s \r\n", cEntry);
		}
	}
}



