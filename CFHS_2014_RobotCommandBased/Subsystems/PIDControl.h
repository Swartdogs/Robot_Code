#ifndef PIDCONTROL_H_
#define PIDCONTROL_H_

class PIDControl
{
public:
	PIDControl(float p, float i, float d);
	~PIDControl();
	
	float  Calculate(float input);
	void   Reset();

	void   SetInputRange(float minimumInput, float maximumInput);
	void   SetOutputRange(float mimimumOutput, float maximumOutput);
	void   SetPID(float p, float i, float d);
	
	void   SetSetpoint(float setpoint);
	

private:
	float   m_P;					// Proportional Coefficient 
	float   m_I;					// Intergral Coeeficeint 
	float   m_D;					// Derivative Coefficen
	float   m_maximumOutput;		// Maximum Output
	float   m_minimumOutput;		// Minimum Output
	float   m_maximumInput;			// Maximum Input (Use to limit setpoint)
	float   m_minimumInput;			// Minimum Input (Use to limit setpoint)
	float   m_prevError;			// Previous Error
	double  m_totalError; 			// Sum of the errors used in the integral calc
	float   m_setpoint;				// Current Setpoint
	float   m_error;				// Current Error
	float   m_result;				// Current Output Result
};

#endif
