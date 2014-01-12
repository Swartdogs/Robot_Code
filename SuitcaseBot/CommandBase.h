#ifndef COMMAND_BASE_H
#define COMMAND_BASE_H

#include "Commands/Command.h"
#include "Subsystems/Dial.h"
#include "Subsystems/Drive.h"
#include "Subsystems/Pneumatic.h"
#include "OI.h"
#include "RobotLog.h"
#include "RobotMap.h"

class CommandBase: public Command {
public:
	CommandBase(const char *name);
	CommandBase();
	
	static void Init(RobotLog* logDelegate);

	static RobotMap*	robotMap;
	static Dial*		dial;
	static Drive* 		drive;
	static OI*			oi;
	static Pneumatic*	pneumatic;
};

#endif
