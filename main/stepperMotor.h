/******************************************************************************
 *
 * Module: Stepper Motor
 *
 * File Name: stepperMotor.h
 *
 * Description: Header file for custom stepper motor class using basic Arduino library Stepper.h
 *
 * Date Created: 7/5/2022
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#pragma once
#include "Motor.h"
#include "std_types.h"

/********************************** Types declarations **********************************/

enum stepperDirection {
    CCW = LOW,
    CW = HIGH
};

class stepperMotor: public Motor {
private:
    //struct Motor_configType* motorConfig = nullptr;
    const float32 stepSize = 1.8;
    const uint16 SPR = 1600;
public:
    stepperDirection currDir = stepperDirection::CW;

    stepperMotor();
    void init(struct Motor_configType* conf);
    void write(float32 revs);
    void write(sint16 angle);
    void changeDirection(stepperDirection dir);
    void invertDirection();
    void setSpeed(sint8 Motor_speed);
    void returnToInitial();
};