#include <avr/io.h>

#define OUTPUT_PIN PD4

void setup() {
  DDRD |= 1 << OUTPUT_PIN;
}

void loop() {
  volatile unsigned int led_value;
  led_value = analogRead(A0);
  
  if (led_value > 250) {
    PORTD |= (1 << OUTPUT_PIN);
  } else PORTD &= ~(1<<OUTPUT_PIN);
  delay(200);
}
