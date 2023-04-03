#include "Buzzer.h"
#include <Arduino.h>
#include "PIN-Namespace.h"

void play_tone_with_delay(int tone_hertz, int tone_duration, int delay_milliseconds) {
  tone(BUZZER_DAC_PIN, tone_hertz, tone_duration);  // Gera um tom de 1kHz por 1 segundo
  delay(delay_milliseconds);                        // Espera 1 segundo antes de gerar o próximo tom
}

void turn_off_buzzer() {
  noTone(BUZZER_DAC_PIN);  // Desliga o buzzer
}
