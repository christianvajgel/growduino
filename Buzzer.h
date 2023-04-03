#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>

void play_tone_with_delay(int tone_hertz, int tone_duration, int delay_milliseconds);

void turn_off_buzzer();

#endif