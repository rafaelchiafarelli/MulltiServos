#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>
typedef struct
{
    volatile unsigned char *port;
    volatile unsigned char *ddr;
    uint16_t pin;

} outputs;
typedef union
{
    struct
    {
        unsigned char b : 1;
    } bits[32];
    uint32_t value;
} binary;

class BinaryOutputs
{
public:
    const outputs pins[32];
    binary out;
    void handler(binary re)
    {
        int i = 0;
        for (auto &e : pins)
        {
            *e.port ^= out.bits[i].b << e.pin;
            i++;
        }
    };
    void load(uint32_t v)
    {
        out.value = v;
    }
    BinaryOutputs() : pins({{&PORTB, &DDRB, 0},
                            {&PORTB, &DDRB, 1},
                            {&PORTB, &DDRB, 2},
                            {&PORTB, &DDRB, 3},
                            {&PORTB, &DDRB, 4},
                            {&PORTB, &DDRB, 5},
                            {&PORTB, &DDRB, 6},
                            {&PORTB, &DDRB, 7},
                            {&PORTC, &DDRC, 0},
                            {&PORTC, &DDRC, 1},
                            {&PORTB, &DDRB, 0},
                            {&PORTB, &DDRB, 1},
                            {&PORTB, &DDRB, 2},
                            {&PORTB, &DDRB, 3},
                            {&PORTB, &DDRB, 4},
                            {&PORTB, &DDRB, 5},
                            {&PORTB, &DDRB, 0},
                            {&PORTB, &DDRB, 1},
                            {&PORTB, &DDRB, 2},
                            {&PORTB, &DDRB, 3},
                            {&PORTB, &DDRB, 4},
                            {&PORTB, &DDRB, 5},
                            {&PORTB, &DDRB, 6},
                            {&PORTB, &DDRB, 7},
                            {&PORTC, &DDRC, 0},
                            {&PORTC, &DDRC, 1},
                            {&PORTB, &DDRB, 0},
                            {&PORTB, &DDRB, 1},
                            {&PORTB, &DDRB, 2},
                            {&PORTB, &DDRB, 3},
                            {&PORTB, &DDRB, 4},
                            {&PORTB, &DDRB, 5}})
    {
        for (auto &e : pins)
        {
            *e.ddr |= 1 << e.pin;
            *e.port ^= 0 << e.pin;
        }
    };
};