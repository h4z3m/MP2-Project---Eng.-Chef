#include "stove.h"

void stove::init()
{
  pinMode(RELAY_PIN, OUTPUT);
  //Turn off the stove initially
  digitalWrite(RELAY_PIN, RELAY_LOW_LOGIC);
}

void stove::heat_minutes_for_normal_humans(char minutes) {
    if (minutes <= 0)
        return;
    unsigned int time_ms = minutes * 60*1000;
    float tempr;
    //Turn on the stove
    digitalWrite(RELAY_PIN, RELAY_HIGH_LOGIC);
    delay(time_ms);
}
void stove::heat_minutes(char minutes, temp_sensor &sensor)
{
  if (minutes <= 0)
    return;
  unsigned int time_seconds = minutes * 60;
  float tempr;
  //Turn on the stove
  digitalWrite(RELAY_PIN, RELAY_HIGH_LOGIC);
  //Check if time has run out or max temp has been reached
  while (time_seconds > 0 && tempr < MAX_SAFE_TEMP_CELSIUS)
  {
    tempr = sensor.getTemperature();
    time_seconds--;
    //Delay one second
    delay(1000);
  }
  //Turn off the stove after time runs out
  digitalWrite(RELAY_PIN, RELAY_LOW_LOGIC);
}
