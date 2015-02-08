#include "MyRobot.h"

RobotLog* 	MyRobot::robotLog = NULL;
Drive*		MyRobot::drive = NULL;
Elevator*	MyRobot::elevator = NULL;
TcpHost*	MyRobot::dashboard = NULL;
PdpData*	MyRobot::powerPanel = NULL;
OI*			MyRobot::oi = NULL;

void MyRobot::RobotInit() {
	robotLog = 	new RobotLog();
	robotLog->Write("");
	robotLog->Write("Otis: Robot Init (Build 2015)");

	drive = 		new Drive();
	elevator = 		new Elevator();

	dashboard = 	new TcpHost(18, 14, 1);
	powerPanel = 	new PdpData();
	oi = 			new OI();

	m_autoDelay = m_autoSelect = 0;
	m_autoCommand = NULL;

	IniParser();

	powerPanel->SetEnabled(true);

	m_compressor = new Compressor();
	m_compressor->ClearAllPCMStickyFaults();
//	m_compressor->Stop();

	m_powerPanel = new PowerDistributionPanel();
	m_powerPanel->ClearStickyFaults();
}
	
void MyRobot::DisabledInit() {
	robotLog->SetMode(RobotLog::mDisabled);
	robotLog->Write("");
	robotLog->Write("Otis: Disabled Init");

	if (m_autoCommand != NULL) m_autoCommand->Cancel();

	dashboard->SetRobotMode(1);
	powerPanel->SetLogEnabled(false);
	robotLog->Write("Otis: Disabled Periodic");
	robotLog->Close();
}

void MyRobot::DisabledPeriodic() {
	if (dashboard->GetDashButtonPress(0, DB_READ_INI)) IniParser();
	if (dashboard->GetDashButtonPress(0, DB_RESET_PEAKS)) powerPanel->ResetPeaks();

	SetDashSensorData();
}

void MyRobot::AutonomousInit() {
	robotLog->SetMode(RobotLog::mAutonomous);
	robotLog->Write("");
	robotLog->Write("Otis: Autonomous Init");

	dashboard->SetRobotMode(2);
	powerPanel->SetLogEnabled(dashboard->GetDashButton(0, DB_PDP_LOG));

	m_autoSelect = dashboard->GetDashValue(DV_AUTO_SELECT);
	m_autoDelay = dashboard->GetDashValue(DV_AUTO_DELAY);

	switch(m_autoSelect) {
		default: m_autoCommand = NULL;
	}

	if (m_autoCommand != NULL) m_autoCommand->Start();

	sprintf(m_log, "Otis: Autonomous Periodic  Command=%d  Delay=%d", m_autoSelect, m_autoDelay * 250);
	robotLog->Write(m_log);
}

void MyRobot::AutonomousPeriodic() {
	robotLog->StartPeriodic();

	Scheduler::GetInstance()->Run();
	SetDashRunData();
	SetDashSensorData();

	robotLog->EndPeriodic();
}

void MyRobot::TeleopInit() {
	robotLog->SetMode(RobotLog::mTeleop);
	robotLog->Write("");
	robotLog->Write("Otis: Teleop Init");

	if (m_autoCommand != NULL) m_autoCommand->Cancel();

	dashboard->SetRobotMode(3);
	powerPanel->SetLogEnabled(dashboard->GetDashButton(0, DB_PDP_LOG));

	robotLog->Write("Otis: Teleop Periodic");
}

void MyRobot::TeleopPeriodic() {
	robotLog->StartPeriodic();

	Scheduler::GetInstance()->Run();
	SetDashRunData();
	SetDashSensorData();

	robotLog->EndPeriodic();
}

void MyRobot::TestInit() {
	robotLog->SetMode(RobotLog::mTest);
	robotLog->Write("");
	robotLog->Write("Otis: Test Init");

	if (m_autoCommand != NULL) m_autoCommand->Cancel();

	dashboard->SetRobotMode(4);

	robotLog->Write("Otis: Test Periodic");
}

void MyRobot::TestPeriodic() {
	static int tunePID = 0;
	SetDashSensorData();

//	if (dashboard->GetDashButton(0, DB_TUNE_PID)) {
//		elevator->SetBrake(Elevator::bOn);
//	} else {
//		elevator->SetBrake(Elevator::bOff);
//	}
//
//	return;

	if (dashboard->GetDashButton(0, DB_TUNE_PID)) {
		if (tunePID == 0) {
			tunePID = dashboard->GetDashValue(DV_PID_SELECT);

			switch(tunePID) {
				case 1:	drive->TuneDrivePID();
						break;
				case 2: drive->TuneRotatePID();
						break;
				case 3: elevator->TuneElevPID();
						break;
				default:;
			}
		}

		switch(tunePID) {
			case 1: drive->ExecuteDistance(true);
					break;
			case 2: drive->ExecuteRotate(true);
					break;
			case 3: elevator->RunWithPID(true);
					break;
			default:;
		}

	} else {
		if (tunePID != 0) {
			switch(tunePID) {
				case 1: drive->SetDrivePID(0);
						break;
				case 2: drive->SetRotatePID();
						break;
				case 3: elevator->SetElevPID(Elevator::dUp);
						break;
				default:;
			}

			tunePID = 0;
		}

		drive->DriveWithJoystick(0, 0, 0, Drive::wAll);
		elevator->RunWithJoystick(0);
	}
}

void MyRobot::IniParser() {
	char		temp[100];
	int			subsystemIndex;
	std::string	subsystemName[2];
	int			equalSign;
	std::string	key;
	int32_t		keyValue;

	subsystemName[0] = "[ELEVATOR]";
	subsystemName[1] = "[DRIVE]";

	FILE* iniFile = fopen("/home/lvuser/525Init.ini", "r");

	if (iniFile == NULL) {
		robotLog->Write("IniParser: File not found");
		fclose(iniFile);
		return;
	}

	while(!feof(iniFile)) {
		fgets(temp, 100, iniFile);
		std::string line(temp);
		line.erase(line.length() - 2, 2);

		if(line[0] == '[') {
			subsystemIndex = -1;

			for(int i = 0; i < 1; i++) {
				if(line == subsystemName[i]) {
					subsystemIndex = i;
					break;
				}
			}
		} else if(line.length() > 0 && line[0] != ' ' && line[0] != '!'){
			equalSign = line.find('=');
			if(equalSign != 0) {
				key = line.substr(0, equalSign);
				keyValue = atoi(line.substr(equalSign + 1).c_str());

				switch(subsystemIndex) {
				case 0:
					elevator->SetConstant(key, keyValue);
					break;

				case 1:
					drive->SetConstant(key, keyValue);
					break;

				default:;
				}
			}
		}
	}
	fclose(iniFile);
}

void MyRobot::SetDashRunData() {
	dashboard->SetRobotValue(RV_VOLTAGE, (int32_t)(powerPanel->GetVoltage() * 10 + 0.5));
	dashboard->SetRobotValue(RV_LOW_VOLTAGE, (int32_t)(powerPanel->GetLowVoltage() * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_LF_AMPS, (int32_t)(powerPanel->GetCurrent(0) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_LF_PEAK, (int32_t)(powerPanel->GetPeak(0) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_LR_AMPS, (int32_t)(powerPanel->GetCurrent(1) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_LR_PEAK, (int32_t)(powerPanel->GetPeak(1) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_RF_AMPS, (int32_t)(powerPanel->GetCurrent(2) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_RF_PEAK, (int32_t)(powerPanel->GetPeak(2) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_RR_AMPS, (int32_t)(powerPanel->GetCurrent(3)* 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_RR_PEAK, (int32_t)(powerPanel->GetPeak(3) * 10 + 0.5));
	dashboard->SetRobotValue(RV_ELEV1_AMPS, (int32_t)(powerPanel->GetCurrent(4) * 10 + 0.5));
	dashboard->SetRobotValue(RV_ELEV1_PEAK, (int32_t)(powerPanel->GetPeak(4) * 10 + 0.5));
	dashboard->SetRobotValue(RV_ELEV2_AMPS, (int32_t)(powerPanel->GetCurrent(5) * 10 + 0.5));
	dashboard->SetRobotValue(RV_ELEV2_PEAK, (int32_t)(powerPanel->GetPeak(5) * 10 + 0.5));
}

void MyRobot::SetDashSensorData() {
	dashboard->SetRobotValue(RV_DRIVE_ENCODER_L, (int32_t)(drive->GetEncoderDistance(Drive::eLeft) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_ENCODER_R, (int32_t)(drive->GetEncoderDistance(Drive::eRight) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_GYRO, (int32_t)(drive->GetGyroAngle() * 10 + 0.5));
	dashboard->SetRobotValue(RV_ELEV_POSITION, elevator->GetPosition());
}

START_ROBOT_CLASS(MyRobot);
