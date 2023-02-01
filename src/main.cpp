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
hw_serial Serial;
engine engines[] =
    {
        {&PORTB, &DDRB, 0, 15624, 31248, 15624},
        {&PORTB, &DDRB, 1, 15624, 31248, 7812},
        {&PORTB, &DDRB, 2, 15624, 31248, 15624},
        {&PORTB, &DDRB, 3, 15624, 31248, 7812},
        {&PORTB, &DDRB, 4, 15624, 31248, 15624},
        {&PORTB, &DDRB, 5, 15624, 31248, 10000},
        {&PORTB, &DDRB, 6, 15624, 31248, 15624},
        {&PORTB, &DDRB, 7, 15624, 31248, 10000},
        {&PORTC, &DDRC, 0, 15624, 31248, 15624},
        {&PORTC, &DDRC, 1, 15624, 31248, 7812}

};
unsigned int counter = 0;
typedef enum
{
  SERVO_UP,
  SERVO_DOWN,
  SERVO_WAIT
} servo_type;
servo_type state = SERVO_UP;

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

  for (auto &e : engines)
  {
    *e.ddr |= 1 << e.pin;
    *e.port ^= 0 << e.pin;
  }
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

  switch (state)
  {
  case SERVO_UP:
    OCR4A = engines[counter].pos_0 + engines[counter].pos;
    *engines[counter].port ^= 1 << engines[counter].pin;
    state = SERVO_DOWN;
    break;
  case SERVO_DOWN:
    *engines[counter].port ^= 1 << engines[counter].pin;

    if (engines[counter].pos_max <= engines[counter].pos + engines[counter].pos_0)
    {
      counter++;
      if (counter >= sizeof(engines) / sizeof(engine))
      {
        counter = 0;
      }
      OCR4A = engines[counter].pos_0 + engines[counter].pos;
      *engines[counter].port ^= 1 << engines[counter].pin;
    }
    else
    {
      OCR4A = engines[counter].pos_max - engines[counter].pos_0 - engines[counter].pos;
      counter++;
      if (counter >= sizeof(engines) / sizeof(engine))
      {
        counter = 0;
      }
      state = SERVO_UP;
    }
    break;
  case SERVO_WAIT:

    break;
  }
}

ISR(USART0_RX_vect)
{
  UDR0 = Serial.receive(UDR0);
}
ISR(USART0_TX_vect)
{
  UDR0 = Serial.send_one();
}

int main()
{
  uint16_t array[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  setup();

  _delay_ms(100);

  while (1)
  {

    if (Serial.handler(array, 10))
    {
      char tmp[100];
      memset(tmp, 0, 100);
      sprintf(tmp, "\na1:%d a2:%d a3:%d a4:%d a5:%d a6:%d a7:%d a8:%d a9:%d a10:%d", array[0], array[1], array[2], array[3], array[4], array[5], array[6], array[7], array[8], array[9]);

      Serial.uart_sendstr(tmp);
      for (int i = 0; i < 10; i++)
        engines[i].pos = array[i];
    }
    _delay_ms(10);
  }

  return 0;
}