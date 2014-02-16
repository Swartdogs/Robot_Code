#ifndef DRIVERESETGYRO_H
#define DRIVERESETGYRO_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Neil
 */
class DriveResetGyro: public CommandBase {
public:
	DriveResetGyro();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
