#include "PIDControl.h"
#include <math.h>
#include <stdio.h>

PIDControl::PIDControl() {
	m_P.threshold = 0;
	m_P.kAbove = 0;
	m_P.kBelow = 0;
	m_P.kNow = 0;

	m_I.threshold = 0;
	m_I.kAbove = 0;
	m_I.kBelow = 0;
	m_I.kNow = 0;

	m_D.threshold = 0;
	m_D.kAbove = 0;
	m_D.kBelow = 0;
	m_D.kNow = 0;

	m_maximumOutput = 1.0;
	m_minimumOutput = -1.0;
	m_maximumInput = 0;
	m_minimumInput = 0;
	m_setpoint = 0;
	m_prevError = 0;
	m_prevReset = true;
	m_totalError = 0;
}

PIDControl::~PIDControl() {
}

float PIDControl::Calculate(float input, bool showResults) {		// Calculate output based on input
	float error = m_setpoint - input;								// Current Error
	float errorDiff = error - m_prevError;							// Difference between current error and previous error

	if (m_prevReset) {
		m_prevReset = false;
		errorDiff = 0;
	}

	if (m_P.threshold > 0) {										// Compare error to threshold for variable coefficients
		if (fabs(error) < m_P.threshold) {
			m_P.kNow = m_P.kBelow;
		} else {
			m_P.kNow = m_P.kAbove;
		}
	}

	if (m_I.threshold > 0) {
		if (fabs(error) < m_I.threshold) {
			if (m_I.kNow != m_I.kBelow) {
				m_I.kNow = m_I.kBelow;
				m_totalError = 0;
			}
		} else if (m_I.kNow != m_I.kAbove) {
			m_I.kNow = m_I.kAbove;
			m_totalError = 0;
		}
	}

	if (m_D.threshold > 0) {
		if (fabs(error) < m_D.threshold) {
			m_D.kNow = m_D.kBelow;
		} else {
			m_D.kNow = m_D.kAbove;
		}
	}

	if (m_I.kNow != 0) {											// If non-zero I coefficient
		if (error < 0) {											// Reset TotalError when Error changes direction
			if (m_totalError > 0) m_totalError = 0;
		} else if (error > 0) {
			if (m_totalError < 0) m_totalError = 0;
		}

		double iGain = (m_totalError + error) * m_I.kNow;			// Calculate potential I gain

		if (iGain < m_maximumOutput) {								// I Gain < Maximum Output
			if (iGain > m_minimumOutput) {							// I Gain > Minimum Output
				m_totalError += error;								// Add current error
			} else {
				m_totalError = m_minimumOutput / m_I.kNow;			// Set I Gain = Minimum Output
			}
		} else {
			m_totalError = m_maximumOutput / m_I.kNow;				// Set I Gain = Maximum Output
		}
	}
																	
	float output = m_P.kNow * error + m_I.kNow * m_totalError + m_D.kNow * errorDiff;

	if (output > m_maximumOutput) {									// Limit result to within Minimum-Maximum range
		output = m_maximumOutput;
	} else if (output < m_minimumOutput) {
		output = m_minimumOutput;
	}

	if (showResults) {												// Show Results
		printf("PID Calculate: Error=%7.1f  P=%6.2f  I=%6.2f  D=%6.2f  Output=%6.2f\n", error, m_P.kNow * error,
				m_I.kNow * m_totalError, m_D.kNow * errorDiff, output);
	}

	m_prevError = error;											// Set Previous error for next calculation

	return output;
}

void PIDControl::Reset() {
	m_prevReset = true;
	m_totalError = 0;
}

void PIDControl::SetCoefficient(char kWhich, float threshold, float kAbove, float kBelow) {
	switch(kWhich) {
		case 68:													// D Coefficient
		case 100:
			m_D.threshold = threshold;
			m_D.kAbove = kAbove;
			m_D.kBelow = kBelow;
			m_D.kNow = kAbove;
			break;

		case 73:													// I Coefficient
		case 105:
			m_I.threshold = threshold;
			m_I.kAbove = kAbove;
			m_I.kBelow = kBelow;
			m_I.kNow = kAbove;
			break;

		case 80:													// P Coefficient
		case 112:
			m_P.threshold = threshold;
			m_P.kAbove = kAbove;
			m_P.kBelow = kBelow;
			m_P.kNow = kAbove;
			break;

		default:;
	}
}

void PIDControl::SetInputRange(float minimumInput, float maximumInput) {
	m_minimumInput = minimumInput;
	m_maximumInput = maximumInput;
}

void PIDControl::SetOutputRange(float minimumOutput, float maximumOutput) {
	m_minimumOutput = minimumOutput;
	m_maximumOutput = maximumOutput;
}

void PIDControl::SetSetpoint(float setpoint) {
	if (m_maximumInput > m_minimumInput) {
		if (setpoint > m_maximumInput) {
			m_setpoint = m_maximumInput;
		} else if (setpoint < m_minimumInput) {
			m_setpoint = m_minimumInput;
		} else {
			m_setpoint = setpoint;
		}
	} else {
		m_setpoint = setpoint;
	}
}





