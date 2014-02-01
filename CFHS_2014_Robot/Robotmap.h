#ifndef ROBOTMAP_H
#define ROBOTMAP_H


/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */


#define	PWM_DRIVE_LEFT_FRONT 1
#define	PWM_DRIVE_LEFT_CENTER_REAR 2
#define	PWM_DRIVE_RIGHT_FRONT 3
#define	PWM_DRIVE_RIGHT_CENTER_REAR 4

#define PWM_FRONT_PICKUP_LEFT_ARM 5
#define PWM_FRONT_PICKUP_RIGHT_ARM 6

#define PWM_BACK_PICKUP_BASE_MOTOR 7
#define PWM_BACK_PICKUP_ROLLERS 8

#define PWM_SHOOT_MOTOR 9

#define RELAY_FRONT_PICKUP_LEFT_ROLLERS 1
#define RELAY_FRONT_PICKUP_RIGHT_ROLLERS 2

#define	MOD_DRIVE_LEFT_FRONT 1
#define	MOD_DRIVE_LEFT_CENTER_REAR 1
#define	MOD_DRIVE_RIGHT_FRONT 1
#define MOD_DRIVE_RIGHT_CENTER_REAR 1

#define MOD_FRONT_PICKUP_LEFT_ARM 1
#define MOD_FRONT_PICKUP_RIGHT_ARM 1
#define MOD_FRONT_PICKUP_LEFT_ROLLERS 1
#define MOD_FRONT_PICKUP_RIGHT_ROLLERS 1

#define MOD_BACK_PICKUP_BASE_MOTOR 1
#define MOD_BACK_PICKUP_ROLLERS 1

#define MOD_SHOOT_MOTOR 1

#define	DI_DRIVE_LEFT_A 1
#define	DI_DRIVE_LEFT_B 2
#define	DI_DRIVE_RIGHT_A 3
#define	DI_DRIVE_RIGHT_B 4
#define DI_BACK_PICKUP_SENSOR 5
#define DI_FRONT_PICKUP_BALL_SENSOR 6
#define DI_DRIVE_TAPE_SENSOR 7

#define AI_GYRO 1
#define AI_FRONT_PICKUP_LEFT_ARM_POT 2
#define AI_FRONT_PICKUP_RIGHT_ARM_POT 3
#define AI_BACK_PICKUP_BASE_POT 4
#define AI_SHOOT_POT 5
#define AI_RANGEFINDER 6

#endif
