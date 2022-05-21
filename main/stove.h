#ifndef stove_h
#define stove_h
#include "Arduino.h"
#include "Digital_sensor.h"

#define MAX_SAFE_TEMP_CELSIUS (105)
#define RELAY_HIGH_LOGIC (HIGH)
#define RELAY_LOW_LOGIC (LOW)
#define RELAY_PIN (5)

class stove {
  public:
    void init();
    void heat_minutes(char minutes, Digital_sensor &sensor);
    void heat_minutes_for_normal_humans(char minutes);
};
#endif
