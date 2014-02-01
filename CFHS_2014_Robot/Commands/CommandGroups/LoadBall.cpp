#include "../AllCommands.h"

LoadBall::LoadBall() {	
	AddSequential(new BallShooterLoad());
	AddSequential(new PickupLoad());
}
