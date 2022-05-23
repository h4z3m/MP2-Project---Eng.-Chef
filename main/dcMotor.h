/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dcMotor.h
 *
 * Description: Header file for custom DC motor class
 *
 * Date Created: 14/5/2022
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#pragma once
#include "Motor.h"
/********************************** Types declarations **********************************/
typedef enum DC_direction {
};

class dcMotor : public Motor{
private:

public:

    dcMotor();
    void init(struct Motor_configType* conf);
    void write(float32 angle);
    void changeDirection(uint8 dir);
    void setSpeed(uint8 Motor_speed);
    void rotateDistance(uint8 distance);
    void returnToInitial();
};

