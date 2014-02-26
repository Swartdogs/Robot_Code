#include "../AllCommands.h"

OneBallAuto::OneBallAuto() {
	AddParallel(new FrontPickupSetMode(FrontPickup::fShoot));
	AddParallel(new FindHotTarget());
	AddSequential(new BallShooterWaitForMode(BallShooter::sReady));
	
	AddSequential(new CheckForHot());
	
	AddSequential(new DriveDistance(120, 0.7, true, 60, 0));
}
