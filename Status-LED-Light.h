#ifndef STATUS_LED_LIGHT_H
#define STATUS_LED_LIGHT_H
#include <Arduino.h>

// method to turn on the LED light with a pre-determined color
void turn_on_led_light_on_color(String color);

// method to turn off the LED light
void turn_off_led_lights();

// method to test the LED light
void test_led_lights();

#endif