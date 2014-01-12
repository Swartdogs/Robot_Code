#ifndef PNEUMATIC_H
#define PNEUMATIC_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "../RobotLog.h"
#include "../RobotMap.h"

class Pneumatic: public Subsystem {
public:
	Pneumatic(RobotMap* robotMap);
	~Pneumatic();
	
	void InitDefaultCommand();
	
	void SetRodIn(bool on);
	void SetRodOut(bool on);
	void StartCompressor();
	void StopCompressor();

private:
	Compressor*		m_compressor;
	RobotLog*       m_robotLog;
	Solenoid*		m_rodIn;
	Solenoid*		m_rodOut;
};

#endif
