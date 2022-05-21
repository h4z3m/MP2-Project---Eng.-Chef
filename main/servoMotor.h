/******************************************************************************
 *
 * Module: Servo Motor
 *
 * File Name: servoMotor.h
 *
 * Description: Header file for custom servo motor class using basic Arduino 
 *		servo motor library
 *
 * Date Created: 1/4/2022
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#pragma once
#include "Motor.h"
#include <Servo.h>
#include "std_types.h"
/******************************************************************************
 *
 * Structure Name: Servo_configType
 *
 * Structure Description: Configuration struct which contains multiple servo
 * parameters:
 *	1. pin: Output pin for the PWM signal
 *  2. initialAngle: The initial angle of the servo motor which it returns
 *	   to after calling returnToInitialPos
 *  3. currentAngle: The angle of the motor at any given time
 *  4. minAngle: Minimum allowable angle for the servo motor
 *  5. maxAngle: Maximum allowable angle for the servo motor
 *  6. maxSpeed: The maximum speed for the motor (RPM)
 *
 *******************************************************************************/


class servoMotor: public Motor {
private:
	Servo servo;
public:

	/******************************************************************************
	 * Function Name:init
	 * Description: Initializes the motor by setting its signal pin, minimum & maximum
	 *		pulse times, and moves it to a given initial position
	 * Returns: void
	 *******************************************************************************/
	void init(struct Motor_configType* conf);

	/******************************************************************************
	 * Function Name: write
	 * Description: Checks if the given angle is valid between minAngle and maxAngle
	 *		and moves the servo motor to that position
	 * Args: -float32 angle: Given angle to check
	 * Returns: void
	 *******************************************************************************/
	void write(float32 angle);


	/******************************************************************************
	 * Function Name: returnToInitial
	 * Description: Returns the motor to its initial position
	 * Returns: void
	 *******************************************************************************/
	void returnToInitial();
};
