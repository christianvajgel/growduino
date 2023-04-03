#include "HardwareSerial.h"
#include "Telegram.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "PIN-Namespace.h"
#include "JSON-Operations.h"
#include "Plant.h"
#include "Soil-Sensor.h"
#include "Delay.h"

// Documentacao -> https://core.telegram.org/bots
// API -> https://core.telegram.org/api
// listas de emojis para mensagens -> https://apps.timwhitlock.info/emoji/tables/unicode
// ultima lista de emoji unicode -> https://unicode.org/emoji/charts/full-emoji-list.html :: Precisa converter para UTF-8 em bytes depois de escolher
// Features -> https://core.telegram.org/bots/features

DynamicJsonDocument last_information_json_for_telegram(1024);

const char* TELEGRAM_BOT_TOKEN = "";
const char* TELEGRAM_CHAT_ID = "";

#define BOTtoken TELEGRAM_BOT_TOKEN
#define CHAT_ID TELEGRAM_CHAT_ID

int botRequestDelay = 1000;  // default 1000
unsigned long lastTimeBotRan;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

const int ledPin = BOARD_BLUE_LED_PIN;
bool ledState = LOW;

const int MINIMUM_TARGET_HUMIDITY = 0;
const int MAXIMUM_TARGET_HUMIDITY = 100;

// set a flag to keep track of whether we're waiting for a reply
bool waiting_for_reply_target_humidity = false;
// set a timer to keep track of the waiting time
unsigned long startTime = 0;

// set a flag to keep track of whether we're waiting for a reply
bool waiting_for_reply_language = false;
// set a timer to keep track of the waiting time
unsigned long startTime_language = 0;

String bot_language = "pt";


void update_bot() {

  ledState = HIGH;
  digitalWrite(ledPin, ledState);

  last_information_json_for_telegram = last_information_json;

  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    // Serial.println("########## New messages 1: " + String(numNewMessages));

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      // Serial.println("########## New messages 2: " + String(numNewMessages));
    }
    lastTimeBotRan = millis();
  }

  ledState = LOW;
  digitalWrite(ledPin, ledState);
}

void handleNewMessages(int numNewMessages) {

  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "\xE2\x9D\x8C Unauthorized user \xE2\x9D\x8C", "");
      continue;
    }

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;


    if (bot.messages[i].type == "callback_query") {

      // Serial.print("\n\nCall back button pressed by: ");
      // Serial.println(bot.messages[i].from_id);
      // Serial.print("Data on the button: ");
      // Serial.println(bot.messages[i].text);
      // bot.sendMessage(bot.messages[i].from_id, bot.messages[i].text, "\n\n");

      // if (text == "en") {
      //   bot_language = "en";
      //   bot.sendMessage(chat_id, bot_language, "Markdown");

      // } else if (text == "pt") {
      //   bot_language = "pt";
      //   bot.sendMessage(chat_id, bot_language, "Markdown");
      // }

      if (text == "en" || text == "pt") {

        bot_language = text == "en" ? "en" : "pt";

        String bot_message_language_configured = bot_language == "en" ? "System language: *English* \U0001F1FA\U0001F1F8" : "Idioma do sistema: *Português* \U0001F1E7\U0001F1F7";

        bot.sendMessage(chat_id, bot_message_language_configured, "Markdown");
      }
    }

    if (text == "/language" || text == "/idioma") {

      String bot_message_language = bot_language == "en" ? "Choose the system language: \nActual: *English* \U0001f1fa\U0001f1f8" : "Escolha o novo idioma do sistema: \nAtual: *Português* \U0001f1e7\U0001f1f7";

      String keyboardJson = "[[{ \"text\" : \"English \U0001F1FA\U0001F1F8\", \"callback_data\" : \"en\" }],[{ \"text\" : \"Português \U0001f1e7\U0001f1f7\", \"callback_data\" : \"pt\" }]]";

      bot.sendMessageWithInlineKeyboard(chat_id, bot_message_language, "Markdown", keyboardJson);
    }

    if (text == "/start@AIS_Arduino_bot" || text == "/start" || text == "/inicio@AIS_Arduino_bot" || text == "/inicio") {

      String welcome = "";

      if (bot_language == "pt") {

        welcome += "Bem-vindo, " + from_name + "! \xF0\x9F\x98\x8A \n\n";
        welcome += "Utilize os comandos abaixo para controlar as saídas.\n\n";
        welcome += "/alvo para alterar umidade alvo do solo \n";
        welcome += "/ambiente para obter as condições do ambiente \n";
        welcome += "/dia para obter informações do dia \n";
        welcome += "/gas para obter informações dos gases no ambiente \n";
        welcome += "/idioma para alterar o idioma do sistema \n";
        welcome += "/informacoes para obter informações de todo o sistema \n";
        welcome += "/planta para obter informações atuais da planta \n\n";
        welcome += "/inicio para mostrar o menu inicial \n";


      } else if (bot_language == "en") {

        welcome += "Welcome, " + from_name + "! \xF0\x9F\x98\x8A \n\n";
        welcome += "Use the following commands to control your outputs.\n\n";
        welcome += "/ambient to get the conditions of the ambient \n";
        welcome += "/day to get information about the day \n";
        welcome += "/gases to get information about the gases in the ambient \n";
        welcome += "/information to get information about the whole system \n";
        welcome += "/language to change the language of the system \n";
        welcome += "/plant to get actual information of the plant \n";
        welcome += "/target to change the target humidity of the soil \n\n";
        welcome += "/start to show the start menu \n";
      }
      bot.sendMessage(chat_id, welcome, "Markdown");
    }

    if (text == "/status@AIS_Arduino_bot" || text == "/status") {

      String bot_message_led_status = bot_language == "en" ? "LED state set to " : "Estado do LED configurado para ";

      if (digitalRead(ledPin)) {
        bot_message_led_status += bot_language == "en" ? "ON \xF0\x9F\x94\xB5" : "LIGADO \xF0\x9F\x94\xB5";
      } else {
        bot_message_led_status += bot_language == "en" ? "OFF \xE2\x9A\xAA" : "DESLIGADO \xE2\x9A\xAA";
      }

      bot.sendMessage(chat_id, bot_message_led_status, "Markdown");
    }

    if (text == "/ambiente@AIS_Arduino_bot" || text == "/ambiente" || text == "/ambient@AIS_Arduino_bot" || text == "/ambient") {

      String local_time_json = last_information_json["local_datetime"].as<String>();
      String hour_clock_emoji = get_hour_clock_emoji(local_time_json);
      String date_human_readable = get_date_human_readable(local_time_json);
      String time_human_readable = get_time_human_readable(local_time_json);

      String ambient = "";

      if (bot_language == "pt") {

        String air_quality_translation = get_air_quality_in_portuguese();

        ambient += "\xF0\x9F\x8F\xA1 *Informações do ambiente* \xF0\x9F\x8F\xA1 \n\n";

        ambient += hour_clock_emoji + " Hora: " + time_human_readable + "\n";
        ambient += "\xF0\x9F\x93\x86 Data: " + date_human_readable + "\n";

        ambient += "\n *Ambiente:*\n";
        ambient += "\xF0\x9F\x8C\xA1 Temperatura: " + String(last_information_json["ambient_temperature"].as<int>()) + " ºC \n";
        ambient += "\xF0\x9F\x92\xA8 Umidade do ar: " + String(last_information_json["ambient_air_humidity"].as<float>(), 2) + " % \n";
        ambient += "\xF0\x9F\x9A\xa6 Qualidade do ar: " + air_quality_translation + "\n";
        ambient += "\xF0\x9F\x92\xA2 Pressão atmosférica: " + String(last_information_json["ambient_atmospheric_pressure"].as<int>()) + " kPa \n";
        ambient += "\xF0\x9F\x8F\x94 Altitude: " + String(last_information_json["ambient_altitude"].as<float>(), 2) + " m \n";

      } else if (bot_language == "en") {

        ambient += "\xF0\x9F\x8F\xA1 *Ambient information* \xF0\x9F\x8F\xA1 \n\n";

        ambient += hour_clock_emoji + " Time: " + time_human_readable + "\n";
        ambient += "\xF0\x9F\x93\x86 Date: " + date_human_readable + "\n";

        ambient += "\n *Ambient:*\n";
        ambient += "\xF0\x9F\x8C\xA1 Temperature: " + String(last_information_json["ambient_temperature"].as<int>()) + " ºC \n";
        ambient += "\xF0\x9F\x92\xA8 Air humidity: " + String(last_information_json["ambient_air_humidity"].as<float>(), 2) + " % \n";
        ambient += "\xF0\x9F\x9A\xa6 Air quality: " + last_information_json["ambient_air_quality_indicator"].as<String>() + "\n";
        ambient += "\xF0\x9F\x92\xA2 Atmospheric pressure: " + String(last_information_json["ambient_atmospheric_pressure"].as<int>()) + " kPa \n";
        ambient += "\xF0\x9F\x8F\x94 Altitude: " + String(last_information_json["ambient_altitude"].as<float>(), 2) + " m \n";
      }

      Serial.println("!!!!!!!!!!!!!!!!!! SENDING MSG: ");
      bot.sendMessage(chat_id, ambient, "Markdown");
      Serial.println("!!!!!!!!!!!!!!!!!! SENT MSG: ");
      Serial.println(ambient);
    }

    if (text == "/dia@AIS_Arduino_bot" || text == "/dia" || text == "/day@AIS_Arduino_bot" || text == "/day") {

      String local_time_json = last_information_json["local_datetime"].as<String>();
      String hour_clock_emoji = get_hour_clock_emoji(local_time_json);
      String date_human_readable = get_date_human_readable(local_time_json);
      String time_human_readable = get_time_human_readable(local_time_json);

      String day = "";

      if (bot_language == "pt") {

        day += "\xF0\x9F\x98\x8E *Informações do dia* \xF0\x9F\x98\x8E \n\n";

        day += hour_clock_emoji + " Hora: " + time_human_readable + "\n";
        day += "\xF0\x9F\x93\x86 Data: " + date_human_readable + "\n";

        day += "\n *Dia:* \n";
        day += "\xF0\x9F\x8C\x85 Nascer do sol: " + last_information_json["sunrise_hour"].as<String>() + "\n";
        day += "\xF0\x9F\x8C\x87 Pôr do sol: " + last_information_json["sunset_hour"].as<String>() + "\n";
        day += "\xE2\x98\x80 Meio-dia solar: " + last_information_json["solar_noon_hour"].as<String>() + "\n";
        day += "\xF0\x9F\x8C\xBB Duração do dia: " + last_information_json["day_length_duration"].as<String>() + "\n";

      } else if (bot_language == "en") {

        day += "\xF0\x9F\x98\x8E *Day information* \xF0\x9F\x98\x8E \n\n";

        day += hour_clock_emoji + " Time: " + time_human_readable + "\n";
        day += "\xF0\x9F\x93\x86 Date: " + date_human_readable + "\n";

        day += "\n *Day:* \n";
        day += "\xF0\x9F\x8C\x85 Sunrise: " + last_information_json["sunrise_hour"].as<String>() + "\n";
        day += "\xF0\x9F\x8C\x87 Sunset: " + last_information_json["sunset_hour"].as<String>() + "\n";
        day += "\xE2\x98\x80 Solar noon: " + last_information_json["solar_noon_hour"].as<String>() + "\n";
        day += "\xF0\x9F\x8C\xBB Day length duration: " + last_information_json["day_length_duration"].as<String>() + "\n";
      }

      Serial.println("!!!!!!!!!!!!!!!!!! SENDING MSG: ");
      bot.sendMessage(chat_id, day, "Markdown");
      Serial.println("!!!!!!!!!!!!!!!!!! SENT MSG: ");
      Serial.println(day);
    }

    if (text == "/plant@AIS_Arduino_bot" || text == "/plant" || text == "/planta@AIS_Arduino_bot" || text == "/planta") {

      String local_time_json = last_information_json["local_datetime"].as<String>();
      String hour_clock_emoji = get_hour_clock_emoji(local_time_json);
      String date_human_readable = get_date_human_readable(local_time_json);
      String time_human_readable = get_time_human_readable(local_time_json);

      String plant_status = "";

      if (bot_language == "pt") {

        String watered_plant = last_information_json["plant_watered"].as<bool>() == true ? "Sim" : "Não";

        plant_status += "\xF0\x9F\x8C\xB1 *Informações da planta* \xF0\x9F\x8C\xB1 \n\n";

        plant_status += "\xF0\x9F\xAA\xB4 Planta: " + plant_name + "\n";

        plant_status += hour_clock_emoji + " Hora: " + time_human_readable + "\n";
        plant_status += "\xF0\x9F\x93\x86 Data: " + date_human_readable + "\n";

        plant_status += "\n *Solo:* \n";
        plant_status += "\xF0\x9F\x92\xA6 Umidade: " + String(soil_humidity_percentage) + " % \n";
        plant_status += "\xF0\x9F\x8E\xAF Umidade alvo: " + String(plant_target_soil_humidity_percentage) + " % \n";
        plant_status += "\xE2\x9B\xb2 Planta regada: " + watered_plant + " \n";

      } else if (bot_language == "en") {

        String watered_plant = last_information_json["plant_watered"].as<bool>() == true ? "Yes" : "No";

        plant_status += "\xF0\x9F\x8C\xB1 *Plant information* \xF0\x9F\x8C\xB1 \n\n";

        plant_status += "\xF0\x9F\xAA\xB4 Plant: " + plant_name + "\n";

        plant_status += hour_clock_emoji + " Time: " + time_human_readable + "\n";
        plant_status += "\xF0\x9F\x93\x86 Date: " + date_human_readable + "\n";

        plant_status += "\n *Soil:* \n";
        plant_status += "\xF0\x9F\x92\xA6 Humidity: " + String(soil_humidity_percentage) + " % \n";
        plant_status += "\xF0\x9F\x8E\xAF Target humidity: " + String(plant_target_soil_humidity_percentage) + " % \n";
        plant_status += "\xE2\x9B\xb2 Plant watered: " + watered_plant + " \n";
      }

      Serial.println("!!!!!!!!!!!!!!!!!! SENDING MSG: ");
      bot.sendMessage(chat_id, plant_status, "Markdown");
      Serial.println("!!!!!!!!!!!!!!!!!! SENT MSG: ");
      Serial.println(plant_status);
    }

    if (text == "/gases@AIS_Arduino_bot" || text == "/gases" || text == "/gas@AIS_Arduino_bot" || text == "/gas") {

      String local_time_json = last_information_json["local_datetime"].as<String>();
      String hour_clock_emoji = get_hour_clock_emoji(local_time_json);
      String date_human_readable = get_date_human_readable(local_time_json);
      String time_human_readable = get_time_human_readable(local_time_json);

      String gases = "";

      if (bot_language == "pt") {

        gases += "\xF0\x9F\x8F\xAD *Informações de gases* \xF0\x9F\x8F\xAD \n\n";

        gases += hour_clock_emoji + " Hora: " + time_human_readable + "\n";
        gases += "\xF0\x9F\x93\x86 Data: " + date_human_readable + "\n";

        gases += "\n *Gases:*\n";
        gases += "\xF0\x9F\x9A\x97 Monóxido de Carbono (CO): " + String(last_information_json["co_ppm"].as<float>(), 2) + " ppm \n";
        gases += "\xF0\x9F\xA7\xAF Dióxido de Carbono (CO2): " + String(last_information_json["co2_ppm"].as<float>(), 2) + " ppm \n";
        gases += "\xF0\x9F\xaa\xa3 Tolueno (C6H5CH3): " + String(last_information_json["toluen_ppm"].as<float>(), 2) + " ppm \n";
        gases += "\xF0\x9F\x92\xA9 Amônia (NH4): " + String(last_information_json["nh4_ppm"].as<float>(), 2) + " ppm \n";
        gases += "\xF0\x9F\x92\x85 Acetona (C3H60): " + String(last_information_json["aceton_ppm"].as<float>(), 2) + " ppm \n";
        gases += "\xF0\x9F\x8D\xBE Álcool (C2H60): " + String(last_information_json["alcohol_ppm"].as<float>(), 2) + " ppm \n";

      } else if (bot_language == "en") {

        gases += "\xF0\x9F\x8F\xAD *Gases information* \xF0\x9F\x8F\xAD \n\n";

        gases += hour_clock_emoji + " Time: " + time_human_readable + "\n";
        gases += "\xF0\x9F\x93\x86 Date: " + date_human_readable + "\n";

        gases += "\n *Gases:*\n";
        gases += "\xF0\x9F\x9A\x97 Carbon Monoxide (CO): " + String(last_information_json["co_ppm"].as<float>(), 2) + " ppm \n";
        gases += "\xF0\x9F\xA7\xAF Carbon Dioxide (CO2): " + String(last_information_json["co2_ppm"].as<float>(), 2) + " ppm \n";
        gases += "\xF0\x9F\xaa\xa3 Toluen (C6H5CH3): " + String(last_information_json["toluen_ppm"].as<float>(), 2) + " ppm \n";
        gases += "\xF0\x9F\x92\xA9 Ammonia (NH4): " + String(last_information_json["nh4_ppm"].as<float>(), 2) + " ppm \n";
        gases += "\xF0\x9F\x92\x85 Aceton (C3H60): " + String(last_information_json["aceton_ppm"].as<float>(), 2) + " ppm \n";
        gases += "\xF0\x9F\x8D\xBE Alcohol (C2H60): " + String(last_information_json["alcohol_ppm"].as<float>(), 2) + " ppm \n";
      }

      Serial.println("!!!!!!!!!!!!!!!!!! SENDING MSG: ");
      bot.sendMessage(chat_id, gases, "Markdown");
      Serial.println("!!!!!!!!!!!!!!!!!! SENT MSG: ");
      Serial.println(gases);
    }

    if (text == "/informacoes@AIS_Arduino_bot" || text == "/informacoes" || text == "/information@AIS_Arduino_bot" || text == "/information") {

      String local_time_json = last_information_json["local_datetime"].as<String>();
      String hour_clock_emoji = get_hour_clock_emoji(local_time_json);
      String date_human_readable = get_date_human_readable(local_time_json);
      String time_human_readable = get_time_human_readable(local_time_json);

      String information = "";

      if (bot_language == "pt") {

        String air_quality_translation = get_air_quality_in_portuguese();

        String watered_plant = last_information_json["plant_watered"].as<bool>() == true ? "Sim" : "Não";

        information += "\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x92\xBB *Informações do sistema* \xF0\x9F\x91\xA9\xE2\x80\x8D\xF0\x9F\x92\xBB \n\n";

        information += "\xF0\x9F\xAA\xB4 Planta: " + plant_name + "\n";

        information += hour_clock_emoji + " Hora: " + time_human_readable + "\n";
        information += "\xF0\x9F\x93\x86 Data: " + date_human_readable + "\n";

        information += "\n *Dia:* \n";
        information += "\xF0\x9F\x8C\x85 Nascer do sol: " + last_information_json["sunrise_hour"].as<String>() + "\n";
        information += "\xF0\x9F\x8C\x87 Pôr do sol: " + last_information_json["sunset_hour"].as<String>() + "\n";
        information += "\xE2\x98\x80 Meio-dia solar: " + last_information_json["solar_noon_hour"].as<String>() + "\n";
        information += "\xF0\x9F\x8C\xBB Duração do dia: " + last_information_json["day_length_duration"].as<String>() + "\n";

        information += "\n *Solo:* \n";
        information += "\xF0\x9F\x92\xA6 Umidade: " + String(soil_humidity_percentage) + " % \n";
        information += "\xF0\x9F\x8E\xAF Umidade alvo: " + String(plant_target_soil_humidity_percentage) + " % \n";
        information += "\xE2\x9B\xb2 Planta regada: " + watered_plant + " \n";

        information += "\n *Ambiente:*\n";
        information += "\xF0\x9F\x8C\xA1 Temperatura: " + String(last_information_json["ambient_temperature"].as<int>()) + " ºC \n";
        information += "\xF0\x9F\x92\xA8 Umidade do ar: " + String(last_information_json["ambient_air_humidity"].as<float>(), 2) + " % \n";
        information += "\xF0\x9F\x9A\xa6 Qualidade do ar: " + air_quality_translation + "\n";
        information += "\xF0\x9F\x92\xA2 Pressão atmosférica: " + String(last_information_json["ambient_atmospheric_pressure"].as<int>()) + " kPa \n";
        information += "\xF0\x9F\x8F\x94 Altitude: " + String(last_information_json["ambient_altitude"].as<float>(), 2) + " m \n";

        information += "\n *Gases:*\n";
        information += "\xF0\x9F\x9A\x97 Monóxido de Carbono (CO): " + String(last_information_json["co_ppm"].as<float>(), 2) + " ppm \n";
        information += "\xF0\x9F\xA7\xAF Dióxido de Carbono (CO2): " + String(last_information_json["co2_ppm"].as<float>(), 2) + " ppm \n";
        information += "\xF0\x9F\xaa\xa3 Tolueno (C6H5CH3): " + String(last_information_json["toluen_ppm"].as<float>(), 2) + " ppm \n";
        information += "\xF0\x9F\x92\xA9 Amônia (NH4): " + String(last_information_json["nh4_ppm"].as<float>(), 2) + " ppm \n";
        information += "\xF0\x9F\x92\x85 Acetona (C3H60): " + String(last_information_json["aceton_ppm"].as<float>(), 2) + " ppm \n";
        information += "\xF0\x9F\x8D\xBE Álcool (C2H60): " + String(last_information_json["alcohol_ppm"].as<float>(), 2) + " ppm \n";

      } else if (bot_language == "en") {

        String watered_plant = last_information_json["plant_watered"].as<bool>() == true ? "Yes" : "No";

        information += "\xF0\x9F\x8C\xB1 *System information* \xF0\x9F\x8C\xB1 \n\n";

        information += "\xF0\x9F\xAA\xB4 Plant: " + plant_name + "\n";

        information += hour_clock_emoji + " Time: " + time_human_readable + "\n";
        information += "\xF0\x9F\x93\x86 Date: " + date_human_readable + "\n";

        information += "\n *Day:* \n";
        information += "\xF0\x9F\x8C\x85 Sunrise: " + last_information_json["sunrise_hour"].as<String>() + "\n";
        information += "\xF0\x9F\x8C\x87 Sunset: " + last_information_json["sunset_hour"].as<String>() + "\n";
        information += "\xE2\x98\x80 Solar noon: " + last_information_json["solar_noon_hour"].as<String>() + "\n";
        information += "\xF0\x9F\x8C\xBB Day length duration: " + last_information_json["day_length_duration"].as<String>() + "\n";

        information += "\n *Soil:* \n";
        information += "\xF0\x9F\x92\xA6 Humidity: " + String(soil_humidity_percentage) + " % \n";
        information += "\xF0\x9F\x8E\xAF Target humidity: " + String(plant_target_soil_humidity_percentage) + " % \n";
        information += "\xE2\x9B\xb2 Plant watered: " + watered_plant + " \n";

        information += "\n *Ambient:*\n";
        information += "\xF0\x9F\x8C\xA1 Temperature: " + String(last_information_json["ambient_temperature"].as<int>()) + " ºC \n";
        information += "\xF0\x9F\x92\xA8 Air humidity: " + String(last_information_json["ambient_air_humidity"].as<float>(), 2) + " % \n";
        information += "\xF0\x9F\x9A\xa6 Air quality: " + last_information_json["ambient_air_quality_indicator"].as<String>() + "\n";
        information += "\xF0\x9F\x92\xA2 Atmospheric pressure: " + String(last_information_json["ambient_atmospheric_pressure"].as<int>()) + " kPa \n";
        information += "\xF0\x9F\x8F\x94 Altitude: " + String(last_information_json["ambient_altitude"].as<float>(), 2) + " m \n";

        information += "\n *Gases:*\n";
        information += "\xF0\x9F\x9A\x97 Carbon Monoxide (CO): " + String(last_information_json["co_ppm"].as<float>(), 2) + " ppm \n";
        information += "\xF0\x9F\xA7\xAF Carbon Dioxide (CO2): " + String(last_information_json["co2_ppm"].as<float>(), 2) + " ppm \n";
        information += "\xF0\x9F\xaa\xa3 Toluen (C6H5CH3): " + String(last_information_json["toluen_ppm"].as<float>(), 2) + " ppm \n";
        information += "\xF0\x9F\x92\xA9 Ammonia (NH4): " + String(last_information_json["nh4_ppm"].as<float>(), 2) + " ppm \n";
        information += "\xF0\x9F\x92\x85 Aceton (C3H60): " + String(last_information_json["aceton_ppm"].as<float>(), 2) + " ppm \n";
        information += "\xF0\x9F\x8D\xBE Alcohol (C2H60): " + String(last_information_json["alcohol_ppm"].as<float>(), 2) + " ppm \n";
      }

      Serial.println("!!!!!!!!!!!!!!!!!! SENDING MSG: ");
      bot.sendMessage(chat_id, information, "Markdown");
      Serial.println("!!!!!!!!!!!!!!!!!! SENT MSG: ");
      Serial.println(information);
    }

    if (text == "/target@AIS_Arduino_bot" || text == "/target" || text == "/alvo@AIS_Arduino_bot" || text == "/alvo") {
      // set the flag to true and start the timer
      waiting_for_reply_target_humidity = true;
      startTime = millis();

      String bot_message_target_humidity = bot_language == "en" ? "Please, reply this message entering the new target humidity for the plant (in percentage):" : "Por favor, responda esta mensagem entrando com o valor percentual da umidade alvo da planta:";

      bot.sendMessage(chat_id, bot_message_target_humidity, "Markdown");

      // wait for the user's reply or timeout after 60 seconds
      while (waiting_for_reply_target_humidity && millis() - startTime < 60000) {
        // check for new messages from the Telegram server
        int numUpdates = bot.getUpdates(bot.last_message_received + 1);

        if (numUpdates > 0) {
          // loop over all received messages
          for (int i = 0; i < numUpdates; i++) {
            // get the latest message
            String messageText = bot.messages[i].text;
            String messageChatId = String(bot.messages[i].chat_id);

            // check if the message is from the user and is a text message
            if (String(messageChatId) == String(chat_id) && messageText.length() > 0) {
              // get the user's reply and set the new target humidity
              int newTargetHumidity;
              bool isNumber = true;
              for (int j = 0; j < messageText.length(); j++) {
                if (!isdigit(messageText.charAt(j))) {
                  isNumber = false;
                  break;
                }
              }
              if (isNumber) {
                newTargetHumidity = messageText.toInt();
                if (newTargetHumidity >= 0 && newTargetHumidity <= 100) {
                  plant_target_soil_humidity_percentage = newTargetHumidity;

                  String bot_message_confirmation_target_humidity = bot_language == "en" ? "The new target humidity has been set to " : "O novo valor para umidade alvo foi configurado para ";


                  // send a confirmation message to the user
                  bot.sendMessage(chat_id, bot_message_confirmation_target_humidity + String(plant_target_soil_humidity_percentage) + " %", "Markdown");

                  // flash the LED to signal that the new target humidity has been set
                  for (int i = 0; i < 3; i++) {
                    ledState = HIGH;
                    digitalWrite(ledPin, ledState);
                    delay(DELAY_100_MILLISECONDS);
                    ledState = LOW;
                    digitalWrite(ledPin, ledState);
                    delay(DELAY_100_MILLISECONDS);
                  }

                  // set the flag to false
                  waiting_for_reply_target_humidity = false;

                } else {

                  // send a message to the user that input was out of the range (0-100)
                  String bot_message_input_value_error_number = bot_language == "en" ? "The input value must be between 0 and 100. Please try again." : "O valor de entrada deve ser entre 0 a 100. Por favor, tente novamente.";

                  bot.sendMessage(chat_id, bot_message_input_value_error_number, "Markdown");
                }
              } else {

                // send a message to the user that value entered was different from a number
                String bot_message_input_value_error = bot_language == "en" ? "The input value must be a valid number (0 to 100). Please try again." : "O valor de entrada deve ser um número válido (0 a 100). Por favor, tente novamente.";

                bot.sendMessage(chat_id, bot_message_input_value_error, "Markdown");
              }
            }
          }
        }
        delay(DELAY_250_MILLISECONDS);
      }

      // check if we received a reply or timed out
      if (waiting_for_reply_target_humidity) {

        // send a message to the user that nothing was done due to no answer
        String bot_message_no_reply_from_user = bot_language == "en" ? "Timeout: No reply was received within 60 seconds. Please try again." : "Tempo esgotado: Nenhuma resposta foi recebida durante de 60 segundos. Tente novamente.";

        bot.sendMessage(chat_id, bot_message_no_reply_from_user, "Markdown");
      }
    }

    if (text == "/soil0@AIS_Arduino_bot" || text == "/soil0") {
      bot.sendMessage(chat_id, "Soil humidity of the plant is now 10%", "Markdown");
      soil_humidity_percentage = 0;

      for (int i = 0; i < 3; i++) {
        ledState = HIGH;
        digitalWrite(ledPin, ledState);
        delay(DELAY_250_MILLISECONDS);
        ledState = LOW;
        digitalWrite(ledPin, ledState);
        delay(DELAY_250_MILLISECONDS);
      }
    }
  }
}

String get_hour_clock_emoji(String date) {

  // Extract the hour from the date string
  String hour = date.substring(11, 13);

  // Extract the minute from the date string
  int minute = date.substring(14, 16).toInt();

  // Create a message based on the hour
  String clock_emoji;

  if (hour == "00" || hour == "12") {

    clock_emoji = "\xF0\x9F\x95\x9B";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\xA7";
    }

  } else if (hour == "01" || hour == "13") {

    clock_emoji = "\xF0\x9F\x95\x90";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\x9C";
    }

  } else if (hour == "02" || hour == "14") {

    clock_emoji = "\xF0\x9F\x95\x91";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\x9D";
    }

  } else if (hour == "03" || hour == "15") {

    clock_emoji = "\xF0\x9F\x95\x92";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\x9E";
    }

  } else if (hour == "04" || hour == "16") {

    clock_emoji = "\xF0\x9F\x95\x93";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\x9F";
    }

  } else if (hour == "05" || hour == "17") {

    clock_emoji = "\xF0\x9F\x95\x94";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\xA0";
    }

  } else if (hour == "06" || hour == "18") {

    clock_emoji = "\xF0\x9F\x95\x95";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\xA1";
    }

  } else if (hour == "07" || hour == "19") {

    clock_emoji = "\xF0\x9F\x95\x96";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\xA2";
    }

  } else if (hour == "08" || hour == "20") {

    clock_emoji = "\xF0\x9F\x95\x97";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\xA3";
    }

  } else if (hour == "09" || hour == "21") {

    clock_emoji = "\xF0\x9F\x95\x98";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\xA4";
    }

  } else if (hour == "10" || hour == "22") {

    clock_emoji = "\xF0\x9F\x95\x99";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\xA5";
    }

  } else if (hour == "11" || hour == "23") {

    clock_emoji = "\xF0\x9F\x95\x9A";

    if (minute >= 30) {
      clock_emoji = "\xF0\x9F\x95\xA6";
    }

  } else {

    clock_emoji = "\xE2\x8F\xB0";
  }

  // Return the message
  return clock_emoji;
}

String get_day_period_greeting(String date) {

  //"2023-03-19T10:51:26.529145-03:00"

  // Extract the hour from the date string
  int hour = date.substring(11, 13).toInt();

  String greeting = "";

  if (bot_language == "en") {
    if (hour >= 0 && hour <= 12) {

      greeting = "Good morning";

    } else if (hour >= 13 && hour <= 19) {

      greeting = "Good afternoon";

    } else if (hour >= 20 && hour <= 23) {

      greeting = "Good evening";

    } else {

      greeting = "Hello";
    }
  }

  else if (bot_language == "pt") {
    if (hour >= 0 && hour <= 12) {

      greeting = "Bom dia";

    } else if (hour >= 13 && hour <= 19) {

      greeting = "Boa tarde";

    } else if (hour >= 20 && hour <= 23) {

      greeting = "Boa noite";

    } else {

      greeting = "Olá";
    }
  }

  // Return the message
  return greeting;
}

String get_day_period_greeting_emoji(String date) {

  //"2023-03-19T10:51:26.529145-03:00"

  // Extract the hour from the date string
  int hour = date.substring(11, 13).toInt();

  String day_period_emoji = "";

  if (hour >= 0 && hour <= 2) {

    day_period_emoji = "\xF0\x9F\x8C\xA0";

  }

  else if (hour == 3 || hour == 4) {

    day_period_emoji = "\xF0\x9F\x8C\x8C";

  }

  else if (hour >= 5 && hour <= 7) {

    day_period_emoji = "\xF0\x9F\x8C\x85";

  }

  else if (hour >= 8 && hour <= 15) {

    day_period_emoji = "\xF0\x9F\x8F\x99\xEF\xB8\x8F";

  } else if (hour == 16 || hour == 17) {

    day_period_emoji = "\xF0\x9F\x8C\x87";

  } else if (hour == 18 || hour == 19) {

    day_period_emoji = "\xF0\x9F\x8C\x86";

  } else if (hour >= 20 && hour <= 23) {

    day_period_emoji = "\xF0\x9F\x8C\x83";

  } else {

    day_period_emoji = "\xF0\x9F\x8C\x81";
  }

  // Return the message
  return day_period_emoji;
}

String get_date_human_readable(String date) {

  String human_readable_date = "";

  // Extract the items from the date string
  String day = date.substring(8, 10);
  String month = date.substring(5, 7);
  String year = date.substring(0, 4);

  if (bot_language == "en") {
    // Format the date as MM/DD/YYYY for English
    human_readable_date = month + "/" + day + "/" + year;
  }

  else if (bot_language == "pt") {
    // Format the date as DD/MM/YYYY for Portuguese
    human_readable_date = day + "/" + month + "/" + year;
  }

  // Return the formatted date
  return human_readable_date;
}

String get_time_human_readable(String time) {

  String human_readable_time = "";

  // Extract the items from the time string
  String hour = time.substring(11, 13);
  String minute = time.substring(14, 16);

  if (bot_language == "en") {
    // Format the time as hh:mm:ss AM/PM for English

    switch (hour.toInt()) {
      case 13:
        human_readable_time = "01:" + minute + " PM";
        break;
      case 14:
        human_readable_time = "02:" + minute + " PM";
        break;
      case 15:
        human_readable_time = "03:" + minute + " PM";
        break;
      case 16:
        human_readable_time = "04:" + minute + " PM";
        break;
      case 17:
        human_readable_time = "05:" + minute + " PM";
        break;
      case 18:
        human_readable_time = "06:" + minute + " PM";
        break;
      case 19:
        human_readable_time = "07:" + minute + " PM";
        break;
      case 20:
        human_readable_time = "08:" + minute + " PM";
        break;
      case 21:
        human_readable_time = "09:" + minute + " PM";
        break;
      case 22:
        human_readable_time = "10:" + minute + " PM";
        break;
      case 23:
        human_readable_time = "11:" + minute + " PM";
        break;
      default:
        // Handle cases where hour is not between 13 and 23 (inclusive)
        human_readable_time = hour + ":" + minute;
        break;
    }
  }

  else if (bot_language == "pt") {
    // Format the time as hh:mm:ss for Portuguese
    human_readable_time = hour + ":" + minute;
  }

  // Return the formatted time
  return human_readable_time;
}

String get_air_quality_in_portuguese() {

  String english_word = last_information_json["ambient_air_quality_indicator"].as<String>();

  String translation = "";

  if (english_word == "Excellent") {

    translation = "Excelente";

  } else if (english_word == "Moderate") {

    translation = "Moderada";

  } else if (english_word == "Bad") {

    translation = "Ruim";

  } else if (english_word == "Very bad") {

    translation = "Muito ruim";

  } else {

    translation = "Perigo";
  }

  return translation;
}
