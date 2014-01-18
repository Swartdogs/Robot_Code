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

#define	DI_DRIVE_LEFT_A 1
#define	DI_DRIVE_LEFT_B 2
#define	DI_DRIVE_RIGHT_A 3
#define	DI_DRIVE_RIGHT_B 4

#define AI_GYRO 1

#endif
