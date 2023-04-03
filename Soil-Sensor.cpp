#include "Soil-Sensor.h"
#include "PIN-Namespace.h"
#include "Plant.h"

const int MAXIMUM_SOIL_SENSOR_READ_VALUE = 4095;
const int MINIMUM_SOIL_SENSOR_READ_VALUE = 0;


int soil_humidity_sensor_read_value = 0;

int soil_humidity_percentage = 0;


void check_soil_umidity_percentage() {

  // lendo o sensor e retornando valor entre 0 a 4095
  read_soil_humidity_from_sensor();

  // mapeando o valor de forma percentual de 0 a 100%

  // ###############################
  // TODO: DESCOMENTAR O CODIGO ABAIXO QUANDO COLOCAR O SENSOR
  // soil_humidity_percentage = map(soil_humidity_sensor_read_value, MINIMUM_SOIL_SENSOR_READ_VALUE, MAXIMUM_SOIL_SENSOR_READ_VALUE, 100, 0);

  // ###############################
  // TODO: REMOVER OU COMENTAR O CODIGO ABAIXO QUANDO COLOCAR O SENSOR
  soil_humidity_percentage = soil_humidity_percentage + 5;

  // ###############################
  // TODO: REMOVER OU COMENTAR O CODIGO ABAIXO QUANDO COLOCAR O SENSOR
  // if (soil_humidity_percentage >= plant_target_soil_humidity_percentage) {
  //   soil_humidity_percentage = 0;
  // }
}


void read_soil_humidity_from_sensor() {

  // ###############################
  // TODO: DESCOMENTAR O CODIGO ABAIXO QUANDO COLOCAR O SENSOR
  //soil_humidity_sensor_read_value = analogRead(SOIL_SENSOR_PIN); // lê o valor do sensor de umidade


  // ###############################
  // TODO: REMOVER OU COMENTAR O CODIGO ABAIXO QUANDO COLOCAR O SENSOR

  int range = 4095 - 1 + 1;
  int num = rand() % range + 1;

  soil_humidity_sensor_read_value = num;

  // Serial.println("******* RANDOM int: " + String(num));
}
