#ifndef AMBIENT_AIR_HUMIDITY_H
#define AMBIENT_AIR_HUMIDITY_H
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

// variable responsible to hold ambient air humidity value
extern float ambient_air_humidity;

// method to check the ambient air humidity and update the variable
void check_ambient_air_humidity_percentage();

#endif