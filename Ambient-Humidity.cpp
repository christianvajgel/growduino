#include "Ambient-Humidity.h"
#include "Ambient-Temperature.h"
#include "Multiplexer.h"

float ambient_air_humidity;

const uint8_t SENSOR_MULTIPLEXER_CHANNEL = 6;

void check_ambient_air_humidity_percentage() {

  ambient_air_humidity = 0.0;

  select_multiplexer_channel(SENSOR_MULTIPLEXER_CHANNEL);

  ambient_air_humidity = sht31.readHumidity();

  if (ambient_air_humidity != 0.0) {
    // if (!isnan(ambient_air_humidity)) {
    Serial.print("Ambient humidity = " + String(ambient_air_humidity) + " %");
    Serial.println();
  } else {
    ambient_air_humidity = 0.0;
    Serial.println("Failed to read ambient humidity");
  }
}
