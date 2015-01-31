#ifndef PDPDATA_H_
#define PDPDATA_H_

#include "WPILib.h"
#include "RobotLog.h"

class PdpData
{
public:
	PdpData();
	~PdpData();

	double		GetCurrent(int index);
	double		GetPeak(int index);
	double		GetVoltage();
	double		GetLowVoltage();
	bool		IsEnabled();
	void		LogData();
	void		ResetPeaks();
	void		SetCurrent(int index, double value);
	void		SetEnabled(bool enable);
	void		SetLogEnabled(bool enable);
	void		SetVoltage(double value);

private:
	Task		m_task;
	FILE*		m_logFile;

	bool		m_enabled;
	char		m_log[100];
	bool		m_logEnabled;
	double		m_pdpVoltage;
	double		m_pdpLowVoltage;
	double		m_pdpCurrent[16];
	double		m_pdpPeakCurrent[16];

	std::string DataString(int32_t number);
};
#endif
