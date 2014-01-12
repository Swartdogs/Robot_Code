#include "AllCommands.h"

DialSetTarget::DialSetTarget(float target) {
	Requires(dial);
	m_target = target;
}

void DialSetTarget::Initialize() {
	dial->SetTarget(m_target);
}

void DialSetTarget::Execute() {
}

bool DialSetTarget::IsFinished() {
	return true;
}

void DialSetTarget::End() {
}

void DialSetTarget::Interrupted() {
}
