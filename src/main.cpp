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
#include "AnalogOut.h"
#include "BinaryInput.h"
hw_serial Serial;
AnalogOut analogs;
BinaryOutputs switches;
BinaryInputs buttons;
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

  UBRR0L = BAUD_PRESCALE;        // set the baud to 25000, have to split it into the two registers
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
static uint16_t array[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65535, 65535, 0, 0, 0};
int main()
{

  setup();

  while (1)
  {
    buttons.handler(); 
    if(buttons.get_pin(0))
    {
      analogs.SetAnalog(0, 255);
      analogs.SetAnalog(1, 0);
      analogs.SetAnalog(2, 0);
      analogs.SetAnalog(3, 255);
      analogs.SetAnalog(4, 0);
      analogs.SetAnalog(5, 0);
      analogs.SetAnalog(6, 0);
      analogs.SetAnalog(7, 0);  
    }
    if(buttons.get_pin(1))
    {
      analogs.SetAnalog(0, 0);
      analogs.SetAnalog(1, 255);
      analogs.SetAnalog(2, 0);
      analogs.SetAnalog(3, 0);
      analogs.SetAnalog(4, 255);
      analogs.SetAnalog(5, 0);
      analogs.SetAnalog(6, 0);
      analogs.SetAnalog(7, 0);  
    } 
    if(buttons.get_pin(2))
    {
      analogs.SetAnalog(0, 0);
      analogs.SetAnalog(1, 0);
      analogs.SetAnalog(2, 255);
      analogs.SetAnalog(3, 0);
      analogs.SetAnalog(4, 0);
      analogs.SetAnalog(5, 255);
      analogs.SetAnalog(6, 0);
      analogs.SetAnalog(7, 0);  
    }     
    /*
    int var[20];   
    for(int i=0; i<20;i++)
    {
      var[i] = buttons.get_pin(i);
    }
      char tmp[100];
      memset(tmp,0,100);
      sprintf(tmp, "b0:%d b1:%d b2:%d b3:%d b4:%d b5:%d b6:%d b7:%d b8:%d b9:%d b10:%d b11:%d b12:%d b13:%d b14:%d b15:%d b16:%d b17:%d b18:%d b19:%d ",var[0],var[1],var[2],var[3],var[4],var[5],var[6],var[7],var[8],var[9],var[10],var[11],var[12],var[13],var[14],var[15],var[16],var[17],var[18],var[19]);
     
      Serial.uart_sendstr(tmp);
    */
    if (Serial.handler(array, 16))
    {
            /*
      char tmp[50];
      memset(tmp,0,50);

      char ansi_0[17];
      memset(ansi_0, 0, 17);
      itoa(array[10],ansi_0,2);

      char ansi_1[17];
      memset(ansi_1, 0, 17);
      itoa(array[11],ansi_1,2);

      sprintf(tmp, "a10:%lu %s a11:%lu %s",array[10], ansi_0,array[11], ansi_1);
     
      Serial.send(tmp, strlen(tmp));
       */

      servos.load(array);
      uint16_t value0 = array[10]; // switches hi
      uint16_t value1 = array[11]; // switches lo

      uint8_t duty_R_LED0 = array[12]&0x00ff;
      uint8_t duty_G_LED0 = array[13]&0x00ff;
      uint8_t duty_B_LED0 = array[14]&0x00ff;

      uint8_t duty_R_LED1 = (array[12]&0xff00)>>8;
      uint8_t duty_G_LED1 = (array[13]&0xff00)>>8;
      uint8_t duty_B_LED1 = (array[14]&0xff00)>>8;

      uint8_t duty_LED0 = (array[15]&0x00ff);
      uint8_t duty_LED1 = (array[15]&0xff00)>>8;

      uint32_t value = ((uint32_t)value0<<16) + value1;
      
      switches.load(value);
      switches.handler();
      
      analogs.SetAnalog(0, duty_R_LED0);
      analogs.SetAnalog(1, duty_G_LED0);
      analogs.SetAnalog(2, duty_B_LED0);
      analogs.SetAnalog(3, duty_R_LED1);
      analogs.SetAnalog(4, duty_G_LED1);
      analogs.SetAnalog(5, duty_B_LED1);
      analogs.SetAnalog(6, duty_LED0);
      analogs.SetAnalog(7, duty_LED1);   

    }

    
    _delay_ms(50);
  }
  return 0;
}