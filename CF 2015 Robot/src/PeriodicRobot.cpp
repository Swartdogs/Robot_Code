#include "PeriodicRobot.h"

#include "DriverStation.h"
#include "HAL/HAL.hpp"
#include "LiveWindow/LiveWindow.h"

PeriodicRobot::PeriodicRobot() {
	m_mode = mInit;
	m_period = 0.02;
	m_notifier = new Notifier(PeriodicRobot::CallPeriodic, this);
}

PeriodicRobot::~PeriodicRobot() {
	delete m_notifier;
}

void PeriodicRobot::CallPeriodic(void *periodicRobot) {
	PeriodicRobot* robot = (PeriodicRobot*) periodicRobot;
	robot->Periodic();
}

void PeriodicRobot::Periodic() {
	if (IsDisabled()) {
		if (m_mode != mDisabled) {
			m_mode = mDisabled;
			DisabledInit();
            Scheduler::GetInstance()->SetEnabled(false);
		}

		HALNetworkCommunicationObserveUserProgramDisabled();
		DisabledPeriodic();

	} else if (IsAutonomous()) {
		if (m_mode != mAuto) {
			m_mode = mAuto;
			AutonomousInit();
            Scheduler::GetInstance()->SetEnabled(true);
		}

		HALNetworkCommunicationObserveUserProgramAutonomous();
		AutonomousPeriodic();

	} else if (IsTest()) {
		if (m_mode != mTest) {
			m_mode = mTest;
			TestInit();
            Scheduler::GetInstance()->SetEnabled(true);
		}

		HALNetworkCommunicationObserveUserProgramTest();
		TestPeriodic();

	} else {
		if (m_mode != mTeleop) {
			m_mode = mTeleop;
			TeleopInit();
            Scheduler::GetInstance()->SetEnabled(true);
		}

		HALNetworkCommunicationObserveUserProgramTeleop();
		TeleopPeriodic();
	}
}

void PeriodicRobot::Prestart() {
}

void PeriodicRobot::SetPeriod(double period) {
	if (period <= 0) period = 0.02;
	m_period = period;
}

void PeriodicRobot::StartCompetition() {
	HALReport(HALUsageReporting::kResourceType_Framework, HALUsageReporting::kFramework_Iterative);

	RobotInit();
	m_notifier->StartPeriodic(m_period);

	HALNetworkCommunicationObserveUserProgramStarting();

	while(true) Wait(1.0);
}

void PeriodicRobot::RobotInit() {
}

void PeriodicRobot::DisabledInit() {
}

void PeriodicRobot::AutonomousInit() {
}

void PeriodicRobot::TeleopInit() {
}

void PeriodicRobot::TestInit() {
}

void PeriodicRobot::DisabledPeriodic() {
}

void PeriodicRobot::AutonomousPeriodic() {
}

void PeriodicRobot::TeleopPeriodic() {
}

void PeriodicRobot::TestPeriodic() {
}
