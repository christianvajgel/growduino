#include "Barometer.h"
#include "Multiplexer.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP280 bmp;

unsigned long delayTime;

float ambient_atmospheric_pressure;
float ambient_altitude;

float ambient_temperature_bmp280;

float altitude_correction_in_meters = 6.0;

#define BMP_SDA 21
#define BMP_SCL 22

const uint8_t SENSOR_MULTIPLEXER_CHANNEL = 4;

void setup_barometer_sensor() {
  bool status;

  select_multiplexer_channel(SENSOR_MULTIPLEXER_CHANNEL);

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bmp.begin(0x76);
  // status = bmp.begin(0x70);

  if (!status) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1)
      ;
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void check_ambient_atmospheric_pressure() {

  ambient_atmospheric_pressure = 0.0;

  select_multiplexer_channel(SENSOR_MULTIPLEXER_CHANNEL);

  ambient_atmospheric_pressure = bmp.readPressure() / 100.0F;
  // ambient_atmospheric_pressure = bmp.readPressure() / 101325.0F;

  if (ambient_atmospheric_pressure != 0.0) {
    Serial.println("####### \n Pressure BMP280 = " + String(ambient_atmospheric_pressure) + " atm");
    Serial.println(ambient_atmospheric_pressure);
  } else {
    ambient_atmospheric_pressure = 0.0;
    Serial.println("Failed to read pressure with BMP280 sensor.");
  }
}

void check_ambient_altitude() {

  ambient_altitude = 0.0;

  select_multiplexer_channel(SENSOR_MULTIPLEXER_CHANNEL);

  ambient_altitude = bmp.readAltitude(ambient_atmospheric_pressure) + altitude_correction_in_meters;

  Serial.println("####### \n Altitude 1 BMP280 = " + String(ambient_altitude) + " m");

  if (ambient_altitude != 0.0) {

    Serial.println("####### \n Altitude 2 BMP280 = " + String(ambient_altitude) + " m");

  } else {
    ambient_altitude = 0.0;
    Serial.println("Failed to read altitude with BMP280 sensor.");
  }
}

void check_ambient_temperature_BMP280() {

  ambient_temperature_bmp280 = 0.0;

  select_multiplexer_channel(SENSOR_MULTIPLEXER_CHANNEL);

  ambient_temperature_bmp280 = bmp.readTemperature();

  if (ambient_temperature_bmp280 != 0.0) {
    Serial.print("Temp *C BMP280 = ");
    Serial.println(ambient_temperature_bmp280);
  } else {
    ambient_temperature_bmp280 = 0.0;
    Serial.println("Failed to read temperature with BMP280 sensor.");
  }
}
