#ifndef PDPDATA_H_
#define PDPDATA_H_

#include "WPILib.h"
#include "RobotLog.h"

class PdpData
{
public:
	PdpData(int32_t outputsInUse);			// outputsInUse is a bit map of PDP outputs that are wired
	~PdpData();

	double		GetAveragePower();
	double		GetCurrent(int index);
	bool		GetEnergyReset();
	double		GetLowVoltage();
	int32_t		GetOutputsInUse();
	double		GetPeakCurrent(int index);
	double      GetPeakPower();
	double		GetTotalEnergy();
	double		GetVoltage();
	bool		IsEnabled();
	void		LogData();
	void		ResetEnergy();
	void		ResetPeaks();
	void		SetCapturePower(bool enable);
	void		SetCurrent(int index, double value);
	void		SetEnabled(bool enable);
	void		SetLogEnabled(bool enable);
	void		SetTotalEnergy(double value);
	void		SetVoltage(double value);

private:
	Task		m_task;
	FILE*		m_logFile;

	bool		m_enabled;
	bool 		m_energyReset;
	char		m_log[100];
	bool		m_logEnabled;
	int32_t     m_outputsInUse;
	double		m_pdpVoltage;
	double		m_pdpLowVoltage;
	double		m_pdpCurrent[16];
	double		m_pdpPeakCurrent[16];
	double		m_powerAverage;
	bool		m_powerCapture;
	double      m_powerPeak;
	double		m_totalEnergy;

	void        CapturePower();
	std::string DataString(int32_t number);
};
#endif
