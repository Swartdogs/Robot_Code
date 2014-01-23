#ifndef BACKPICKUPPERIODIC_H
#define BACKPICKUPPERIODIC_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Neil
 */
class BackPickupPeriodic: public CommandBase {
public:
	BackPickupPeriodic();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
