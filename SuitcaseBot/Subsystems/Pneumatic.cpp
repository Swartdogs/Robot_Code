#include "Pneumatic.h"


Pneumatic::Pneumatic(RobotMap* robotMap) : Subsystem("Pneumatic") {
	m_robotLog = robotMap->robotLog;
	
	m_compressor = 	new Compressor(robotMap->diMap.pressureSwitch, robotMap->relayMap.airCompressor);
	m_rodIn = 		new Solenoid(robotMap->solenoidMap.rodIn);
	m_rodOut = 		new Solenoid(robotMap->solenoidMap.rodOut);
}

Pneumatic::~Pneumatic() {
	delete m_compressor;
	delete m_rodIn;
	delete m_rodOut;
}

void Pneumatic::InitDefaultCommand() {
}

void Pneumatic::SetRodIn(bool on) {
	m_rodIn->Set(on);
}

void Pneumatic::SetRodOut(bool on) {
	m_rodOut->Set(on);
}

void Pneumatic::StartCompressor() {
	m_compressor->Start();
}

void Pneumatic::StopCompressor() {
	m_compressor->Stop();
}

