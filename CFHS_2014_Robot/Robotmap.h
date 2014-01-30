#ifndef ROBOTMAP_H
#define ROBOTMAP_H


/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */


#define	PWM_DRIVE_LEFT_FRONT 1
#define	PWM_DRIVE_LEFT_CENTER 2
#define	PWM_DRIVE_LEFT_REAR 3
#define	PWM_DRIVE_RIGHT_FRONT 4
#define	PWM_DRIVE_RIGHT_CENTER 5
#define	PWM_DRIVE_RIGHT_REAR 6

#define PWM_FRONT_PICKUP_LEFT_ARM 7
#define PWM_FRONT_PICKUP_RIGHT_ARM 8
#define PWM_FRONT_PICKUP_LEFT_ROLLERS 9
#define PWM_FRONT_PICKUP_RIGHT_ROLLERS 10

#define PWM_BACK_PICKUP_BASE_MOTOR 11
#define PWM_BACK_PICKUP_ROLLERS 12

#define PWM_SHOOT_MOTOR 13

#define	MOD_DRIVE_LEFT_FRONT 1
#define	MOD_DRIVE_LEFT_CENTER 1
#define	MOD_DRIVE_LEFT_REAR 1
#define	MOD_DRIVE_RIGHT_FRONT 1
#define MOD_DRIVE_RIGHT_CENTER 1
#define	MOD_DRIVE_RIGHT_REAR 1

#define MOD_FRONT_PICKUP_LEFT_ARM 1
#define MOD_FRONT_PICKUP_RIGHT_ARM 1
#define MOD_FRONT_PICKUP_LEFT_ROLLERS 1
#define MOD_FRONT_PICKUP_RIGHT_ROLLERS 1

#define MOD_BACK_PICKUP_BASE_MOTOR 2
#define MOD_BACK_PICKUP_ROLLERS 2

#define MOD_SHOOT_MOTOR 2

#define	DI_DRIVE_LEFT_A 1
#define	DI_DRIVE_LEFT_B 2
#define	DI_DRIVE_RIGHT_A 3
#define	DI_DRIVE_RIGHT_B 4
#define DI_BACK_PICKUP_SENSOR 5
#define DI_FRONT_PICKUP_BALL_SENSOR 6

#define AI_GYRO 1
#define AI_FRONT_PICKUP_LEFT_ARM_POT 2
#define AI_FRONT_PICKUP_RIGHT_ARM_POT 3
#define AI_BACK_PICKUP_BASE_POT 4
#define AI_SHOOT_POT 5

#endif
