#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotMap.h"

class Elevator: public Subsystem {
public:
	typedef enum {pUnknown,
			      pBinLoad,
			      pBinRelease,
				  pFeederLoad,
				  pLiftFromTote,
				  pLiftFromFloor,
				  pCarry,
				  pStep,
				  pStepUnload,
				  pStepTote,
				  pStepToteUnload,
				  pPlatform} ElevPosition;

	typedef enum {sOff, sOn} SolenoidState;
	typedef enum {dUp, dDown, dDownToLoad, dDownTooFar} ElevDirection;
	typedef enum {potLeft, potRight, potNone} ElevPot;

	Elevator();
	~Elevator();
	
	int32_t GetPosition(ElevPot pot);
	void	IncrementSetpoint(ElevDirection direction);
	void	InitDefaultCommand();
	bool	IsOnTarget();
	void    LogPotInUse();
	void 	RunWithJoystick(float joyPWM);
	void	RunPID(bool showPID);
	void	SetBrake(SolenoidState state);
	void	SetConstant(std::string key, int32_t value);
	void    SetElevPID(ElevDirection direction);
	void	SetElevPosition(ElevPosition position);
	void	SetRunPID(bool run);
	void	SetToteCenter(SolenoidState state);
	void	SetToteEject(SolenoidState state);
	void    StopMotors();
	void	TuneElevPID();
	
private:
	typedef enum {mStop, mDown, mUp} CheckMove;
	typedef enum {cDone, cUnknown, cExpected, cUnexpected} CheckState;

	Solenoid*				m_elevBrake;
	PIDControl*				m_elevPID;
	DigitalInput*			m_elevPotCheck;
	AnalogInput*			m_elevPotL;
	AnalogInput*    		m_elevPotR;
	DigitalInput*			m_elevTipCheck;
	Solenoid*				m_toteCenter;
	Solenoid*				m_toteEject;

	#if (MY_ROBOT == 0)							// Schumacher
		VictorSP*		m_motor1;
		VictorSP*		m_motor2;
	#else										// Otis
		Victor*			m_motor1;
		Victor*			m_motor2;
	#endif

	SolenoidState	m_brakeState;
	ElevDirection   m_elevDirection;
	float 			m_elevPWM;
	int32_t			m_elevTarget;
	char			m_log[100];
	bool			m_onTarget;
	ElevPot         m_potInUse;
	int				m_potStatus[2];
	bool			m_rampDone;
	bool			m_runPID;
	SolenoidState	m_toteCenterState;
	SolenoidState	m_toteEjectState;

	int32_t 		f_potCheckDown;
	int32_t			f_potCheckUp;
	int32_t 		f_potZeroOffsetL;
	int32_t         f_potZeroOffsetR;
	int32_t 		f_elevMaxPosition;
	int32_t			f_toteCenterMin;
	int32_t			f_toteEjectAbove;
	int32_t    		f_toteEjectBelow;

	int32_t 		f_binLoad;
	int32_t			f_binRelease;
	int32_t			f_feederLoad;
	int32_t			f_liftFromTote;
	int32_t			f_liftFromFloor;
	int32_t			f_carry;
	int32_t			f_step;
	int32_t			f_stepUnload;
	int32_t			f_stepTote;
	int32_t			f_stepToteUnload;
	int32_t			f_platform;

	void    	ElevPotCheck(CheckMove move);
	std::string	ElevPotName(int index);
	bool		RampPWM(float& curPWM, float pidPWM);
	void  		SetSetpoint(int32_t target);
	void        UpdatePotStatus();
};

#endif
