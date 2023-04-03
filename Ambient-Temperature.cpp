#include "esp32-hal.h"
#include "Ambient-Temperature.h"
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "Multiplexer.h"
#include "Delay.h"


Adafruit_SHT31 sht31 = Adafruit_SHT31();

float ambient_temperature;

const uint8_t SENSOR_MULTIPLEXER_CHANNEL = 6;

void setup_temperature_humidity_sensor() {

  // check multiplexer channel before setting up
  select_multiplexer_channel(SENSOR_MULTIPLEXER_CHANNEL);

  delay(DELAY_100_MILLISECONDS);

  // if (!sht31.begin(0x40)) {

  if (!sht31.begin(0x44)) {
    // if (!sht31.begin(0x70)) {
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}

void check_ambient_temperature() {

  ambient_temperature = 0.0;

  select_multiplexer_channel(SENSOR_MULTIPLEXER_CHANNEL);

  ambient_temperature = sht31.readTemperature();

  if (ambient_temperature != 0.0) {
    // if (!isnan(ambient_temperature)) {
    Serial.print("Temp *C = ");
    Serial.println(ambient_temperature);
  } else {
    ambient_temperature = 0.0;
    Serial.println("Failed to read temperature");
  }
}
