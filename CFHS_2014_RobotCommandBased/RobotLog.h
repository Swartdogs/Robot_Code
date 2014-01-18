#ifndef ROBOTLOG_H
#define ROBOTLOG_H

class RobotLog {
public:
	virtual void LogWrite(char* LogEntry) = 0;
};

#endif
