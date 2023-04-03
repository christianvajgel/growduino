#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "PIN-Namespace.h"

extern UniversalTelegramBot bot;

extern const char* TELEGRAM_BOT_TOKEN;
extern const char* TELEGRAM_CHAT_ID;

extern unsigned long lastTimeBotRan;
extern int botRequestDelay;
extern WiFiClientSecure client;

extern const int ledPin;
extern bool ledState;

void update_bot();

void handleNewMessages(int numNewMessages);

String get_hour_clock_emoji(String date);

String get_day_period_greeting(String date);

String get_day_period_greeting_emoji(String date);

String get_date_human_readable(String date);

String get_time_human_readable(String time);

String get_air_quality_in_portuguese();

#endif