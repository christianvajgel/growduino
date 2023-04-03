#include "esp32-hal.h"
#include "HardwareSerial.h"
#include "Water-Pump.h"
#include "Soil-Sensor.h"
#include "PIN-Namespace.h"
#include "Delay.h"
#include "OLED-Display.h"

// TODO: Trocar para 5 segundos ao final
const int PUMP_ACTIVATION_TIME = DELAY_2_SECONDS;  // in milliseconds (ms)

bool plant_watered = false;

// TODO: Verificar se sera necessario esta variavel
// bool to know if plant is being watered at the moment
bool watering_plant = false;

// TODO: trocar valores hardcoded para reais
void water_plant_until_umidity_level(int target_soil_humidity_percentage) {

  // adicionar leitura do solo aqui
  check_soil_umidity_percentage();

  while (target_soil_humidity_percentage >= soil_humidity_percentage) {

    // liga a bomba
    turn_on_water_pump();

    Serial.println("*********** water pump ON - TARGET: " + String(target_soil_humidity_percentage) + "   : : : : : : : : : :   ACTUAL: " + String(soil_humidity_percentage));

    // delay para molhar o solo
    delay(PUMP_ACTIVATION_TIME);

    // desliga bomba
    turn_off_water_pump();

    // ###############################
    // TODO: DESCOMENTAR O CODIGO ABAIXO QUANDO COLOCAR O SENSOR
    // delay para agua descer no vaso
    delay(DELAY_2_SECONDS);

    // realiza leitura novamente para verificar se ainda precisara regar
    check_soil_umidity_percentage();

    // atualiza o display OLED com a umidade atual
    display_soil_humidity_percentage();

    // TODO: colocar aqui o display do bitmap de irrigacao


    if (soil_humidity_percentage >= target_soil_humidity_percentage) {
      Serial.println("@@@@@@@ BREAK: water pump OFF - TARGET: " + String(target_soil_humidity_percentage) + "   : : : : : : : : : :   ACTUAL: " + String(soil_humidity_percentage));
      break;
    }

    Serial.println("########### water pump OFF - TARGET: " + String(target_soil_humidity_percentage) + "   : : : : : : : : : :   ACTUAL: " + String(soil_humidity_percentage));
  }

  // caso nao precise ira desligar a bomba
  delay(DELAY_3_SECONDS);
  turn_off_water_pump();
  Serial.println("METHOD OFF.....");
}

// ligar a bomba de agua
void turn_on_water_pump() {

  digitalWrite(WATER_PUMP_PIN, LOW);

  Serial.println("$$$$$$$$$$$ Water Pump ON: LOW sent");

  plant_watered = true;
  watering_plant = true;
}

// desligar a bomba de agua
void turn_off_water_pump() {

  digitalWrite(WATER_PUMP_PIN, HIGH);

  Serial.println("$$$$$$$$$$$ Water Pump OFF: HIGH sent");

  watering_plant = false;
}
