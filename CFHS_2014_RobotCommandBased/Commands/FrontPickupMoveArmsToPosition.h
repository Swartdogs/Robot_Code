#ifndef FRONTPICKUPMOVEARMSTOPOSITION_H
#define FRONTPICKUPMOVEARMSTOPOSITION_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Collin
 */
class FrontPickupMoveArmsToPosition: public CommandBase {
public:
	FrontPickupMoveArmsToPosition();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
