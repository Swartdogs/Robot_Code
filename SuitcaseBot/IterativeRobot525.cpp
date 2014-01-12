/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "IterativeRobot525.h"

#include "DriverStation.h"
#include "NetworkCommunication/UsageReporting.h"
#include <taskLib.h>
#include "SmartDashboard/SmartDashboard.h"
#include "LiveWindow/LiveWindow.h"
#include "networktables/NetworkTable.h"

const double IterativeRobot525::kDefaultPeriod;

/**
 * Constructor for RobotIterativeBase
 * 
 * The constructor initializes the instance variables for the robot to indicate
 * the status of initialization for disabled, autonomous, teleop, and test code.
 */
IterativeRobot525::IterativeRobot525()
	: m_disabledInitialized (false)
	, m_autonomousInitialized (false)
	, m_teleopInitialized (false)
	, m_testInitialized (false)
	, m_period (kDefaultPeriod)
{
	m_watchdog.SetEnabled(false);
}

/**
 * Free the resources for a RobotIterativeBase class.
 */
IterativeRobot525::~IterativeRobot525()
{
}

/**
 * Set the period for the periodic functions.
 * 
 * @param period The period of the periodic function calls.  0.0 means sync to driver station control data.
 */
void IterativeRobot525::SetPeriod(double period)
{
	if (period > 0.0)
	{
		// Not syncing with the DS, so start the timer for the main loop
		m_mainLoopTimer.Reset();
		m_mainLoopTimer.Start();
	}
	else
	{
		// Syncing with the DS, don't need the timer
		m_mainLoopTimer.Stop();
	}
	m_period = period;
}

/**
 * Get the period for the periodic functions.
 * Returns 0.0 if configured to syncronize with DS control data packets.
 * @return Period of the periodic function calls
 */
double IterativeRobot525::GetPeriod()
{
	return m_period;
}

/**
 * Get the number of loops per second for the IterativeRobot
 * @return Frequency of the periodic function calls
 */
double IterativeRobot525::GetLoopsPerSec()
{
	// If syncing to the driver station, we don't know the rate,
	//   so guess something close.
	if (m_period <= 0.0)
		return 50.0;
	return 1.0 / m_period;
}

/**
 * Provide an alternate "main loop" via StartCompetition().
 * 
 * This specific StartCompetition() implements "main loop" behavior like that of the FRC
 * control system in 2008 and earlier, with a primary (slow) loop that is
 * called periodically, and a "fast loop" (a.k.a. "spin loop") that is 
 * called as fast as possible with no delay between calls. 
 */
void IterativeRobot525::StartCompetition()
{
	static bool waitForNext = false;
	
	nUsageReporting::report(nUsageReporting::kResourceType_Framework, nUsageReporting::kFramework_Iterative);

	RobotInit();

	// loop forever, calling the appropriate mode-dependent function
	while (true)
	{
		// Call the appropriate function depending upon the current robot mode
		if (IsDisabled())
		{
			// call DisabledInit() if we are now just entering disabled mode from
			// either a different mode or from power-on
			if(!m_disabledInitialized)
			{
				DisabledInit();
				m_disabledInitialized = true;
				// reset the initialization flags for the other modes
				m_autonomousInitialized = false;
                m_teleopInitialized = false;
                m_testInitialized = false;
			}
			if (NextPeriodReady())
			{
				FRC_NetworkCommunication_observeUserProgramDisabled();
				DisabledPeriodic();
				waitForNext = true;
			}
		}
		else if (IsAutonomous())
		{
			// call AutonomousInit() if we are now just entering autonomous mode from
			// either a different mode or from power-on
			if(!m_autonomousInitialized)
			{
				AutonomousInit();
				m_autonomousInitialized = true;
				// reset the initialization flags for the other modes
				m_disabledInitialized = false;
                m_teleopInitialized = false;
                m_testInitialized = false;
			}
			if (NextPeriodReady())
			{
				FRC_NetworkCommunication_observeUserProgramAutonomous();
				AutonomousPeriodic();
				waitForNext = true;
			}
		}
        else if (IsTest())
        {
            // call TestInit() if we are now just entering test mode from
            // either a different mode or from power-on
            if(!m_testInitialized)
            {
                TestInit();
                m_testInitialized = true;
                // reset the initialization flags for the other modes
                m_disabledInitialized = false;
                m_autonomousInitialized = false;
                m_teleopInitialized = false;
            }
            if (NextPeriodReady())
            {
                FRC_NetworkCommunication_observeUserProgramTest();
                TestPeriodic();
                waitForNext = true;
            }
        }
		else
		{
			// call TeleopInit() if we are now just entering teleop mode from
			// either a different mode or from power-on
			if(!m_teleopInitialized)
			{
				TeleopInit();
				m_teleopInitialized = true;
				// reset the initialization flags for the other modes
				m_disabledInitialized = false;
                m_autonomousInitialized = false;
                m_testInitialized = false;
                Scheduler::GetInstance()->SetEnabled(true);
			}
			if (NextPeriodReady())
			{
				FRC_NetworkCommunication_observeUserProgramTeleop();
				TeleopPeriodic();
				waitForNext = true;
			}
		}

		if(waitForNext) {
			waitForNext = false;
			
			if (m_period > 0.0) {
				Wait(m_period - m_mainLoopTimer.Get());
			} else {
				m_ds->WaitForData();
			}
		}
	}	
}

/**
 * Determine if the periodic functions should be called.
 *
 * If m_period > 0.0, call the periodic function every m_period as compared
 * to Timer.Get().  If m_period == 0.0, call the periodic functions whenever
 * a packet is received from the Driver Station, or about every 20ms.
 *
 * @todo Decide what this should do if it slips more than one cycle.
 */

bool IterativeRobot525::NextPeriodReady()
{
	if (m_period > 0.0)
	{
		return m_mainLoopTimer.HasPeriodPassed(m_period);
	}
	else
	{
		return m_ds->IsNewControlData();
	}
}

/**
 * Robot-wide initialization code should go here.
 * 
 * Users should override this method for default Robot-wide initialization which will
 * be called when the robot is first powered on.  It will be called exactly 1 time.
 */
void IterativeRobot525::RobotInit()
{
	printf("Default %s() method... Overload me!\n", __FUNCTION__);
}

/**
 * Initialization code for disabled mode should go here.
 * 
 * Users should override this method for initialization code which will be called each time
 * the robot enters disabled mode.
 */
void IterativeRobot525::DisabledInit()
{
	printf("Default %s() method... Overload me!\n", __FUNCTION__);
}

/**
 * Initialization code for autonomous mode should go here.
 * 
 * Users should override this method for initialization code which will be called each time
 * the robot enters autonomous mode.
 */
void IterativeRobot525::AutonomousInit()
{
	printf("Default %s() method... Overload me!\n", __FUNCTION__);
}

/**
 * Initialization code for teleop mode should go here.
 * 
 * Users should override this method for initialization code which will be called each time
 * the robot enters teleop mode.
 */
void IterativeRobot525::TeleopInit()
{
    printf("Default %s() method... Overload me!\n", __FUNCTION__);
}

/**
 * Initialization code for test mode should go here.
 * 
 * Users should override this method for initialization code which will be called each time
 * the robot enters test mode.
 */
void IterativeRobot525::TestInit()
{
    printf("Default %s() method... Overload me!\n", __FUNCTION__);
}

/**
 * Periodic code for disabled mode should go here.
 * 
 * Users should override this method for code which will be called periodically at a regular
 * rate while the robot is in disabled mode.
 */
void IterativeRobot525::DisabledPeriodic()
{
	static bool firstRun = true;
	if (firstRun)
	{
		printf("Default %s() method... Overload me!\n", __FUNCTION__);
		firstRun = false;
	}
	taskDelay(1);
}

/**
 * Periodic code for autonomous mode should go here.
 *
 * Users should override this method for code which will be called periodically at a regular
 * rate while the robot is in autonomous mode.
 */
void IterativeRobot525::AutonomousPeriodic()
{
	static bool firstRun = true;
	if (firstRun)
	{
		printf("Default %s() method... Overload me!\n", __FUNCTION__);
		firstRun = false;
	}
	taskDelay(1);
}

/**
 * Periodic code for teleop mode should go here.
 *
 * Users should override this method for code which will be called periodically at a regular
 * rate while the robot is in teleop mode.
 */
void IterativeRobot525::TeleopPeriodic()
{
	static bool firstRun = true;
	if (firstRun)
	{
		printf("Default %s() method... Overload me!\n", __FUNCTION__);
		firstRun = false;
	}
	taskDelay(1);
}

/**
 * Periodic code for test mode should go here.
 *
 * Users should override this method for code which will be called periodically at a regular
 * rate while the robot is in test mode.
 */
void IterativeRobot525::TestPeriodic()
{
    static bool firstRun = true;
    if (firstRun)
    {
        printf("Default %s() method... Overload me!\n", __FUNCTION__);
        firstRun = false;
    }
    taskDelay(1);
}

