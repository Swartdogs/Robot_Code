#ifndef ROBOTMAP_H
#define ROBOTMAP_H

#include "WPILib.h"

#define MY_ROBOT 1					// 0=Otis;  1=Schumacher

// RoboRIO Analog Inputs

#define AI_GYRO 0
#define AI_ELEVATOR_POT 1

// RoboRIO Digital Inputs

#define DI_ENCODER_LEFT_A 0
#define DI_ENCODER_LEFT_B 1
#define DI_ENCODER_RIGHT_A 2
#define DI_ENCODER_RIGHT_B 3

//RoboRIO Solenoids

#define S_ELEVATOR_BRAKE 0

// RoboRIO PWM Outputs

#define PWM_DRIVE_LF 0
#define PWM_DRIVE_LR 1
#define PWM_DRIVE_RF 2
#define PWM_DRIVE_RR 3
#define PWM_ELEVATOR_1 4
#define PWM_ELEVATOR_2 5

// Dashboard Value (From Dashboard)

#define DV_AUTO_SELECT 0
#define DV_AUTO_DELAY 1
#define DV_PID_SELECT 2
#define DV_PID_SETPOINT 3
#define DV_PID_MAX_PWM 4
#define DV_PID_P_THRESHOLD 5
#define DV_PID_P_ABOVE 6
#define DV_PID_P_BELOW 7
#define DV_PID_I_THRESHOLD 8
#define DV_PID_I_ABOVE 9
#define DV_PID_I_BELOW 10
#define DV_PID_D_THRESHOLD 11
#define DV_PID_D_ABOVE 12
#define DV_PID_D_BELOW 13

// Dashboard Buttons (From Dashboard)

#define DB_RESET_ROBOT 0
#define DB_READ_INI 1
#define DB_RESET_PEAKS 2
#define DB_PDP_LOG 3
#define DB_TUNE_PID 4

// Robot Values (To Dashboard)

#define RV_VOLTAGE 0
#define RV_LOW_VOLTAGE 1
#define RV_DRIVE_LF_AMPS 2
#define RV_DRIVE_LF_PEAK 3
#define RV_DRIVE_LR_AMPS 4
#define RV_DRIVE_LR_PEAK 5
#define RV_DRIVE_RF_AMPS 6
#define RV_DRIVE_RF_PEAK 7
#define RV_DRIVE_RR_AMPS 8
#define RV_DRIVE_RR_PEAK 9
#define RV_ELEV1_AMPS 10
#define RV_ELEV1_PEAK 11
#define RV_ELEV2_AMPS 12
#define RV_ELEV2_PEAK 13
#define RV_DRIVE_ENCODER_L 14
#define RV_DRIVE_ENCODER_R 15
#define RV_DRIVE_GYRO 16
#define RV_ELEV_POSITION 17

#endif
