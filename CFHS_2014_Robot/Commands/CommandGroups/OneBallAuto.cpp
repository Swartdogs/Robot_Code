#include "../AllCommands.h"

OneBallAuto::OneBallAuto() {
//	AddParallel(new BallShooterLoad());
//	AddParallel(new DriveDistance(96.0, 0.7, true, 0,0, 0.0));
//	AddSequential(new FindHotTarget());
//	
//	AddSequential(new CheckForHot());
//	
//	AddSequential(new BallShooterFire());
	
	AddSequential(new BallShooterWaitForMode(BallShooter::sReady));
	AddSequential(new DriveDistance(120, 0.7, true, 60));
}