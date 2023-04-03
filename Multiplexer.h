#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H
#include <Arduino.h>
#include <Wire.h>

void setup_multiplexer();

void select_multiplexer_channel(uint8_t multiplexer_channel);

#endif