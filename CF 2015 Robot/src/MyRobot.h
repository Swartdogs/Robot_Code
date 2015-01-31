#ifndef MYROBOT_H_
#define MYROBOT_H_

#include "WPILib.h"
#include "Commands/Command.h"
#include "Commands/AllCommands.h"
#include "PeriodicRobot.h"
#include "RobotMap.h"
#include <cstdlib>

#include "OI.h"
#include "PdpData.h"
#include "RobotLog.h"
#include "TcpHost.h"

#include "Subsystems/Drive.h"
#include "Subsystems/Elevator.h"

class MyRobot: public PeriodicRobot {
public:
	static TcpHost*     dashboard;
	static Drive*		drive;
	static OI*			oi;
	static PdpData*		powerPanel;
	static RobotLog*	robotLog;
	static Elevator*	elevator;

private:
	void RobotInit();
	void DisabledInit();
	void DisabledPeriodic();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void TestInit();
	void TestPeriodic();

	void IniParser();
	void SetDashRunData();
	void SetDashSensorData();

	CommandGroup*	m_autoCommand;
	int				m_autoDelay;
	int				m_autoSelect;
	char			m_log[100];
};

#endif
