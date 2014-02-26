#include "../AllCommands.h"

TwoBallHotDecision::TwoBallHotDecision() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(drive);
}

// Called just before this Command runs the first time
void TwoBallHotDecision::Initialize() {
	if(findTarget->GetHotTarget()) {
		m_decision = dRight;
		drive->InitDistance(153, 0.8, true, 72, 23);
	} else {
		m_decision = dLeft;
		drive->InitDistance(132, 0.7, true, 0, 0);
	}
}

// Called repeatedly when this Command is scheduled to run
void TwoBallHotDecision::Execute() {
	drive->ExecuteDistance();
}

// Make this return true when this Command no longer needs to run execute()
bool TwoBallHotDecision::IsFinished() {
	return drive->OnTarget();
}

// Called once after isFinished returns true
void TwoBallHotDecision::End() {
	if(m_decision == dLeft) {
		ballShooter->Fire();
	}
	printf("Decision has been made\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TwoBallHotDecision::Interrupted() {
}
