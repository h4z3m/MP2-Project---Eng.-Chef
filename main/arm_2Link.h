/******************************************************************************
 *
 * Module: 2 Link manipulator Arm
 *
 * File Name: arm_2Link.h
 *
 * Description: Header file for a 2-link arm manipulator with servo motors
 *
 * Date Created: 1/4/2022
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#pragma once
#define DEBUG_MODE (1u)
#include "Motor.h"
#include "servoMotor.h"
#include "stepperMotor.h"


#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
 /********************************** Definitions **********************************/

/*Delay between the motion of the second link after the first link has completed its movement*/
#define ARM2_LINK_DEFAULT_HOLDTIME_MS (500)	

/******************************************************************************
 *
 * Structure Name: anglePair
 *
 * Structure Description: Used to store the angles of the 2 motors in a 2- Link arm
 *	1. float32 th1: Angle of the base motor
 *  2. float32 th2: Angle of the elbow motor
 *
 *******************************************************************************/
struct anglePair {
	float32 th1;
	float32 th2;
};
enum MotorType {
	type_Servo,
	type_Stepper,
	type_DC
};

class arm_2Link {
private:
	//The length of the base-elbow link
	float32 arm1Length;
	//The length of the elbow-wrist link
	float32 arm2Length;
	//Motors at each joint
	Motor* baseMotor;
	Motor* midMotor;
	Motor* endMotor;
	Motor* endEffector;
	//Current angles of the base & elbow joints
	anglePair motorAngles;
	/******************************************************************************
	 * Function Name: IKPM
	 * Description: Solves the equation for the required coordinates and calculates
	 *		new angles for that position based on the inverse kinematics model.
	 * Args: -sint32 x: x-coordinate
	 *		 -sint32 y: y-coordinate
	 * Returns: bool: If the movement is possible or not possible
	 *******************************************************************************/
	bool IKPM(sint32 x, sint32 y);
public:
	int testvar=99;

	/******************************************************************************
	 * Function Name: arm_2Link
	 * Description: Constructor for the arm which takes the length of both arms
	 * Args: -float32 length1: The length of the arm from the base to elbow joint
	 *		 -float32 length2: The length of the arm from the elbow to wrist joint
	 * Returns: void
	 *******************************************************************************/
	arm_2Link::arm_2Link(float32 length1, float32 length2, MotorType base, MotorType mid, MotorType end, MotorType eff);
	/******************************************************************************
	 * Function Name: init
	 * Description: Initializes all 3 motors for the arm
	 * Args: -Motor_configType* base: Pointer to the configuration struct for the base motor
	 *		 -Motor_configType* mid:  Pointer to the configuration struct for the elbow motor
	 *		 -Motor_configType* end:  Pointer to the configuration struct for the
	 *			wrist motor
	 *		 -Motor_configType* eff:  Pointer to the configuration struct for the
	 *			end effector motor 
	 * Returns: void
	 *******************************************************************************/
	void init(struct Motor_configType* base, struct Motor_configType* mid, struct Motor_configType* end, struct Motor_configType* eff);

	/******************************************************************************
	 * Function Name: moveCoord
	 * Description: Moves the arm to the given x,y position
	 * Args: -sint32 x: x-coordinate
	 *		 -sint32 y: y-coordinate
	 * Returns: void
	 *******************************************************************************/
	void moveCoord(sint32 x, sint32 y);

	/******************************************************************************
	 * Function Name: returnToInitialPos
	 * Description: Returns all motors to their initial position.
	 * Args: void
	 * Returns: void
	 *******************************************************************************/
	void returnToInitialPos();

};