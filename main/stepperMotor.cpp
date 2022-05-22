/******************************************************************************
 *
 * Module: Stepper Motor
 *
 * File Name: stepperMotor.cpp
 *
 * Description: Source file for custom stepper motor class using basic Arduino library Stepper.h
 *
 * Date Created: 7/5/2022
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#include "stepperMotor.h"

stepperMotor::stepperMotor() {

}
void stepperMotor::init(struct Motor_configType* conf)
{
	motorConfig = conf;
	pinMode(motorConfig->pin, OUTPUT);
	pinMode(motorConfig->dirPin, OUTPUT);
	changeDirection(currDir);
	//this->write(motorConfig->initialAngle);
}


void stepperMotor::write(float32 revs)
{
//	//Safety check to ensure the motor does not surpass 360deg 
//	//of rotation around itself
//	/*while(angle>=motorConfig->maxAngle){
//		angle -= 360;
//	}*/
//	
//	//1. Calculate difference between current angle and given angle
//	//Set current angle to new angle
//	float32 targetAngle = (float32)angle - motorConfig->currentAngle;
//
//	/*if (!validAngle(targetAngle))
//		return;*/
//
//#if DEBUG_MODE==(1u)
//	Serial.print("Angle:");
//	Serial.println(targetAngle);
//#endif
//
//	//2. Determine direction to rotate
//	if (targetAngle >= 0.0f) {
//		changeDirection(CCW);
//	}
//	else {
//		changeDirection(CW);
//	}
//
//	//3. Rotate by the given angle
//	uint16 totalSteps = ceil(float(abs(targetAngle)) / stepSize);
//#if DEBUG_MODE==(1u)
//	Serial.print("Angle:");
//	Serial.println(totalSteps);
//#endif
	uint16 totalSteps = float(revs) * SPR;
	for (uint16 steps=0; steps < totalSteps; ++steps) {
		digitalWrite(motorConfig->pin, HIGH);
		delayMicroseconds(motorConfig->maxSpeed);
		digitalWrite(motorConfig->pin, LOW);
		delayMicroseconds(motorConfig->maxSpeed);
	}
	//motorConfig->currentAngle =   angle;

}


void stepperMotor::changeDirection(stepperDirection dir) 
{
	currDir = dir;
	digitalWrite(motorConfig->dirPin, dir);
}
void stepperMotor::invertDirection() {
	if (currDir == CW)
	{
		currDir = CCW;
	}
	else
	{
		currDir = CW;
	}
	digitalWrite(motorConfig->dirPin, currDir);
}

void stepperMotor::setSpeed(sint8 a_motorSpeed)
{
	motorConfig->maxSpeed = a_motorSpeed;
}


void stepperMotor::returnToInitial() {
	write(motorConfig->initialAngle);
}
