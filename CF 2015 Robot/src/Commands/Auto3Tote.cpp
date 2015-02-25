#include "AllCommands.h"

Auto3Tote::Auto3Tote(double wait) {
	AddSequential(new WaitCommand(wait));
	AddSequential(new ElevSetPosition(Elevator::pStepTote));
	AddSequential(new DriveDistance(80, 0.7, true, true, 0, Drive::fNewMark));
	AddSequential(new ElevAutoCenter());
	AddSequential(new ElevSetPosition(Elevator::pLiftFromFloor));
	AddSequential(new ElevWaitForOnTarget());
	AddParallel  (new ElevSetPosition(Elevator::pStepTote));
	AddSequential(new DriveDistance(80, 0.7, true, true, 0, Drive::fMark));
	AddSequential(new DriveRotate(90, Drive::fMark, 0.7));
	AddSequential(new DriveDistanceMoveElev(122, 0.8, true, true, 90, Drive::fMark, 60, Elevator::pStep));
	AddSequential(new ElevWaitForOnTarget());
	AddSequential(new DriveDistance(-16, 0.7, true, true, 90, Drive::fMark));
}
