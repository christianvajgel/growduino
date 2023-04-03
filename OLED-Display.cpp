#include "OLED-Display.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Multiplexer.h"
#include "Soil-Sensor.h"

#define SCREEN_WIDTH 128  // OLED width,  in pixels
#define SCREEN_HEIGHT 64  // OLED height, in pixels

//create an OLED display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void display_humidity(int humidityValue, int pos_X, int pos_Y) {
  select_multiplexer_channel(0);
  oled.clearDisplay();
  oled.setTextSize(1);           // set text size
  oled.setTextColor(WHITE);      // set text color
  oled.setCursor(pos_X, pos_Y);  // set position to display
  oled.println(String(humidityValue) + " %");
  oled.display();
}

void setup_oled_display() {
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1)
      ;
  }
  oled.setCursor(0, 0);
  oled.clearDisplay();
  Serial.println("setup_OLED_Display() *********");
}

void display_soil_humidity_percentage() {

  // select the OLED multiplexer channel for the right display
  select_multiplexer_channel(0);


  oled.clearDisplay();

  // Set text size and color for the humidity label
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  String humidityLabel = "H U M I D I T Y";
  int labelWidth = humidityLabel.length() * 6;  // 6 pixels per character
  int labelHeight = 16;                         // 16 pixels per line of text
  int centerX = oled.width() / 2;
  int centerY = oled.height() / 2;
  int labelX = centerX - labelWidth / 2;
  int labelY = centerY - labelHeight - 10;  // 10 pixels above the center
  oled.setCursor(labelX, labelY);           // set position to display
  oled.println(humidityLabel);

  // Set text size and color for the soil humidity percentage value
  oled.setTextSize(3);  // Increase text size to 3
  oled.setTextColor(WHITE);
  String humidityStr = String(soil_humidity_percentage) + "%";
  int textWidth = humidityStr.length() * 24;  // 24 pixels per character
  int textHeight = 36;                        // 36 pixels per line of text
  int textX = 39;
  int textY = 29;                // center vertically
  oled.setCursor(textX, textY);  // set position to display
  oled.println(humidityStr);

  // PSA: CENTER LINES ON X AND Y AXIS TO HELP CENTER THE CONTENT ON THE SCREEN
  // // Draw vertical line in the middle of Y-axis
  // oled_display_to_show.drawLine(centerX, 0, centerX, oled_display_to_show.height() - 1, WHITE);

  // // Draw horizontal line in the middle of X-axis
  // oled_display_to_show.drawLine(0, centerY, oled_display_to_show.width() - 1, centerY, WHITE);

  oled.display();
}
