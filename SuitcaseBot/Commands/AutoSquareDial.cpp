#include "AllCommands.h"

AutoSquareDial::AutoSquareDial(double wait, double distance) {
	AddParallel(new DialSetTarget(0));
	AddSequential(new WaitCommand(wait));
	AddSequential(new DriveDistance(distance, 0.5, true, 0, true));
	AddParallel(new DialSetTarget(270));
	AddSequential(new DriveRotate(90, false));
	AddSequential(new DriveDistance(distance, 0.5, true, 90, false));
	AddParallel(new DialSetTarget(180));
	AddSequential(new DriveRotate(180, false));
	AddSequential(new DriveDistance(distance, 0.5, true, 180, false));
	AddParallel(new DialSetTarget(90));
	AddSequential(new DriveRotate(270, false));
	AddSequential(new DriveDistance(distance, 0.5, true, 270, false));
}
