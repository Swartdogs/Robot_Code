#ifndef VISION_BUTTON_H
#define VISION_BUTTON_H

#include "../CommandBase.h"
 
class VisionButton: public CommandBase {
public:
	VisionButton();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
