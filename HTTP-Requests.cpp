#include "HardwareSerial.h"
//#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include "Wi-Fi-Connection.h"
#include <string.h>

const char* ENDPOINT_DATE_TIME = "http://worldtimeapi.org/api/ip";

// const String IP_ADDRESS = get_external_ipv4();

const String API_KEYS[] = { "",
                            "",
                            "" };  // List of API keys to try

DynamicJsonDocument get_date_time_by_ip() {

  DynamicJsonDocument newDoc(1024);

  HTTPClient http;
  http.begin(ENDPOINT_DATE_TIME);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {

    String payload = http.getString();

    // Parse JSON payload
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    //Extract desired properties
    const char* abbreviation = doc["abbreviation"];
    const char* datetime = doc["datetime"];
    int raw_offset = doc["raw_offset"];
    unsigned long unixtime = doc["unixtime"];
    const char* utc_datetime = doc["utc_datetime"];
    const char* utc_offset = doc["utc_offset"];
    const char* client_ip = doc["client_ip"];

    // Add desired properties to new JSON
    newDoc["timezone_difference_to_utc_hours"] = String(abbreviation);
    newDoc["timezone_difference_to_utc_hours_complete"] = String(utc_offset);
    newDoc["timezone_difference_to_utc_seconds"] = raw_offset;
    newDoc["local_datetime"] = String(datetime);
    newDoc["epoch_time"] = unixtime;
    newDoc["utc_datetime"] = String(utc_datetime);
    newDoc["client_ip"] = String(client_ip);

  } else {
    Serial.println("Error making HTTP request");
  }

  http.end();

  return newDoc;
}

DynamicJsonDocument get_astronomical_information_by_ip() {

  const String IP_ADDRESS = get_external_ipv4();

  DynamicJsonDocument newDoc(1024);

  for (const auto& API_KEY : API_KEYS) {  // try all the API keys
    String endpoint = "https://api.ipgeolocation.io/astronomy?apiKey=" + API_KEY + "&ip=" + IP_ADDRESS;

    Serial.println(endpoint);

    HTTPClient http;
    http.begin(endpoint);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {

      String payload = http.getString();

      // Parse JSON payload
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      //Extract desired properties
      const char* sunrise = doc["sunrise"];
      const char* sunset = doc["sunset"];
      const char* solar_noon = doc["solar_noon"];
      const char* day_length = doc["day_length"];

      // Add desired properties to new JSON
      newDoc["sunrise_hour"] = String(sunrise);
      newDoc["sunset_hour"] = String(sunset);
      newDoc["solar_noon_hour"] = String(solar_noon);
      newDoc["day_length_duration"] = String(day_length);

      http.end();

      return newDoc;

    } else {
      Serial.println("Error making HTTP request with API key: " + API_KEY);
    }

    http.end();
  }

  Serial.println("All API keys failed to make a successful request");

  return newDoc;
}
