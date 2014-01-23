#ifndef DRIVEDISTANCE_H
#define DRIVEDISTANCE_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Srinu
 */
class DriveDistance: public CommandBase {
private:
	double m_targetDistance;
	float m_maxSpeed;
	float m_targetAngle;
	bool m_resetEncoders;
	bool m_resetGyro;
	bool m_useGyro;
public:
	DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders);
	DriveDistance(double targetDistance, float maxSpeed, bool resetEncoders, float targetAngle, bool resetGyro);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
