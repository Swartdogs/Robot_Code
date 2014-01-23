#ifndef FINDHOTTARGET_H
#define FINDHOTTARGET_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Srinu
 */
class FindHotTarget: public CommandBase {
public:
	FindHotTarget();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
