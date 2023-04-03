

#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 oled;

void display_humidity(int humidityValue, int pos_X, int pos_Y);

void display_ais_animation();

void setup_oled_display();

void display_soil_humidity_percentage();

#endif
