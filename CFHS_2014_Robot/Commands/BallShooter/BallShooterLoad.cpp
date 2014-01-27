#include "../AllCommands.h"

BallShooterLoad::BallShooterLoad() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(ballShooter);
}

// Called just before this Command runs the first time
void BallShooterLoad::Initialize() {
	ballShooter->Load();
}

// Called repeatedly when this Command is scheduled to run
void BallShooterLoad::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BallShooterLoad::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void BallShooterLoad::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BallShooterLoad::Interrupted() {
}
