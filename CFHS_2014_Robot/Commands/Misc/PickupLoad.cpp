#include "../AllCommands.h"

PickupLoad::PickupLoad() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(frontPickup);
	Requires(backPickup);
	
	m_pickup = (frontPickup->HasBall()) ? pFront : (ballShooter->HasBall()) ? pBack : pNone;
}

// Called just before this Command runs the first time
void PickupLoad::Initialize() {
	switch(m_pickup) {
		case pFront:
			frontPickup->SetPickupMode(FrontPickup::fLoad);
			backPickup->SetPickupMode(BackPickup::bStore);
			break;
		case pBack:
			backPickup->SetPickupMode(BackPickup::bStore);
			frontPickup->SetPickupMode(FrontPickup::fStore);
			break;
		default:;
	}
}

// Called repeatedly when this Command is scheduled to run
void PickupLoad::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool PickupLoad::IsFinished() {
	return (m_pickup == pFront) ? (frontPickup->GetFrontPickupMode() == FrontPickup::fStore) : 
			(m_pickup == pBack) ? (backPickup->GetBackPickupMode() == BackPickup::bStore) :
								  true;
}

// Called once after isFinished returns true
void PickupLoad::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void PickupLoad::Interrupted() {
	if(m_pickup == pFront) {
		frontPickup->SetPickupMode(FrontPickup::fStore);
	} else if(m_pickup == pBack) {
		backPickup->SetPickupMode(BackPickup::bStore);
	}
}
