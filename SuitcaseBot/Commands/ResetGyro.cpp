#include "AllCommands.h"

ResetGyro::ResetGyro() {
	Requires(drive);
}

void ResetGyro::Initialize() {
	drive->ResetGyro();
}

void ResetGyro::Execute() {
}

bool ResetGyro::IsFinished() {
	return true;
}

void ResetGyro::End() {
}

void ResetGyro::Interrupted() {
}
