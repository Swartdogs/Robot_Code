#ifndef DRIVE_H
#define DRIVE_H
#include "Commands/Subsystem.h"
#include "WPILib.h"

/**
 *
 *
 * @author Srinu
 */
class Drive: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	
	RobotDrive* drive;
	Jaguar* left;
	Jaguar* right;
public:
	Drive();
	void InitDefaultCommand();
	void DriveArcade(float move, float rotate);
};

#endif
