#ifndef AIR_QUALITY_H
#define AIR_QUALITY_H
#include <Arduino.h>

extern String ambient_air_quality_indicator;

extern float co_ppm;
extern float co2_ppm;
extern float alcohol_ppm;
extern float toluen_ppm;
extern float nh4_ppm;
extern float aceton_ppm;

void setup_air_quality_sensor();

void check_air_quality_conditions();

#endif