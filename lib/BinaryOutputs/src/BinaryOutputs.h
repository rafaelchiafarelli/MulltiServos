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
    BinaryOutputs() : pins({{&PORTK, &DDRK, 0},
                            {&PORTK, &DDRK, 1},
                            {&PORTK, &DDRK, 2},
                            {&PORTK, &DDRK, 3},
                            {&PORTK, &DDRK, 4},
                            {&PORTK, &DDRK, 5},
                            {&PORTK, &DDRK, 6},
                            {&PORTK, &DDRK, 7},
                            {&PORTF, &DDRF, 0},
                            {&PORTF, &DDRF, 1},
                            {&PORTF, &DDRF, 2},
                            {&PORTF, &DDRF, 3},
                            {&PORTF, &DDRF, 4},
                            {&PORTF, &DDRF, 5},
                            {&PORTF, &DDRF, 6},
                            {&PORTF, &DDRF, 7},
                            {&PORTL, &DDRL, 0},
                            {&PORTL, &DDRL, 1},
                            {&PORTL, &DDRL, 2},
                            {&PORTL, &DDRL, 3},
                            {&PORTL, &DDRL, 4},
                            {&PORTL, &DDRL, 5},
                            {&PORTL, &DDRL, 6},
                            {&PORTL, &DDRL, 7},
                            {&PORTG, &DDRG, 0},
                            {&PORTG, &DDRG, 1},
                            {&PORTG, &DDRG, 2},
                            {&PORTG, &DDRG, 3},
                            {&PORTG, &DDRG, 4},
                            {&PORTG, &DDRG, 5},
                            {&PORTG, &DDRG, 6},
                            {&PORTG, &DDRG, 7}})
    {
        for (auto &e : pins)
        {
            *e.ddr |= 1 << e.pin;
            *e.port ^= 0 << e.pin;
        }
    };
};