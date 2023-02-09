#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>
typedef struct
{
    volatile unsigned char *port;
    volatile unsigned char *ddr;
    uint16_t pin;
    uint16_t pos_0;
    uint16_t pos_max;
    uint16_t pos;
} engine;

class EngineControl
{
private:
    engine engines[10];

    unsigned int counter = 0;
    typedef enum
    {
        SERVO_UP,
        SERVO_DOWN,
        SERVO_WAIT
    } servo_type;
    servo_type state = SERVO_UP;

public:
    EngineControl() : engines({{&PORTB, &DDRB, 0, 15624, 31248, 15624},
                               {&PORTB, &DDRB, 1, 15624, 31248, 7812},
                               {&PORTB, &DDRB, 2, 15624, 31248, 15624},
                               {&PORTB, &DDRB, 3, 15624, 31248, 7812},
                               {&PORTB, &DDRB, 4, 15624, 31248, 15624},
                               {&PORTB, &DDRB, 5, 15624, 31248, 10000},
                               {&PORTB, &DDRB, 6, 15624, 31248, 15624},
                               {&PORTB, &DDRB, 7, 15624, 31248, 10000},
                               {&PORTC, &DDRC, 0, 15624, 31248, 15624},
                               {&PORTC, &DDRC, 1, 15624, 31248, 7812}

                      })
    {
        for (auto &e : engines)
        {
            *e.ddr |= 1 << e.pin;
            *e.port ^= 0 << e.pin;
        }
    };

    void handler();
    void load(uint16_t *ar);
};