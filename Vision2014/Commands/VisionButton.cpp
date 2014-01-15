#include "VisionButton.h"

VisionButton::VisionButton() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(chassis);
	Requires(visionTarget);
}

// Called just before this Command runs the first time
void VisionButton::Initialize() {
	printf("\nVision Button\n");
	visionTarget->FindTargets();
}

// Called repeatedly when this Command is scheduled to run
void VisionButton::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool VisionButton::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void VisionButton::End() {
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void VisionButton::Interrupted() {
}
