#include "PdpData.h"
#include "MyRobot.h"
#include <cstdlib>
#include <sstream>

static void PdpLoop(PdpData *pdp) {
	PowerDistributionPanel*	panel = new PowerDistributionPanel;
	double 					amps;
	int32_t 				bit;
	int32_t					inUse = pdp->GetOutputsInUse();
	double 					loopStartTime = GetClock();

	panel->ClearStickyFaults();

	while(true) {
		if (pdp->IsEnabled()){
			pdp->SetVoltage(panel->GetVoltage());
			bit = 1;

			for (int i = 0; i < 16; i++) {
				if ((inUse & bit) > 0) {
					amps = panel->GetCurrent(i);
					if (amps <= 1.5) amps = 0;
					pdp->SetCurrent(i, amps);
				}

				bit *= 2;
				if (bit > inUse) break;
			}

			pdp->LogData();
		}

		if (pdp->GetEnergyReset()) {
			panel->ResetTotalEnergy();
			pdp->SetTotalEnergy(0);
		} else {
			pdp->SetTotalEnergy(panel->GetTotalEnergy());
		}

		Wait((loopStartTime += 0.05) - GetClock());
	}
}
PdpData::PdpData(int32_t outputsInUse) : m_task("PdpData", (FUNCPTR)PdpLoop){
	m_enabled = false;
	m_energyReset = false;
	m_outputsInUse = outputsInUse;
	m_powerCapture = false;
	m_powerAverage = m_powerPeak = 0;

	ResetPeaks();

	if(!m_task.Start((int32_t)this)) MyRobot::robotLog->Write("PDP Data Failed to Start");
}

PdpData::~PdpData() {
	m_task.Stop();
	delete &m_task;
}

double PdpData::GetAveragePower() {
	return m_powerAverage;
}

double PdpData::GetCurrent(int index) {
	if(index < 16) return m_pdpCurrent[index];
	return 0;
}

bool PdpData::GetEnergyReset() {
	if (!m_energyReset) return false;
	m_energyReset = false;
	return true;
}
double PdpData::GetLowVoltage() {
	return m_pdpLowVoltage;
}

int32_t PdpData::GetOutputsInUse() {
	return m_outputsInUse;
}

double PdpData::GetPeakCurrent(int index) {
	if (index < 16) return m_pdpPeakCurrent[index];
	return 0;
}

double PdpData::GetPeakPower() {
	return m_powerPeak;
}

double PdpData::GetTotalEnergy() {
	return m_totalEnergy;
}

double PdpData::GetVoltage() {
	return m_pdpVoltage;
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

	CapturePower();
}

void PdpData::ResetEnergy() {
	m_energyReset = true;
}

void PdpData::ResetPeaks() {
	for (int i = 0; i < 16; i++) m_pdpPeakCurrent[i] = 0;
	m_pdpLowVoltage = 99;
}

void PdpData::SetCapturePower(bool enable) {
	m_powerCapture = enable;
}

void PdpData::SetCurrent(int index, double value) {
	if(index < 16) {
		m_pdpCurrent[index] = value;
		if (m_pdpPeakCurrent[index] < value) m_pdpPeakCurrent[index] = value;
	}
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

void PdpData::SetTotalEnergy(double value) {
	m_totalEnergy = value;
}

void PdpData::SetVoltage(double value) {
	m_pdpVoltage = value;
	if (value > 0 && value < m_pdpLowVoltage) m_pdpLowVoltage = value;
}

// ******************** PRIVATE ********************

std::string PdpData::DataString(int32_t number) {
	std::stringstream ss;
	ss << number;
	return ss.str() + ",";
}

void PdpData::CapturePower() {
	static bool 	capture = false;
	static double   peak = 0;
	static double   sum = 0;
	static int		sumCount = 0;

	if (capture != m_powerCapture) {									// Change in capture status
		capture = m_powerCapture;

		if (capture) {													// Start new capture
			peak = 0;
			sum = 0;
			sumCount = 0;
		} else if(sumCount > 0) {										// Capture completed
			m_powerAverage = sum / sumCount;							// Calculate average power
			m_powerPeak = peak;
		} else {
			m_powerAverage = 0;
			m_powerPeak = 0;
		}
	}

	if (capture) {														// Capture is enabled
		double power = m_pdpVoltage * (m_pdpCurrent[4] + m_pdpCurrent[5]);
		if (peak < power) peak = power;
		sum += power; 													// Add current power to sum
		sumCount++;														// Increment count
	}
}
