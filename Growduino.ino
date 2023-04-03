#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "OLED-Display.h"
#include "Water-Pump.h"
#include "PIN-Namespace.h"
#include "Status-LED-Light.h"
#include "Soil-Sensor.h"
#include "OLED-Images-Animations.h"
#include "Wi-Fi-Connection.h"
#include "JSON-Operations.h"
#include "HTTP-Requests.h"
#include <UniversalTelegramBot.h>
#include <ThingSpeak.h>
#include "Telegram.h"
#include "Plant.h"
#include "Delay.h"
#include "Ambient-Temperature.h"
#include "Ambient-Humidity.h"
#include "Blynk.h"
#include <Wire.h>
#include "Multiplexer.h"
#include "Barometer.h"
#include "Air-Quality.h"
#include "Buzzer.h"

unsigned long myChannelNumber = 2031957;
const char* myWriteAPIKey = "ZLVRI8ORM71L8K4C";

WiFiClient client_thing_speak;

void setup() {

  Serial.begin(9600);

  Wire.begin();

  Serial.println("Registrando PINOS RGB");

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  turn_on_led_light_on_color("white");

  setup_multiplexer();

  pinMode(BUZZER_DAC_PIN, OUTPUT);

  pinMode(WATER_PUMP_PIN, OUTPUT);
  turn_off_water_pump();
  delay(DELAY_250_MILLISECONDS);

  Serial.println("Setup OLED");

  setup_oled_display();

  pinMode(ledPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  Serial.println("Inciando SHT31");
  setup_temperature_humidity_sensor();
  delay(DELAY_250_MILLISECONDS);

  Serial.println("Setup Barometer");
  setup_barometer_sensor();

  setup_air_quality_sensor();

  connect_to_wifi();

  connect_to_blynk();

  ThingSpeak.begin(client_thing_speak);

  turn_off_led_lights();
}

void loop() {

  turn_off_water_pump();

  update_bot();

  // PASSO 1
  // servicos que precisam ser atualizados imediatamente

  turn_on_led_light_on_color("orange");

  // method to check the humidity percentage of the soil
  check_soil_umidity_percentage();
  delay(DELAY_100_MILLISECONDS);

  // // method to check the ambient temperature
  check_ambient_temperature();
  delay(DELAY_100_MILLISECONDS);

  // // method to check the ambient air humidity percentage
  check_ambient_air_humidity_percentage();
  delay(DELAY_100_MILLISECONDS);


  // method to check the ambient atmospheric pressure
  check_ambient_atmospheric_pressure();
  delay(DELAY_250_MILLISECONDS);

  // method to check the ambient altitude using BMP280 sensor
  check_ambient_temperature_BMP280();
  delay(DELAY_250_MILLISECONDS);

  // method to check the ambient altitude
  check_ambient_altitude();
  delay(DELAY_250_MILLISECONDS);


  // // method to check the ambient air quality
  check_air_quality_conditions();
  delay(DELAY_250_MILLISECONDS);

  turn_off_led_lights();

  // method to update Telegram Bot
  update_bot();

  turn_on_led_light_on_color("purple");
  update_blynk();
  turn_off_led_lights();

  delay(DELAY_100_MILLISECONDS);

  update_bot();


  // PASSO 2
  // logica de irrigacao

  // sensor menor que 25% de umidade (valor hardcoded: 4095)
  if (soil_humidity_percentage <= 25 && soil_humidity_percentage < plant_target_soil_humidity_percentage) {

    Serial.println("IF 0-25");

    // show the humidity information on display
    display_soil_humidity_percentage();

    // turn on the LED
    turn_on_led_light_on_color("blue");


    //delay(DELAY_3_SECONDS);

    // display the image of low humidity
    // display_bitmap_image(low_umidity_image, 0, 0, oled_display_2, 1);

    // method to activate the water pump
    water_plant_until_umidity_level(plant_target_soil_humidity_percentage);

    turn_off_led_lights();

    delay(DELAY_250_MILLISECONDS);

  }

  // sensor maior que 25% e menor que 50% de umidade (valor hardcoded: 2750)
  else if (soil_humidity_percentage > 25 && soil_humidity_percentage < 50 && soil_humidity_percentage < plant_target_soil_humidity_percentage) {

    Serial.println("IF 25 - 50");

    // show the humidity information on display
    display_soil_humidity_percentage();

    turn_on_led_light_on_color("green");

    // method to activate the water pump
    water_plant_until_umidity_level(plant_target_soil_humidity_percentage);

    turn_off_led_lights();

    delay(DELAY_250_MILLISECONDS);
  }

  // sensor maior que 50% e menor que 75% de umidade (valor hardcoded: 1750)
  else if (soil_humidity_percentage > 50 && soil_humidity_percentage < 75 && soil_humidity_percentage < plant_target_soil_humidity_percentage) {

    Serial.println("IF 50 - 75");

    turn_on_led_light_on_color("yellow");

    // show the humidity information on display
    display_soil_humidity_percentage();

    // method to activate the water pump
    water_plant_until_umidity_level(plant_target_soil_humidity_percentage);

    turn_off_led_lights();

    delay(DELAY_250_MILLISECONDS);
  }

  // sensor maior que 75% até 100% de umidade (valor hardcoded: 1)
  else if (soil_humidity_percentage > 75 && soil_humidity_percentage < plant_target_soil_humidity_percentage) {

    Serial.println("IF 75 - 100");

    turn_on_led_light_on_color("red");

    // show the humidity information on display
    display_soil_humidity_percentage();

    // method to activate the water pump
    water_plant_until_umidity_level(plant_target_soil_humidity_percentage);

    turn_off_led_lights();

    delay(DELAY_250_MILLISECONDS);
  }

  update_bot();

  // PASSO 3
  // finalizacao, cloud e fim do loop

  create_json_file_to_cloud();

  read_and_print_json_file(last_information_json);

  update_bot();

  turn_on_led_light_on_color("purple");
  ThingSpeak.writeField(myChannelNumber, 1, soil_humidity_percentage, myWriteAPIKey);
  turn_off_led_lights();

  plant_watered = false;

  check_wifi_connection();

  // turn_on_water_pump();
  // delay(DELAY_3_SECONDS);
  // turn_off_water_pump();
  update_bot();

  delay(DELAY_250_MILLISECONDS);

  display_soil_humidity_percentage();

  delay(DELAY_5_SECONDS);  // 300000
}
