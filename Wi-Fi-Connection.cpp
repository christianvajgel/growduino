#include "HardwareSerial.h"
#include "Wi-Fi-Connection.h"
#include <WiFi.h>  // ESP32 board
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include "Telegram.h"
#include "Delay.h"
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

const char* wifi_ssids[] = { "" };      // Array of Wi-Fi SSIDs
const char* wifi_passwords[] = { "" };  // Array of Wi-Fi passwords

const char* ENDPOINT_IPV4 = "https://api.ipify.org";
const char* ENDPOINT_IPV6 = "https://api64.ipify.org/?format=json";

char* CONNECTED_SSID = "";
char* CONNECTED_PASSWORD = "";

void connect_to_wifi() {

  int num_wifi_networks = sizeof(wifi_ssids) / sizeof(wifi_ssids[0]);  // Calculate number of Wi-Fi networks

  for (int i = 0; i < num_wifi_networks; i++) {
    const char* ssid = wifi_ssids[i];
    const char* password = wifi_passwords[i];
    Serial.println("@ WI-FI:    Connecting to Wi-Fi network: " + String(ssid));
    WiFi.begin(ssid, password);

    unsigned long start_time = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start_time) < 10000) {  // Try connecting for 10 seconds
      delay(DELAY_250_MILLISECONDS);
      Serial.print(". ");
    }

    if (WiFi.status() == WL_CONNECTED) {  // Connection successful
      Serial.println("@ WI-FI:    Connected to " + String(ssid) + " :::: RSSI: " + WiFi.RSSI() + " dBm");
      break;  // Exit loop if connection successful
    } else {  // Connection unsuccessful
      Serial.println("@ WI-FI:    Failed to connect to " + String(ssid));
      if (i == num_wifi_networks - 1) {  // Last Wi-Fi network in the array
        Serial.println("@ WI-FI:    Could not connect to any Wi-Fi network");
      }
    }
  }
#ifdef ESP32
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
#endif
}

void check_wifi_connection() {

  switch (WiFi.status()) {
    case WL_IDLE_STATUS:
      Serial.println("WiFi is in idle state");
      connect_to_wifi();
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("Couldn't find the SSID");
      connect_to_wifi();
      break;
    case WL_CONNECT_FAILED:
      Serial.println("Connection failed");
      connect_to_wifi();
      break;
    case WL_CONNECTION_LOST:
      Serial.println("Connection lost");
      connect_to_wifi();
      break;
    case WL_DISCONNECTED:
      Serial.println("Disconnected from WiFi");
      connect_to_wifi();
      break;
    case WL_CONNECTED:
      Serial.println("WiFi connected");
      Serial.println("@ WI-FI:    Connected to " + String(WiFi.SSID()) + " :::: RSSI: " + WiFi.RSSI() + " dBm");
      break;
    default:
      Serial.println("Unknown status");
      connect_to_wifi();
      break;
  }
}

String get_external_ipv4() {

  HTTPClient http;
  String response;

  // List of API endpoints to try in case the first one fails
  String apiEndpoints[] = { "https://api.ipify.org", "https://icanhazip.com", "https://ipapi.co/ip/" };

  for (int i = 0; i < 3; i++) {         // try all the API endpoints
    if (http.begin(apiEndpoints[i])) {  // try connecting to the current API endpoint
      int httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK) {
        response = http.getString();
        break;
      } else {
        Serial.printf("IPv4: HTTP request failed with error code %d\n", httpCode);
      }
      http.end();
    } else {
      Serial.println("IPv4: Failed to connect to server");
    }

    delay(DELAY_1_SECOND);  // wait 1 second before trying again
  }

  if (response == "") {
    Serial.println("IPv4: Failed to get external IP address");
  }

  // Serial.println("IPv4: " + response);

  return response;
}

String get_external_ipv6() {
  HTTPClient http;
  String response;

  if (http.begin("https://ipapi.co/ip/")) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      response = http.getString();
    } else {
      Serial.printf("IPv6: HTTP request failed with error code %d\n", httpCode);
    }
    http.end();
  } else {
    Serial.println("IPv6: Failed to connect to server");
  }

  Serial.println("IPv6: " + response);

  return response;
}
