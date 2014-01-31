#include "../AllCommands.h"

PickupLoad::PickupLoad() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(frontPickup);
	Requires(backPickup);
	
	m_pickup = (frontPickup->HasBall()) ? pFront : (backPickup->HasBall()) ? pBack : pNone;
}

// Called just before this Command runs the first time
void PickupLoad::Initialize() {
	switch(m_pickup) {
		case pFront:
			frontPickup->SetPickupMode(FrontPickup::fMoveToLoad);
			if(backPickup->GetBackPickupMode() == BackPickup::bMoveToLoad || backPickup->GetBackPickupMode() == BackPickup::bLoad) {
				backPickup->SetPickupMode(BackPickup::bStore);
			}
			break;
		case pBack:
			backPickup->SetPickupMode(BackPickup::bMoveToLoad);
			if(frontPickup->GetFrontPickupMode() == FrontPickup::fMoveToLoad || frontPickup->GetFrontPickupMode() == FrontPickup::fLoad) {
				frontPickup->SetPickupMode(FrontPickup::fStore);
			}
			break;
		default:;
	}
}

// Called repeatedly when this Command is scheduled to run
void PickupLoad::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool PickupLoad::IsFinished() {
	if(m_pickup == pFront) {
		return (frontPickup->GetFrontPickupMode() == FrontPickup::fStore);
	} else if(m_pickup == pBack) {
		return (backPickup->GetBackPickupMode() == BackPickup::bStore);
	} else {
		return true;
	}
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
