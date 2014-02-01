#include "../AllCommands.h"

TwoBallAuto::TwoBallAuto() {
	AddParallel(new FrontPickupSetMode(FrontPickup::fAutoDeploy));
	AddParallel(new BallShooterLoad());
	AddSequential(new DriveDistance(96.0, 0.7, true, 0));
	
	AddSequential(new BallShooterFire());
	
	AddSequential(new PickupLoad());
	
	AddSequential(new BallShooterFire());
}
