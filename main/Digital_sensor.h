#pragma once

#include "Arduino.h"
#include "std_types.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS (12u)  //pin 2 of arduino
#define MAX_ALLOWABLE_TEMP (105u)
#define TIMEOUT_MINUTES (30u)

#ifndef DEBUG_MODE 
#define DEBUG_MODE (0u)
#endif

class Digital_sensor {

private:
	OneWire* oneWireInterface;
	DallasTemperature sensors;
public:
	void Digital_Sensor();
	void delayUntilTempReached(uint8&);
	float getTemperature();
};
