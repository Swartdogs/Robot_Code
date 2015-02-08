#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotMap.h"

class Elevator: public Subsystem {
public:
	typedef enum{pUnknown,
			     pReadyLiftBin,
				 pReadyLiftTote,
				 pCarryTote,
				 pPlaceStep,
				 pPlaceBinOnTote,
				 pPlaceToteOnOne,
				 pPlaceOnToteStep,
				 pPlaceOnTwoTote,
				 pFeederBin,
				 pFeederTote} ElevPosition;

	typedef enum{bOff, bOn} BrakeState;
	typedef enum{dUp, dDown, dDownTooFar} ElevDirection;

	Elevator();
	~Elevator();
	
	int32_t GetPosition();
	void	InitDefaultCommand();
	void 	RunWithJoystick(float joyPWM);
	void	RunWithPID(bool showPID);
	void	SetBrake(BrakeState state);
	void	SetConstant(std::string key, int32_t value);
	void    SetElevPID(ElevDirection direction);
	void	SetElevPosition(ElevPosition position);
	void	TuneElevPID();
	
private:
	void			SetSetpoint(int32_t target);

	PIDControl*		m_elevPID;
	AnalogInput*	m_elevPot;
	Solenoid*		m_elevBrake;
	
	#if (MY_ROBOT == 0)							// Otis
		VictorSP*		m_motor1;
		VictorSP*		m_motor2;
	#else										// Schumacher
		Victor*			m_motor1;
		Victor*			m_motor2;
	#endif

	ElevDirection   m_elevDirection;
	float 			m_elevPWM;
	int32_t			m_elevTarget;
	char			m_log[100];
	bool			m_onTarget;
	bool			m_rampDone;

	int32_t 		f_elevMaxPosition;
	int32_t 		f_elevReadyLift;
	int32_t 		f_elevZeroOffset;

	bool	RampPWM(float& curPWM, float pidPWM);
};

#endif
