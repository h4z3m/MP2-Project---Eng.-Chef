#pragma once
#include "std_types.h"
#include "Arduino.h"

struct Motor_configType {
	sint8 pin;
	sint8 dirPin;
	float32 initialAngle;
	float32 currentAngle;
	float32 minAngle;
	float32 maxAngle;
	float32 maxSpeed;
};

class Motor
{
protected:
	struct Motor_configType* motorConfig = nullptr;
public:
	virtual bool validAngle(float32 angle) {
		/*if (angle < 0) {
			angle += 360;
		}
		else if (angle > 360) {
			angle -= 360;
		}*/
		return angle >= motorConfig->minAngle && angle <= motorConfig->maxAngle;
	}
	virtual void init(struct Motor_configType* conf) = 0;
	virtual void write(float32 angle) = 0;
	virtual void returnToInitial() = 0;
};

