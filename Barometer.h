#ifndef BAROMETER_H
#define BAROMETER_H
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

extern Adafruit_BMP280 bmp;  // create BMP280 object

extern float ambient_atmospheric_pressure;
extern float ambient_altitude;
extern float ambient_temperature_bmp280;

void setup_barometer_sensor();

void check_ambient_atmospheric_pressure();

void check_ambient_altitude();

void check_ambient_temperature_BMP280();

#endif