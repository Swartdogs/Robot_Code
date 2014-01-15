#ifndef FINDBUTTON_H
#define FINDBUTTON_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Neil
 */
class FindButton: public CommandBase {
public:
	FindButton();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
