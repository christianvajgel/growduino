#ifndef JSON_OPERATIONS_H
#define JSON_OPERATIONS_H
#include <ArduinoJson.h>
#include <Arduino.h>
#include <list>

extern DynamicJsonDocument last_information_json;

// method to create the JSON file to send to the cloud
DynamicJsonDocument create_json_file_to_cloud();

// method to merge two JSONs into one
DynamicJsonDocument mergeJson(DynamicJsonDocument json1, DynamicJsonDocument json2);

// method to print a JSON file on Serial
void read_and_print_json_file(DynamicJsonDocument json_file_to_print);

#endif