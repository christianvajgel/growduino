#ifndef HTTP_REQUESTS_H
#define HTTP_REQUESTS_H
#include <Arduino.h>

// GET to retrive Date and Time by IP
DynamicJsonDocument get_date_time_by_ip();

// GET to retrive Astronomical information by IP
DynamicJsonDocument get_astronomical_information_by_ip();

#endif