#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>

#define MAX_OUTPUT_SIZE 32

typedef struct
{
    volatile uint8_t * port;
    volatile uint8_t * ddr;
    const uint8_t mask_set;
    const uint8_t mask_reset;
    
} out_port;




class BinaryOutputs
{
public:
    out_port ports[MAX_OUTPUT_SIZE];


    void SetOutput(int i, bool v)
    {
        if(v) //set the output
            *ports[i].port |= ports[i].mask_set;
        else //reset the output
            *ports[i].port &= ports[i].mask_reset;
    }
    BinaryOutputs() : ports({
                            {&PORTK, &DDRK,0b00000010,0b11111101},
                            {&PORTK, &DDRK,0b00000001,0b11111110},
                            {&PORTK, &DDRK,0b00000100,0b11111011},
                            {&PORTK, &DDRK,0b00001000,0b11110111},
                            {&PORTK, &DDRK,0b00010000,0b11101111},
                            {&PORTK, &DDRK,0b00100000,0b11011111},
                            {&PORTK, &DDRK,0b01000000,0b10111111},
                            {&PORTK, &DDRK,0b10000000,0b01111111},
                            {&PORTF, &DDRF,0b00000001,0b11111110},
                            {&PORTF, &DDRF,0b00000010,0b11111101},
                            {&PORTF, &DDRF,0b00000100,0b11111011},
                            {&PORTF, &DDRF,0b00001000,0b11110111},
                            {&PORTF, &DDRF,0b00010000,0111101111},
                            {&PORTF, &DDRF,0b00100000,0b11011111},
                            {&PORTF, &DDRF,0b01000000,0b10111111},
                            {&PORTF, &DDRF,0b10000000,0b01111111},
                            {&PORTB, &DDRB,0b00000001,0b11111110},
                            {&PORTH, &DDRH,0b00000010,0b11111101},
                            {&PORTJ, &DDRJ,0b00000001,0b11111110},
                            {&PORTJ, &DDRJ,0b00000010,0b11111101},
                            {&PORTJ, &DDRJ,0b00000100,0b11111011},
                            {&PORTJ, &DDRJ,0b00001000,0b11110111},
                            {&PORTJ, &DDRJ,0b00010000,0b11101111},
                            {&PORTD, &DDRD,0b00000001,0b11111110},
                            {&PORTD, &DDRD,0b00000010,0b11111101},
                            {&PORTD, &DDRD,0b00000100,0b11111011},
                            {&PORTD, &DDRD,0b00001000,0b11110111},
                            {&PORTE, &DDRE,0b00000001,0b11111110},
                            {&PORTE, &DDRE,0b00000010,0b11111101},
                            {&PORTE, &DDRE,0b00100000,0b11011111}
                            })

    {
        for(int i=0; i<MAX_OUTPUT_SIZE; i++)
        {
            *ports[i].ddr |= ports[i].mask_set;
            *ports[i].port &= ports[i].mask_reset;
        }

    };
};