#include "WString.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "HTTP-Requests.h"
#include "Wi-Fi-Connection.h"
#include <string.h>
#include "Plant.h"
#include "Soil-Sensor.h"
#include "Water-Pump.h"
#include "Ambient-Temperature.h"
#include "Ambient-Humidity.h"
#include "Air-Quality.h"
#include "Barometer.h"
#include "Status-LED-Light.h"

using namespace ArduinoJson;

DynamicJsonDocument last_information_json(1024);


DynamicJsonDocument mergeJson(DynamicJsonDocument json1, DynamicJsonDocument json2) {

  DynamicJsonDocument result(json1.capacity() + json2.capacity());

  // Copy json1 to result
  for (const auto& kv : json1.as<JsonObject>()) {
    result[kv.key()] = kv.value();
  }

  // Copy json2 to result, overwriting values from json1
  for (const auto& kv : json2.as<JsonObject>()) {
    result[kv.key()] = kv.value();
  }

  // Serial.println("RESULT: \n\n");

  // // print each key and attribute from the JSON
  // for (const auto& kv : result.as<JsonObject>()) {
  //   const char* key = kv.key().c_str();
  //   const char* value = kv.value().as<const char*>();
  //   Serial.print(key);
  //   Serial.print(": ");
  //   serializeJsonPretty(kv.value(), Serial);
  //   Serial.println("\n\n");
  // }

  return result;
}

DynamicJsonDocument create_json_file_to_cloud() {


  turn_on_led_light_on_color("purple");

  DynamicJsonDocument json_date_time_data = get_date_time_by_ip();

  DynamicJsonDocument json_astronomical_data = get_astronomical_information_by_ip();

  turn_off_led_lights();


  DynamicJsonDocument json_to_cloud = mergeJson(json_date_time_data, json_astronomical_data);

  // adicionar campos de parametros do metodo aqui
  json_to_cloud["plant_name"] = plant_name;
  json_to_cloud["plant_target_soil_humidity_percentage"] = plant_target_soil_humidity_percentage;
  json_to_cloud["soil_humidity_percentage"] = soil_humidity_percentage;
  json_to_cloud["plant_watered"] = plant_watered;

  json_to_cloud["ambient_temperature"] = ambient_temperature;
  json_to_cloud["ambient_air_humidity"] = ambient_air_humidity;

  json_to_cloud["ambient_atmospheric_pressure"] = ambient_atmospheric_pressure;
  json_to_cloud["ambient_altitude"] = ambient_altitude;

  json_to_cloud["ambient_air_quality_indicator"] = ambient_air_quality_indicator;

  json_to_cloud["co_ppm"] = co_ppm;
  json_to_cloud["co2_ppm"] = co2_ppm;
  json_to_cloud["alcohol_ppm"] = alcohol_ppm;
  json_to_cloud["toluen_ppm"] = toluen_ppm;
  json_to_cloud["nh4_ppm"] = nh4_ppm;
  json_to_cloud["aceton_ppm"] = aceton_ppm;

  last_information_json.clear();

  last_information_json = json_to_cloud;

  return json_to_cloud;
}

void read_and_print_json_file(DynamicJsonDocument json_file_to_print) {

  Serial.println("\n::: P R I N T I N G  J S O N  F I L E :::\n");

  for (const auto& kv : json_file_to_print.as<JsonObject>()) {
    const char* key = kv.key().c_str();
    const char* value = kv.value().as<const char*>();
    Serial.print(key);
    Serial.print(": ");
    serializeJsonPretty(kv.value(), Serial);
    Serial.println();
  }

  // Calculate the size of the serialized JSON data
  size_t jsonSize = measureJson(json_file_to_print);

  // Print the size of the serialized JSON data in kebibytes
  Serial.print("\n\tJSON size: ");
  Serial.print(jsonSize / 1024.0, 2);  // KiB
  //Serial.print(jsonSize / 1024.0 / 1024.0, 2); // MiB
  Serial.println(" KiB");

  Serial.println("\n::: E N D  O F  J S O N  F I L E :::\n");
}
