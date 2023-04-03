#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

// sensor
extern Adafruit_SHT31 sht31;

// variable responsible to hold ambient temperature value
extern float ambient_temperature;

// method to setup the SHT31 temperature and humidity sensor
void setup_temperature_humidity_sensor();

// method to check the ambient temperature and update the variable
void check_ambient_temperature();

#endif