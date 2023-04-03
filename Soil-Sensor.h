#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H
#include <Arduino.h>

extern int soil_humidity_sensor_read_value;

extern int soil_humidity_percentage;

// method to read the value of the soil sensor
void read_soil_humidity_from_sensor();

// method to obtain the percentage of the value read on the method read_soil_humidity_from_sensor()
void check_soil_umidity_percentage();

#endif