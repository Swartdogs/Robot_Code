#include "AllCommands.h"

AutoBinPosition::AutoBinPosition(double wait) {
	AddSequential(new WaitCommand(wait));
	AddSequential(new DriveDistance(44, 0.6, true, true, 0, Drive::fNewMark));
	AddSequential(new DriveAutoStrafe(1.4, -0.5, 0, Drive::fMark));
	AddSequential(new WaitCommand(1.25));
	AddSequential(new DriveDistance(-22, 0.6, true, false, 0, Drive::fMark));
	AddSequential(new ElevSetPosition(Elevator::pBinLoad));
}
