#ifndef DRIVEJOYSTICK_H
#define DRIVEJOYSTICK_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Neil
 */
class DriveJoystick: public CommandBase {
public:
	DriveJoystick();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
