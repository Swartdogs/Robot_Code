#include "PdpData.h"
#include "MyRobot.h"
#include <cstdlib>
#include <sstream>

static void PdpLoop(PdpData *pdp) {
	static PowerDistributionPanel* panel = new PowerDistributionPanel;
	double amps;
	double loopStartTime = GetClock();

	while(true) {
		if (pdp->IsEnabled()){
			pdp->SetVoltage(panel->GetVoltage());

				for (int i = 0; i < 16; i++) {
					amps = panel->GetCurrent(i);
					if (amps <= 1.5) amps = 0;
					pdp->SetCurrent(i, amps);
				}

			pdp->LogData();
		}

		Wait((loopStartTime += 0.05) - GetClock());
	}
}
PdpData::PdpData() : m_task("PdpData", (FUNCPTR)PdpLoop){
	m_enabled = false;
	ResetPeaks();

	if(!m_task.Start((int32_t)this)) MyRobot::robotLog->Write("PDP Data Failed to Start");
}

PdpData::~PdpData() {
	m_task.Stop();
	delete &m_task;
}

double PdpData::GetCurrent(int index) {
	if(index < 16) return m_pdpCurrent[index];
	return 0;
}

double PdpData::GetPeak(int index) {
	if (index < 16) return m_pdpPeakCurrent[index];
	return 0;
}

double PdpData::GetVoltage() {
	return m_pdpVoltage;
}

double PdpData::GetLowVoltage() {
	return m_pdpLowVoltage;
}

bool PdpData::IsEnabled() {
	return m_enabled;
}

void PdpData::LogData() {
	std::string data;

	if(m_logEnabled) {
		data = DataString((int32_t)(m_pdpVoltage *100 + 0.5));
		for(int i = 0; i < 16; i++) data += DataString((int32_t)(m_pdpCurrent[i] * 10 + 0.5));
		fprintf(m_logFile, "%s \r\n", data.c_str());
	}
}

void PdpData::ResetPeaks() {
	for (int i = 0; i < 16; i++) m_pdpPeakCurrent[i] = 0;
	m_pdpLowVoltage = 99;
}

void PdpData::SetCurrent(int index, double value) {
	if(index < 16) {
		m_pdpCurrent[index] = value;
		if (m_pdpPeakCurrent[index] < value) m_pdpPeakCurrent[index] = value;
	}
}

void PdpData::SetVoltage(double value) {
	m_pdpVoltage = value;
	if (value > 0 && value < m_pdpLowVoltage) m_pdpLowVoltage = value;
}

void PdpData::SetEnabled(bool enable) {
	m_enabled = enable;
}

void PdpData::SetLogEnabled(bool enable) {
	m_logEnabled = enable;

	if(enable) {
		m_logFile = fopen("/home/lvuser/PdpLog.txt", "w");

	} else if (m_logFile != NULL) {
		fclose(m_logFile);
		m_logFile = NULL;
	}
}

// ******************** PRIVATE ********************

std::string PdpData::DataString(int32_t number) {
	std::stringstream ss;
	ss << number;
	return ss.str() + ",";

}
