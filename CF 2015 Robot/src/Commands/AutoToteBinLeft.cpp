#include "AllCommands.h"

AutoToteBinLeft::AutoToteBinLeft(double wait) {
	AddSequential(new WaitCommand(wait));
	AddSequential(new ElevSetPosition(Elevator::pBinRelease));
	AddSequential(new ElevWaitForOnTarget());
	AddSequential(new DriveDistance(48, 0.6, true, false, 0, Drive::fNewMark));
	AddSequential(new DriveRotate(45, Drive::fMark, 0.5));
	AddSequential(new DriveDistance(12, 0.6, true, false, 45, Drive::fMark));
	AddSequential(new DriveRotate(90, Drive::fMark, 0.5));
	AddSequential(new DriveDistance(90, 0.5, true, true, 90, Drive::fMark));
	AddSequential(new DriveRotate(0, Drive::fMark, 0.5));
	AddSequential(new DriveDistance(-18, 0.5, true, true, 0, Drive::fMark));
}
