#include "AllCommands.h"

AutoSquare::AutoSquare(double wait, double distance) {
	AddSequential(new WaitCommand(wait));
	AddSequential(new DriveDistance(distance, 0.5, true, 0, true ));
	AddSequential(new DriveRotate(90, false));
	AddSequential(new DriveDistance(distance, 0.5, true, 90, false));
	AddSequential(new DriveRotate(180, false));
	AddSequential(new DriveDistance(distance, 0.5, true, 180, false));
	AddSequential(new DriveRotate(270, false));
	AddSequential(new DriveDistance(distance, 0.5, true, 270, false));
	AddSequential(new DriveRotate(360, false));
}

