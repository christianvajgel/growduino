#ifndef WATER_PUMP_H
#define WATER_PUMP_H
#include <Arduino.h>

extern bool plant_watered;

extern bool watering_plant;

// method to water the plant until a target humidity percentage level
void water_plant_until_umidity_level(int target_humidity_percentage_level);

// method to turn on the water pump
void turn_on_water_pump();

// method to turn off the water pump
void turn_off_water_pump();

#endif