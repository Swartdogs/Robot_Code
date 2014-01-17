#ifndef FRONTPICKUPRUNRIGHTWHEELS_H
#define FRONTPICKUPRUNRIGHTWHEELS_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Collin
 */
class FrontPickupRunRightWheels: public CommandBase {
public:
	FrontPickupRunRightWheels(Relay::Value value);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	Relay::Value m_value;
};

#endif
