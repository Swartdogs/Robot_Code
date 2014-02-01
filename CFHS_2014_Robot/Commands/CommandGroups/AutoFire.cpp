#include "../AllCommands.h"

AutoFire::AutoFire() {	
	AddSequential(new AutoFireDriveAndLoad());
	AddSequential(new BallShooterFire());
//	AddSequential(new BallShooterLoad());
}
