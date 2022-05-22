/******************************************************************************
 *
 * Module: 2 Link manipulator Arm
 *
 * File Name: arm_2Link.cpp
 *
 * Description: Source file for a 2-link arm manipulator
 *
 * Date Created: 1/4/2022
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#include "arm_2Link.h"
#include <math.h>

 /**********************************Public Functions**********************************/
bool arm_2Link::IKPM(sint32 x, sint32 y)
{
	//Calculate Theta2
	float32 th2 = acos(((float)x * x + y * y - arm1Length * arm1Length - arm2Length * arm2Length) / (2 * arm1Length * arm2Length));
	float32 det = arm1Length * arm1Length + arm2Length * arm2Length + 2 * arm1Length * arm2Length * cos(th2);

	//Calculate Theta1
	float32 cth1 = ((float)arm1Length * x + arm2Length * y * sin(th2) + arm2Length * x * cos(th2)) / (float)det;
	float32 sth1 = ((float)arm1Length * y + arm2Length * y * cos(th2) - arm2Length * x * sin(th2)) / (float)det;
	float32 th1 = (atan2(sth1, cth1) * 180) / (float)PI;

#if DEBUG_MODE == (1u)
	Serial.println("Angles:");
	Serial.println(th1);
	Serial.println(th2 * 180 / PI);
#endif

	if (!(baseMotor->validAngle(th1) && midMotor->validAngle(th2))) {
#if DEBUG_MODE == (1u)
		Serial.println("Error: Invalid position.");
#endif
		return false;
	}
	
	motorAngles.th1 = th1;
	motorAngles.th2 = th2 * 180 / PI;
	return true;
}

arm_2Link::arm_2Link(float32 length1, float32 length2,
	MotorType base,
	MotorType mid,
	MotorType end,
	MotorType eff) {

	arm1Length = length1;
	arm2Length = length2;
	
	switch (base) {
	case type_Servo:
	{
		servoMotor* servo = new servoMotor();
		baseMotor = servo;
		break;
	}
	case type_Stepper:
	{
		stepperMotor* stepper = new stepperMotor();
		baseMotor = stepper;
		testvar = 5;
		break;
	}
	case type_DC:
	{
		break;
	}
	}

	switch (mid) {
	case type_Servo:
	{
		servoMotor* servo = new servoMotor();
		midMotor = servo;
		break;
	}
	case type_Stepper:
	{
		stepperMotor* stepper = new stepperMotor();
		midMotor = stepper;
		testvar = 5;
		break;
	}
	case type_DC:
	{
		break;
	}
	}

	switch (end) {
	case type_Servo:
	{
		servoMotor* servo = new servoMotor();
		endMotor = servo;
		break;
	}
	case type_Stepper:
	{
		stepperMotor* stepper = new stepperMotor();
		endMotor = stepper;
		break;
	}
	case type_DC:
	{
		break;
	}
	}

	switch (eff) {
	case type_Servo:
	{
		servoMotor* servo = new servoMotor();
		endEffector = servo;
		break;
	}
	case type_Stepper:
	{
		stepperMotor* stepper = new stepperMotor();
		endEffector = stepper;
		break;
	}
	case type_DC:
	{
		break;
	}
	}

}
void arm_2Link::init(
	struct Motor_configType* base,
	struct Motor_configType* mid,
	struct Motor_configType* end,
	struct Motor_configType* eff)
{
	Serial.println(testvar);
	baseMotor->init(base);
	midMotor->init(mid);
	endMotor->init(end);
	endEffector->init(eff);
}

void arm_2Link::moveCoord(sint32 x, sint32 y) {
#if DEBUG_MODE==(1u)
	Serial.println("Moving to coords.");
#endif
	if (!IKPM(x, y)) {
		return;
	}
	baseMotor->write(motorAngles.th1);
	delay(ARM2_LINK_DEFAULT_HOLDTIME_MS);
	midMotor->write(motorAngles.th2);
#if DEBUG_MODE==(1u)
	Serial.println("Movement completed.");
#endif
}

void arm_2Link::returnToInitialPos() {
	baseMotor->returnToInitial();
	delay(ARM2_LINK_DEFAULT_HOLDTIME_MS);
	midMotor->returnToInitial();
	endMotor->returnToInitial();
}
