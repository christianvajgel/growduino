#include "Multiplexer.h"
#include <Arduino.h>
#include <Wire.h>

#define TCAADDR 0x70    // endereço I2C do TCA9548A
#define CHANNEL_0 0x01  // canal 0 do multiplexador
#define CHANNEL_1 0x02  // canal 1 do multiplexador
#define CHANNEL_2 0x04  // canal 2 do multiplexador
#define CHANNEL_3 0x08  // canal 3 do multiplexador
#define CHANNEL_4 0x10  // canal 4 do multiplexador
#define CHANNEL_5 0x20  // canal 5 do multiplexador
#define CHANNEL_6 0x40  // canal 6 do multiplexador
#define CHANNEL_7 0x80  // canal 7 do multiplexador

void setup_multiplexer() {

  Wire.begin();

  Wire.beginTransmission(TCAADDR);

  Wire.write(CHANNEL_0 | CHANNEL_1 | CHANNEL_2 | CHANNEL_3 | CHANNEL_4 | CHANNEL_5 | CHANNEL_6 | CHANNEL_7);

  Wire.endTransmission();
}


void select_multiplexer_channel(uint8_t multiplexer_channel) {

  Wire.beginTransmission(0x70);          // TCA9548A address
  Wire.write(1 << multiplexer_channel);  // send byte to select bus
  Wire.endTransmission();
}
