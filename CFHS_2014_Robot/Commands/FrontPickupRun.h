#ifndef FRONTPICKUPRUN_H
#define FRONTPICKUPRUN_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Collin
 */
class FrontPickupRun: public CommandBase {
public:
	FrontPickupRun();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
