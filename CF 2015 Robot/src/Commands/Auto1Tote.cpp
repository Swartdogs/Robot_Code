#include "AllCommands.h"

Auto1Tote::Auto1Tote(double wait) {
	AddSequential(new WaitCommand(wait));
	AddSequential(new ElevSetPosition(Elevator::pStep));
	AddSequential(new ElevWaitForOnTarget());
	AddSequential(new DriveRotate(90, Drive::fNewMark, 0.7));
	AddSequential(new DriveDistance(100, 0.4, true, true, 90, Drive::fMark));
}

