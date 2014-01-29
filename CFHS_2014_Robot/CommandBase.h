#ifndef COMMAND_BASE_H
#define COMMAND_BASE_H

#include "Commands/Command.h"
#include "Subsystems/Drive.h"
#include "Subsystems/FindTarget.h"
#include "Subsystems/FrontPickup.h"
#include "Subsystems/BackPickup.h"
#include "Subsystems/BallShooter.h"
#include "OI.h"
#include "RobotLog.h"

/**
 * The base for all commands. All atomic commands should subclass CommandBase.
 * CommandBase stores creates and stores each control system. To access a
 * subsystem elsewhere in your code in your code use CommandBase.examplesubsystem
 */
class CommandBase: public Command {
public:
	CommandBase(const char *name);
	CommandBase();

	static void Init(RobotLog* logDelegate);
	static void Periodic();

	// Create a single static instance of all of your subsystems
	static Drive* drive;
	static FindTarget* findTarget;
	static OI* oi;
	static FrontPickup* frontPickup;
	static BackPickup* backPickup;
	static BallShooter* ballShooter;
};

#endif
