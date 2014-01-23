
#include "PIDControl.h"
#include <math.h>

PIDControl::PIDControl(float Kp, float Ki, float Kd) {
	m_P = Kp;
	m_I = Ki;
	m_D = Kd;

	m_maximumOutput = 1.0;
	m_minimumOutput = -1.0;
	m_maximumInput = 0;
	m_minimumInput = 0;
	
	m_setpoint = 0;

	m_prevError = 0;
	m_totalError = 0;
	m_result = 0;
}

PIDControl::~PIDControl() {
	
}

float PIDControl::Calculate(float input) {							// Calculate output based on input
	double potentialIGain;
	
	m_error = m_setpoint - input;									// Current Error

	if (m_I != 0) {
		
		if (m_error < 0) {											// Reset Total Error when Error changes direction
			if (m_totalError > 0) m_totalError = 0;
		} else if (m_error > 0) {
			if (m_totalError < 0) m_totalError = 0;
		}
		
		potentialIGain = (m_totalError + m_error) * m_I;			// Calculate potential Integral gain
		
		if (potentialIGain < m_maximumOutput) {						// I Gain < Max Output
			if (potentialIGain > m_minimumOutput)					// I Gain > Min Output
				m_totalError += m_error;							// Integrate current Error
			else
				m_totalError = m_minimumOutput / m_I;				// Set I Gain = Min Output
		} else {
			m_totalError = m_maximumOutput / m_I;					// Set I Gain = Max Output
		}
	}

	m_result = m_P * m_error + m_I * m_totalError + m_D * (m_error - m_prevError);	// Calculate Result
	m_prevError = m_error;										// Set Previous Error for next calculation
	
	if (m_result > m_maximumOutput) {							// Limit result to Maximum Output
		m_result = m_maximumOutput;
	} else if (m_result < m_minimumOutput) { 					// Limit result to Minimum Output
		m_result = m_minimumOutput;
	}


	return m_result;
}

void PIDControl::Reset() {										// RESET CALCULATION DATA
	m_prevError = 0;
	m_totalError = 0;
	m_result = 0;
}

void PIDControl::SetInputRange(float minimumInput, float maximumInput)	{		// SET INPUT RANGE
	m_minimumInput = minimumInput;
	m_maximumInput = maximumInput;
}

void PIDControl::SetOutputRange(float minimumOutput, float maximumOutput) {		// SET OUTPUT RANGE
	m_minimumOutput = minimumOutput;
	m_maximumOutput = maximumOutput;
}

void PIDControl::SetPID(float p, float i, float d) {			// SET CALCULATION COEFFICENTS
	m_P = p;
	m_I = i;
	m_D = d;
}

void PIDControl::SetSetpoint(float setpoint) {					// SET CALCULATION SETPOINT
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
