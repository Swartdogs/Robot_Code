#include "../AllCommands.h"

BallShooterWaitForMode::BallShooterWaitForMode(BallShooter::ShootState mode) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(ballShooter);
	
	m_mode = mode;
}

// Called just before this Command runs the first time
void BallShooterWaitForMode::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void BallShooterWaitForMode::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BallShooterWaitForMode::IsFinished() {
	return ballShooter->GetShootState() == m_mode;
}

// Called once after isFinished returns true
void BallShooterWaitForMode::End() {
	printf("Wait For Shooter End\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BallShooterWaitForMode::Interrupted() {
}
