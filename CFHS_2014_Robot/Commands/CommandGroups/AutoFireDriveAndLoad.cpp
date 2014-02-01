#include "../AllCommands.h"

AutoFireDriveAndLoad::AutoFireDriveAndLoad() {	
	AddParallel(new DriveDistance(50.0, 0.7, true, 0.0));
	AddSequential(new LoadBall());
}
