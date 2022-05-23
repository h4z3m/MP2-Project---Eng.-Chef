/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dcMotor.cpp
 *
 * Description: Source file for custom DC motor class
 *
 * Date Created: 14/5/2022
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#include "dcMotor.h"


/********************************** Public Functions **********************************/
dcMotor::dcMotor() {

}
void dcMotor::init(struct Motor_configType* conf) {
	motorConfig = conf;
	pinMode(motorConfig->pin, OUTPUT);
	pinMode(motorConfig->dirPin, OUTPUT);
}

void dcMotor::write(float32 angle) {
	if (!validAngle(angle))
		return;
	///////////////////////////////////////////////////////
	///
	///
	/// 
	///	Yet to be implemented using set speed and delays?
	/// 
	/// 
	/// 
	/// 

}

void dcMotor::setSpeed(uint8 Motor_speed)
{
	analogWrite(motorConfig->pin, Motor_speed);
}

void dcMotor::changeDirection(DC_direction dir)
{
	analogWrite(motorConfig->dirPin, dir);
}

void dcMotor::rotateDistance(uint8 distance) {
	///////////////////////////////////////////////////////
	///
	///
	/// 
	///		Yet to be implemented
	///		For the defined gear radius, rotate for a specific time??
	///		****
	///		**** Implement a global (outside of this class) function to open/close containers using this one
	///		void openContainer(dcMotor* myDC);
	/// 
	/// 
	/// 
	/// 
}
void dcMotor::returnToInitial() {
	write(motorConfig->initialAngle);
}

