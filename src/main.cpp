#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "engine.h"
#include "hw_serial.h"
#include "BinaryOutputs.h"
hw_serial Serial;

BinaryOutputs switches;

EngineControl servos;
ISR(TIMER4_COMPA_vect);
ISR(USART0_RX_vect);
ISR(USART0_TX_vect);
#define USART_BAUDRATE 250000
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void setup()
{
  // make the LED pin an output for PORTB7
  // DDRB = 1 << 7;
  // engines[0].ddr = 1 << engines[1].pin;

  cli(); // stop interrupts

  // set timer4 interrupt at 1Hz
  TCCR4A = 0; // set entire TCCR1A register to 0
  TCCR4B = 0; // same for TCCR1B
  TCNT4 = 0;  // initialize counter value to 0
  // set compare match register for 1hz increments
  OCR4A = 555; // 15624 -- 1ms = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR4B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR4B |= (1 << CS40);
  // enable timer compare interrupt
  TIMSK4 |= (1 << OCIE4A);

  UCSR0B |= (1 << RXEN0);                  // transmit side of hardware
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // receive side of hardware

  UBRR0L = BAUD_PRESCALE;        // set the baud to 9600, have to split it into the two registers
  UBRR0H = (BAUD_PRESCALE >> 8); // high end of baud register

  UCSR0B |= (1 << RXCIE0); // recieve data interrupt, makes sure we don't loose data

  sei(); // allow interrupts
}

ISR(TIMER4_COMPA_vect)
{
  // PORTB ^= 1 << 7;
  servos.handler();
}

ISR(USART0_RX_vect)
{
  Serial.receive(UDR0);
}
ISR(USART0_TX_vect)
{
  UDR0 = Serial.send_one();
}
static uint16_t array[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int main()
{
  setup();

  while (1)
  {
    if (Serial.handler(array, 16))
    {
      char tmp[50];
      memset(tmp,0,50);

      char ansi_0[17];
      memset(ansi_0, 0, 17);
      itoa(array[10],ansi_0,2);

      char ansi_1[17];
      memset(ansi_1, 0, 17);
      itoa(array[11],ansi_1,2);

      sprintf(tmp, "a10:%u %s a11:%u %s",array[10], ansi_0,array[11], ansi_1);

      Serial.send(tmp, strlen(tmp));

      servos.load(array);
    }
    uint32_t value0 = array[10];
    uint32_t value1 = array[11];
    uint32_t value = ((uint32_t)value0<<16) + value1;
    switches.load(value);
    switches.handler();
    _delay_ms(50);
  }
  return 0;
}