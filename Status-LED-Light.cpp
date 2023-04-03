#include "WString.h"
#include "Status-LED-Light.h"
#include "PIN-Namespace.h"
#include "Delay.h"

int red_pin_value;
int green_pin_value;
int blue_pin_value;
int led_test_light_counter;

void turn_on_led_light_on_color(String color) {

  // Serial.println(color);

  if (color == "red") {
    red_pin_value = 25;
    green_pin_value = 0;
    blue_pin_value = 0;
  }

  else if (color == "green") {
    red_pin_value = 0;
    green_pin_value = 25;
    blue_pin_value = 0;
  }

  else if (color == "blue") {
    // red_pin_value = 0;
    // green_pin_value = 25;
    // blue_pin_value = 5;
    red_pin_value = 0;
    green_pin_value = 5;
    blue_pin_value = 25;
  }

  else if (color == "yellow") {
    red_pin_value = 25;
    green_pin_value = 25;
    blue_pin_value = 0;
  }

  else if (color == "orange") {
    red_pin_value = 51;
    green_pin_value = 5;
    blue_pin_value = 0;
  }

  else if (color == "purple") {
    red_pin_value = 25;  //51
    green_pin_value = 0;
    blue_pin_value = 25;
  }

  else if (color == "white") {
    red_pin_value = 5;
    green_pin_value = 5;
    blue_pin_value = 5;
  }

  else {
    red_pin_value = 0;
    green_pin_value = 0;
    blue_pin_value = 0;
  }

  analogWrite(RED_LED_PIN, red_pin_value);
  analogWrite(BLUE_LED_PIN, blue_pin_value);
  analogWrite(GREEN_LED_PIN, green_pin_value);
}

void turn_off_led_lights() {
  analogWrite(RED_LED_PIN, 0);
  analogWrite(BLUE_LED_PIN, 0);
  analogWrite(GREEN_LED_PIN, 0);
}

void test_led_lights() {

  for (led_test_light_counter = 0; led_test_light_counter < 255; led_test_light_counter++) {
    analogWrite(RED_LED_PIN, led_test_light_counter);
    analogWrite(GREEN_LED_PIN, 0);
    analogWrite(BLUE_LED_PIN, 0);
    delay(DELAY_10_MILLISECONDS);
  }

  for (led_test_light_counter = 255; led_test_light_counter > 0; led_test_light_counter--) {
    analogWrite(RED_LED_PIN, 0);
    analogWrite(GREEN_LED_PIN, led_test_light_counter);
    analogWrite(BLUE_LED_PIN, 0);
    delay(DELAY_10_MILLISECONDS);
  }

  for (led_test_light_counter = 0; led_test_light_counter < 255; led_test_light_counter++) {
    analogWrite(RED_LED_PIN, 0);
    analogWrite(GREEN_LED_PIN, 0);
    analogWrite(BLUE_LED_PIN, led_test_light_counter);
    delay(DELAY_10_MILLISECONDS);
  }

  for (led_test_light_counter = 255; led_test_light_counter > 0; led_test_light_counter--) {
    analogWrite(RED_LED_PIN, led_test_light_counter);
    analogWrite(GREEN_LED_PIN, 255 - led_test_light_counter);
    analogWrite(BLUE_LED_PIN, 128 - led_test_light_counter);
    delay(DELAY_10_MILLISECONDS);
  }
  for (led_test_light_counter = 0; led_test_light_counter < 255; led_test_light_counter++) {
    analogWrite(RED_LED_PIN, led_test_light_counter);
    analogWrite(GREEN_LED_PIN, 255 - led_test_light_counter);
    analogWrite(BLUE_LED_PIN, 128 - led_test_light_counter);
    delay(DELAY_10_MILLISECONDS);
  }
  for (led_test_light_counter = 255; led_test_light_counter > 0; led_test_light_counter--) {
    analogWrite(RED_LED_PIN, led_test_light_counter);
    analogWrite(GREEN_LED_PIN, led_test_light_counter);
    analogWrite(BLUE_LED_PIN, led_test_light_counter);
    delay(DELAY_10_MILLISECONDS);
  }
  analogWrite(RED_LED_PIN, 0);
  analogWrite(GREEN_LED_PIN, 0);
  analogWrite(BLUE_LED_PIN, 0);
  delay(DELAY_1_SECOND);
}
