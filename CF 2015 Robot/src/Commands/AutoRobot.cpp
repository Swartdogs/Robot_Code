#include "AllCommands.h"

AutoRobot::AutoRobot(double wait) {
	AddSequential(new WaitCommand(wait));
	AddSequential(new DriveDistance(-80, 0.4, true, true, 0, Drive::fNewMark));
}
