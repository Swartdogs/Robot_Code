#include "Drive.h"
#include "../Robotmap.h"
#include "../Commands/DriveWithJoystick.h"

Drive::Drive() : Subsystem("Drive") {
	left = new Jaguar(1, 1);
	right = new Jaguar(1, 2);
	drive = new RobotDrive(left, right);
}
    
void Drive::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
	SetDefaultCommand(new DriveWithJoystick());
}

void Drive::DriveArcade(float move, float rotate) {
	drive->ArcadeDrive(move, rotate);
}


// Put methods for controlling this subsystem
// here. Call these from Commands.
