#include "../AllCommands.h"

BallShooterFire::BallShooterFire() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
//	Requires(ballShooter);
//	Requires(frontPickup);
	Requires(drive);
	
	m_doLowShot = (frontPickup->GetFrontPickupMode() == FrontPickup::fLowDeploy);
	m_distanceToLowGoal = 7.0;
}

// Called just before this Command runs the first time
void BallShooterFire::Initialize() {
	if(m_doLowShot) {
		m_distanceToLowGoal = drive->GetRange() - m_distanceToLowGoal;
		drive->InitDistance(m_distanceToLowGoal, 0.5, true, 0);
	} else {
		ballShooter->Fire();
	}
}

// Called repeatedly when this Command is scheduled to run
void BallShooterFire::Execute() {
	if(m_doLowShot) {
		drive->ExecuteDistance();
	}
}

// Make this return true when this Command no longer needs to run execute()
bool BallShooterFire::IsFinished() {
	if(m_doLowShot) return (drive->OnTarget());
	else return true;
	
}

// Called once after isFinished returns true
void BallShooterFire::End() {
	if(m_doLowShot && (frontPickup->GetFrontPickupMode() == FrontPickup::fLowDeploy)) frontPickup->SetPickupMode(FrontPickup::fLowShoot);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BallShooterFire::Interrupted() {
}
