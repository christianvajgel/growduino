#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H
#include <Arduino.h>

extern char* CONNECTED_SSID;
extern char* CONNECTED_PASSWORD;

// method to connect to wifi
void connect_to_wifi();

// method to get the External IPv4 from router
String get_external_ipv4();

// method to get the External IPv6 from router
String get_external_ipv6();

// method to check the Wi-Fi connection status
void check_wifi_connection();

#endif