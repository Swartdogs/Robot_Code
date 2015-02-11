#include "AllCommands.h"

DriveJoystick::DriveJoystick() {
	Requires(MyRobot::drive);
}

void DriveJoystick::Initialize() {
}

void DriveJoystick::Execute() {
	MyRobot::drive->DriveWithJoystick(MyRobot::oi->GetDrive(),
									  0,
									  MyRobot::oi->GetRotate(),
									  Drive::wAll);

//	MyRobot::drive->DriveWithJoystick(MyRobot::oi->GetDriveY(),
//									  MyRobot::oi->GetDriveX(),
//									  MyRobot::oi->GetDriveZ(),
//									  Drive::wAll);

//	if(MyRobot::drive->GetDriveMode() == Drive::mMecanum){
//		MyRobot::drive->DriveWithJoystick(MyRobot::oi->GetDriveY(),
//										  MyRobot::oi->GetDriveX(),
//										  MyRobot::oi->GetDriveZ(),
//										  Drive::wAll);
//	} else {
//		MyRobot::drive->DriveWithJoystick(MyRobot::oi->GetDriveY(),
//										  MyRobot::oi->GetDriveX());
//	}
}

bool DriveJoystick::IsFinished() {
	return false;
}

void DriveJoystick::End() {
}

void DriveJoystick::Interrupted() {
}
