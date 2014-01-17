#ifndef FRONTPICKUPRUNLEFTWHEELS_H
#define FRONTPICKUPRUNLEFTWHEELS_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Collin
 */
class FrontPickupRunLeftWheels: public CommandBase {
public:
	FrontPickupRunLeftWheels(Relay::Value value);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	Relay::Value m_value;
};

#endif
