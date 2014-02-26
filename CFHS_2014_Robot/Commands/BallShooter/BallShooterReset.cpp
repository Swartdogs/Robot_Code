#include "../AllCommands.h"

BallShooterReset::BallShooterReset() {
	Requires(ballShooter);
}

void BallShooterReset::Initialize() {
	ballShooter->Reset();
}

void BallShooterReset::Execute() {
}

bool BallShooterReset::IsFinished() {
	return true;
}

void BallShooterReset::End() {
}

void BallShooterReset::Interrupted() {
}
