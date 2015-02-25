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
	robotLog->Write("Schumacher: Robot Init (Build 2015)");

	drive = 		new Drive();
	elevator = 		new Elevator();

	dashboard = 	new TcpHost(20, 14, 1);
	powerPanel = 	new PdpData(63);									// Outputs 0-5 are in use
	oi = 			new OI();

	m_autoDelay = m_autoSelect = 0;
	m_autoCommand = NULL;

	IniParser();

	powerPanel->SetEnabled(true);

	m_compressor = new Compressor();
	m_compressor->ClearAllPCMStickyFaults();
//	m_compressor->Stop();

	elevator->LogPotInUse();
}
	
void MyRobot::DisabledInit() {
	robotLog->SetMode(RobotLog::mDisabled);
	robotLog->Write("");
	robotLog->Write("Schumacher: Disabled Init");

	if (m_autoCommand != NULL) m_autoCommand->Cancel();

	sprintf(m_log, "Schumacher: Total Energy=%6.1f joules (watt seconds)", powerPanel->GetTotalEnergy());
	robotLog->Write(m_log);

	dashboard->SetRobotMode(1);
	powerPanel->SetLogEnabled(false);
	robotLog->Write("Schumacher: Disabled Periodic");
	robotLog->Close();
	SetDashRunData();
}

void MyRobot::DisabledPeriodic() {
	if (dashboard->GetDashButtonPress(0, DB_READ_INI)) IniParser();
	if (dashboard->GetDashButtonPress(0, DB_RESET_PEAKS)) powerPanel->ResetPeaks();
//	if (dashboard->GetDashButtonPress(0, DB_RESET_PEAKS)) drive->ResetEncoders();

	SetDashSensorData();
	CheckCameraButton();
}

void MyRobot::AutonomousInit() {
	robotLog->SetMode(RobotLog::mAutonomous);
	robotLog->Write("");
	robotLog->Write("Schumacher: Autonomous Init");

	dashboard->SetRobotMode(2);
	powerPanel->SetLogEnabled(dashboard->GetDashButton(0, DB_PDP_LOG));
	powerPanel->ResetEnergy();

	m_autoSelect = dashboard->GetDashValue(DV_AUTO_SELECT);
	m_autoDelay = dashboard->GetDashValue(DV_AUTO_DELAY);

	switch(m_autoSelect) {
		case 1:	m_autoCommand = new AutoRobot((double)m_autoDelay / 4.0);
				break;
		case 2: m_autoCommand = new Auto1Tote((double)m_autoDelay / 4.0);
				break;
		case 3: m_autoCommand = new AutoToteBinLeft((double)m_autoDelay / 4.0);
				break;
		case 4: m_autoCommand = new AutoToteBinRight((double)m_autoDelay / 4.0);
				break;
		case 5: m_autoCommand = new Auto3Tote((double) m_autoDelay / 4.0);
				break;
		case 6: m_autoCommand = new AutoBinPosition((double) m_autoDelay / 4.0);
				break;
		case 7: m_autoCommand = new AutoBinRobot((double) m_autoDelay / 4.0);
				break;
		case 8: m_autoCommand = new AutoBinToFeeder((double) m_autoDelay / 4.0);
				break;
		default: m_autoCommand = NULL;
	}

	if (m_autoCommand != NULL) m_autoCommand->Start();

	sprintf(m_log, "Schumacher: Autonomous Periodic  Command=%d  Delay=%d", m_autoSelect, m_autoDelay * 250);
	robotLog->Write(m_log);
}

void MyRobot::AutonomousPeriodic() {
	robotLog->StartPeriodic();

	Scheduler::GetInstance()->Run();
	elevator->RunPID(false);

	SetDashRunData();
	SetDashSensorData();
	CheckCameraButton();

	robotLog->EndPeriodic();
}

void MyRobot::TeleopInit() {
	robotLog->SetMode(RobotLog::mTeleop);
	robotLog->Write("");
	robotLog->Write("Schumacher: Teleop Init");

	if (m_autoCommand != NULL) m_autoCommand->Cancel();

	dashboard->SetRobotMode(3);
	powerPanel->SetLogEnabled(dashboard->GetDashButton(0, DB_PDP_LOG));
	powerPanel->ResetEnergy();

	robotLog->Write("Schumacher: Teleop Periodic");
}

void MyRobot::TeleopPeriodic() {
	robotLog->StartPeriodic();

	Scheduler::GetInstance()->Run();
	elevator->RunPID(false);

	SetDashRunData();
	SetDashSensorData();
	CheckCameraButton();

	robotLog->EndPeriodic();
}

void MyRobot::TestInit() {
	robotLog->SetMode(RobotLog::mTest);
	robotLog->Write("");
	robotLog->Write("Schumacher: Test Init");

	if (m_autoCommand != NULL) m_autoCommand->Cancel();

	dashboard->SetRobotMode(4);

	robotLog->Write("Schumacher: Test Periodic");
}

void MyRobot::TestPeriodic() {
	static int tunePID = 0;
	SetDashSensorData();

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
			case 3: elevator->RunPID(true);
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

void MyRobot::CheckCameraButton() {
	static bool lastCheck = false;
	bool 		isPressed = oi->GetCameraButton();

	if (lastCheck != isPressed) {
		lastCheck = isPressed;
		int32_t value = isPressed ? 1 : 0;
		dashboard->SetRobotValue(RV_CAMERA_BUTTON, value);
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

	robotLog->Write("");


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
	dashboard->SetRobotValue(RV_DRIVE_LF_PEAK, (int32_t)(powerPanel->GetPeakCurrent(0) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_LR_AMPS, (int32_t)(powerPanel->GetCurrent(1) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_LR_PEAK, (int32_t)(powerPanel->GetPeakCurrent(1) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_RF_AMPS, (int32_t)(powerPanel->GetCurrent(2) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_RF_PEAK, (int32_t)(powerPanel->GetPeakCurrent(2) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_RR_AMPS, (int32_t)(powerPanel->GetCurrent(3)* 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_RR_PEAK, (int32_t)(powerPanel->GetPeakCurrent(3) * 10 + 0.5));
	dashboard->SetRobotValue(RV_ELEV1_AMPS, (int32_t)(powerPanel->GetCurrent(4) * 10 + 0.5));
	dashboard->SetRobotValue(RV_ELEV1_PEAK, (int32_t)(powerPanel->GetPeakCurrent(4) * 10 + 0.5));
	dashboard->SetRobotValue(RV_ELEV2_AMPS, (int32_t)(powerPanel->GetCurrent(5) * 10 + 0.5));
	dashboard->SetRobotValue(RV_ELEV2_PEAK, (int32_t)(powerPanel->GetPeakCurrent(5) * 10 + 0.5));
}

void MyRobot::SetDashSensorData() {
	dashboard->SetRobotValue(RV_DRIVE_ENCODER_L, (int32_t)(drive->GetEncoderDistance(Drive::eLeft) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_ENCODER_R, (int32_t)(drive->GetEncoderDistance(Drive::eRight) * 10 + 0.5));
	dashboard->SetRobotValue(RV_DRIVE_GYRO, (int32_t)(drive->GetGyroAngle() * 10 + 0.5));
	dashboard->SetRobotValue(RV_ELEV_POSITION_L, elevator->GetPosition(Elevator::potLeft));
	dashboard->SetRobotValue(RV_ELEV_POSITION_R, elevator->GetPosition(Elevator::potRight));
}

START_ROBOT_CLASS(MyRobot);
