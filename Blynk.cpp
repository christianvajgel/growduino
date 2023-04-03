#include <BlynkSimpleEsp32.h>
#include "Wi-Fi-Connection.h"
#include "Soil-Sensor.h"
#include "Plant.h"
#include "Ambient-Temperature.h"
#include "Ambient-Humidity.h"
#include "Air-Quality.h"
#include "Barometer.h"

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

void send_data_to_blynk() {

  Blynk.virtualWrite(V0, ambient_atmospheric_pressure);
  Blynk.virtualWrite(V1, ambient_temperature);
  Blynk.virtualWrite(V3, ambient_air_humidity);
  Blynk.virtualWrite(V4, soil_humidity_percentage);
  Blynk.virtualWrite(V5, plant_target_soil_humidity_percentage);
  Blynk.virtualWrite(V6, ambient_air_quality_indicator);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void connect_to_blynk() {
  Blynk.begin(BLYNK_AUTH_TOKEN, CONNECTED_SSID, CONNECTED_PASSWORD);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void update_blynk() {
  Blynk.run();
  timer.run();
  send_data_to_blynk();
}