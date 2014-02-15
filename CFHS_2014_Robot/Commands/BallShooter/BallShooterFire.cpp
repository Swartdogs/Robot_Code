#include "../AllCommands.h"

BallShooterFire::BallShooterFire() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(ballShooter);
}

// Called just before this Command runs the first time
void BallShooterFire::Initialize() {
	SetTimeout(1.0);
}

// Called repeatedly when this Command is scheduled to run
void BallShooterFire::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BallShooterFire::IsFinished() {
	return (IsTimedOut() || ballShooter->HasBall());
	
}

// Called once after isFinished returns true
void BallShooterFire::End() {
	ballShooter->Fire();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BallShooterFire::Interrupted() {
}
