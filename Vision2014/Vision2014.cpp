#include "WPILib.h"
#include "IterativeRobot525.h"
#include "Commands/Command.h"
#include "CommandBase.h"

class Vision2014 : public IterativeRobot525 {
private:
	Command *autonomousCommand;
//	LiveWindow *lw;
	
	virtual void RobotInit() {
		CommandBase::init();
		autonomousCommand = NULL;
	}
	
	virtual void DisabledInit() {
		
	}
	
	virtual void DisabledPeriodic() {
		static int CameraStartDelay = 250;
		
		if (CameraStartDelay > 0) {							// Start Camera after delay
			CameraStartDelay--;								// (Waiting for completion of camera bootup)
			if (CameraStartDelay == 0) {
				printf("Start Camera\n");
				CommandBase::visionTarget->StartCamera("10.5.25.9");
				CommandBase::findTarget->StartCamera("10.5.25.9");
			}
		}
		
	}
	
	virtual void AutonomousInit() {
		if (autonomousCommand != NULL) autonomousCommand->Start();
	}
	
	virtual void AutonomousPeriodic() {
		Scheduler::GetInstance()->Run();
	}
	
	virtual void TeleopInit() {
		if (autonomousCommand != NULL) autonomousCommand->Cancel();
	}
	
	virtual void TeleopPeriodic() {
		Scheduler::GetInstance()->Run();
	}
	
	virtual void TestPeriodic() {
	}
};

START_ROBOT_CLASS(Vision2014);

