#ifndef PERIODIC_ROBOT_H_
#define PERIODIC_ROBOT_H_

#include "Timer.h"
#include "RobotBase.h"
#include "Notifier.h"

class PeriodicRobot: public RobotBase {
public:
	virtual void StartCompetition();

	virtual void RobotInit();
	virtual void DisabledInit();
	virtual void AutonomousInit();
	virtual void TeleopInit();
	virtual void TestInit();

	virtual void DisabledPeriodic();
	virtual void AutonomousPeriodic();
	virtual void TeleopPeriodic();
	virtual void TestPeriodic();

	void SetPeriod(double period);

protected:
	PeriodicRobot();
	virtual ~PeriodicRobot();

	virtual void Prestart();
	void Periodic();

private:
	typedef enum {mInit, mDisabled, mAuto, mTeleop, mTest} RobotMode;

	double 		m_period;
	RobotMode	m_mode;
	Notifier*   m_notifier;

	static void CallPeriodic(void *periodicRobot);
};

#endif
