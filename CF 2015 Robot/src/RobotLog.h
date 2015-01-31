/*
 * RobotLog.h
 *
 *  Created on: Jan 10, 2015
 *      Author: Anton
 */

#ifndef ROBOTLOG_H_
#define ROBOTLOG_H_

#include "WPILib.h"

class RobotLog {
public:
	typedef enum {mInit, mDisabled, mAutonomous, mTeleop, mTest}RobotMode;

	RobotLog();
	~RobotLog();

	void Close();
	void EndPeriodic();
	void SetMode(RobotMode mode);
	void StartPeriodic();
	void Write(std::string entry);

private:
	char		m_log[100];
	FILE*		m_logFile;

	double		m_periodicBeginTime;
	int32_t		m_periodicCount;
	double		m_periodicLastEnd;
	double		m_periodicLastStart;
	double		m_periodicTotalTime;
	RobotMode	m_robotMode;
};

#endif /* SRC_ROBOTLOG_H_ */
