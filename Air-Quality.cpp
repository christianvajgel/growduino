#include "esp32-hal.h"
#include "Air-Quality.h"
#include <MQUnifiedsensor.h>
#include "PIN-Namespace.h"
#include "Delay.h"

//Definitions
#define placa "ESP-WROOM-32"
#define Voltage_Resolution_VIN 5
#define SENSOR_PIN GAS_SENSOR_ANALOGIC_PIN  //Analog input 0 of your arduino
#define type "MQ-135"                       //MQ135
#define ADC_Bit_Resolution 10               // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6              //RS / R0 = 3.6 ppm
//#define calibration_button 13 //Pin to calibrate your sensor

//Declare Sensor
MQUnifiedsensor MQ135(placa, Voltage_Resolution_VIN, ADC_Bit_Resolution, SENSOR_PIN, type);

String ambient_air_quality_indicator;
float co_ppm;
float co2_ppm;
float alcohol_ppm;
float toluen_ppm;
float nh4_ppm;
float aceton_ppm;

void setup_air_quality_sensor() {

  //Set math model to calculate the PPM concentration and the value of constants
  MQ135.setRegressionMethod(1);

  MQ135.init();

  Serial.print("Calibrating Air Quality Sensor. Please wait.");

  float calcR0 = 0;

  for (int i = 1; i <= 10; i++) {

    MQ135.update();  // Update data, the arduino will read the voltage from the analog SENSOR_PIN
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }

  MQ135.setR0(calcR0 / 10);
  Serial.println("Calibration complete with success!");

  if (isinf(calcR0)) {

    Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
    while (1)
      ;
  }

  if (calcR0 == 0) {

    Serial.println("Warning: Conection issue found, R0 is zero (Analog SENSOR_PIN shorts to ground) please check your wiring and supply");
    while (1)
      ;
  }

  /*****************************  MQ CAlibration ********************************************/
  Serial.println("** Values from MQ-135 ****");
  Serial.println("|    CO   |  Alcohol |   CO2  |  Toluen  |  NH4  |  Aceton  |");
}

void check_air_quality_conditions() {

  MQ135.update();  // Update data, the arduino will read the voltage from the analog SENSOR_PIN
  delay(DELAY_100_MILLISECONDS);

  MQ135.setA(605.18);
  MQ135.setB(-3.937);           // Configure the equation to calculate CO concentration value
  co_ppm = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  delay(DELAY_100_MILLISECONDS);

  MQ135.setA(77.255);
  MQ135.setB(-3.18);                 //Configure the equation to calculate Alcohol concentration value
  alcohol_ppm = MQ135.readSensor();  // SSensor will read PPM concentration using the model, a and b values set previously or from the setup
  delay(DELAY_100_MILLISECONDS);

  MQ135.setA(110.47);
  MQ135.setB(-2.862);            // Configure the equation to calculate CO2 concentration value
  co2_ppm = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  delay(DELAY_100_MILLISECONDS);

  MQ135.setA(44.947);
  MQ135.setB(-3.445);               // Configure the equation to calculate Toluen concentration value
  toluen_ppm = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  delay(DELAY_100_MILLISECONDS);

  MQ135.setA(102.2);
  MQ135.setB(-2.473);            // Configure the equation to calculate NH4 concentration value
  nh4_ppm = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  delay(DELAY_100_MILLISECONDS);

  MQ135.setA(34.668);
  MQ135.setB(-3.369);               // Configure the equation to calculate Aceton concentration value
  aceton_ppm = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  delay(DELAY_100_MILLISECONDS);

  if (co_ppm < 9.0) {

    ambient_air_quality_indicator = "Excellent";

  } else if (co_ppm > 9.0 && co_ppm <= 11.0) {

    ambient_air_quality_indicator = "Moderate";

  } else if (co_ppm > 11.0 && co_ppm <= 13.0) {

    ambient_air_quality_indicator = "Bad";

  } else if (co_ppm > 13.0 && co_ppm <= 15.0) {

    ambient_air_quality_indicator = "Very bad";

  } else {

    ambient_air_quality_indicator = "Danger";
  }

  Serial.println("- - - - - - - - - - - - - - -");

  Serial.println("CO: " + String(co_ppm));
  Serial.println("CO2: " + String(co2_ppm));
  Serial.println("Toluen: " + String(toluen_ppm));
  Serial.println("NH4: " + String(nh4_ppm));
  Serial.println("Aceton: " + String(aceton_ppm));
  Serial.println("Alcohol: " + String(alcohol_ppm));

  Serial.println("- - - - - - - - - - - - - - -");
}
