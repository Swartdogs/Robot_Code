#ifndef DRIVEROTATE_H
#define DRIVEROTATE_H

#include "../CommandBase.h"

// DriveRotate controller class.
// JT

class DriveRotate: public CommandBase {
public:
	DriveRotate(float target_angle, bool reset_gyro);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
	
	float GetTargetAngle(void);
	bool GetGyroReset(void);
	
private:
	float target_angle,reset_gyro;
};

#endif
