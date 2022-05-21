#pragma once
#include "Digital_sensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

void Digital_sensor::Digital_Sensor()
{
	OneWire* oneW = new OneWire(ONE_WIRE_BUS);
	oneWireInterface = oneW;
	sensors.setOneWire(oneWireInterface);
}

void Digital_sensor::delayUntilTempReached(uint8& a_targetTemp)
{

	uint16 time = TIMEOUT_MINUTES* 60;
	//Temperature must be within max allowable safe temperature or given temp
	//Maximum time 
	while (getTemperature() < MAX_ALLOWABLE_TEMP || getTemperature() < a_targetTemp || time<TIMEOUT_MINUTES) {
		//Delay 1s until next poll
		delay(1000);
		--time;
	}
}

float Digital_sensor::getTemperature()
{

	sensors.requestTemperatures();
	float temp = sensors.getTempC(0);
	Serial.print("DEBUG :: Temperature : ");
	Serial.println(sensors.getTempCByIndex(0));
	Serial.println();
	return temp;
}
