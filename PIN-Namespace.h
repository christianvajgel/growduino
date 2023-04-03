#ifndef PIN_NAMESPACE_H
#define PIN_NAMESPACE_H
#include <Arduino.h>

// Soil Sensor Pin
const int SOIL_SENSOR_PIN = A0;

// Water Pump Pin
const int WATER_PUMP_PIN = 33;  // A5 - Setup a PIN with "D" letter to send HIGH (Off) and LOW (On) to the Low-level trigger

// RGB Module Pins
const int RED_LED_PIN = 18;    // 13 -> 18
const int GREEN_LED_PIN = 19;  // 12 -> 5
const int BLUE_LED_PIN = 5;    // 14

// Blue LED Board Pin
const int BOARD_BLUE_LED_PIN = 2;

// Gas Sensor Analogic Pin
const int GAS_SENSOR_ANALOGIC_PIN = 35;

const int BUZZER_DAC_PIN = 25;

#endif