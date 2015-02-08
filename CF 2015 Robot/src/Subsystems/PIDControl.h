#ifndef PIDCONTROL_H_
#define PIDCONTROL_H_

class PIDControl {
public:
	PIDControl();
	~PIDControl();

	float	Calculate(float input, bool showResults);
	void	Reset();
	void 	SetCoefficient(char kWhich, float errorThreshold, float kAbove, float kBelow);
	void	SetInputRange(float minimumInput, float maximumInput);
	void	SetOutputRange(float minimumOutput, float maximumOutput);
	void	SetSetpoint(float setpoint);

private:
	struct Coefficient {
		float		threshold;
		float		kAbove;
		float		kBelow;
		float		kNow;
	};

	Coefficient	m_P;
	Coefficient m_I;
	Coefficient m_D;

	float	m_maximumOutput;
	float 	m_minimumOutput;
	float	m_maximumInput;
	float	m_minimumInput;
	float	m_prevError;
	bool	m_prevReset;
	double	m_totalError;
	float	m_setpoint;
};

#endif







