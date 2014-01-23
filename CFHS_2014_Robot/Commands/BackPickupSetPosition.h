#ifndef BACKPICKUPSETPOSITION_H
#define BACKPICKUPSETPOSITION_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Neil
 */
class BackPickupSetPosition: public CommandBase {
public:
	BackPickupSetPosition(int position);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	int m_position;
};

#endif
