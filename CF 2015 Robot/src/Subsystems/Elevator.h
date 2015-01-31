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
	typedef enum{lUp, lDown} WhichLimitSwitch;

	Elevator();
	~Elevator();
	
	int32_t GetPosition();
	bool	GetLimitSwitch(WhichLimitSwitch which);
	void	InitDefaultCommand();
	void 	RunWithJoystick(float speed);
	void	RunWithPID();
	void	SetBrake(BrakeState state);
	void	SetConstant(std::string key, int32_t value);
	void    SetElevPID();
	void	SetElevPosition(ElevPosition position);
	void	TuneElevPID();
	
private:
	void			SetSetpoint(int32_t target);

	PIDControl*		m_elevPID;
	Victor*			m_motor1;
	Victor*			m_motor2;
	AnalogInput*	m_elevPot;
	DigitalInput*	m_limitUp;
	DigitalInput*	m_limitDown;
	Solenoid*		m_elevBrake;
	
	char			m_log[100];

	bool			m_onTarget;
	int32_t			m_elevTarget;

	int32_t 		f_elevMaxPosition;
	int32_t 		f_elevReadyLift;
	int32_t 		f_elevZeroOffset;
};

#endif
