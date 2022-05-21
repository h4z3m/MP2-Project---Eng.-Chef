/******************************************************************************
 *
 * Module: Servo Motor
 *
 * File Name: servoMotor.cpp
 *
 * Description: Source file for custom servo motor class using basic Arduino
 *		servo motor library
 *
 * Date Created: 1/4/2022
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#include "servoMotor.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
 /********************************** Definitions **********************************/

/*Servo motor pulse times, must be set accordingly for the movement to be accurate*/
#define SERVO_MIN_PULSE_TIME (1000)
#define SERVO_MAX_PULSE_TIME (2000)

/********************************** Public Functions **********************************/

void servoMotor::init(struct Motor_configType* conf) {
	motorConfig = conf;
	pinMode(motorConfig->pin, OUTPUT);
	servo.attach(motorConfig->pin, SERVO_MIN_PULSE_TIME, SERVO_MAX_PULSE_TIME);
	servo.write(motorConfig->initialAngle);
}

void servoMotor::write(float32 angle) {
	if (!validAngle(angle))
		return;
	servo.write(angle);
	motorConfig->currentAngle = angle;
}

void servoMotor::returnToInitial() {
	write(motorConfig->initialAngle);
}

