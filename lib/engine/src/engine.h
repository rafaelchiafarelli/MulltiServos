#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>

#define NUMBER_OF_ENGINES 10

typedef enum
{
    SERVO_UP,
    SERVO_DOWN,
    SERVO_WAIT
} servo_type;

typedef struct
{
    volatile unsigned char *port;
    volatile unsigned char *ddr;
    const uint8_t mask_high_pin;
    const uint8_t mask_low_pin;
    const uint16_t pos_0;
    const uint16_t pos_max;
    
} engine;

class EngineControl
{
private:
    uint16_t pos[NUMBER_OF_ENGINES];
    const engine engines[NUMBER_OF_ENGINES];

    unsigned int counter = 0;

public:
    servo_type state = SERVO_UP;
    EngineControl() : engines({
        {&PORTB, &DDRB, 0b00000001, 0b11111110, 15624, 31248},
        {&PORTB, &DDRB, 0b00000010, 0b11111101, 15624, 31248},
        {&PORTB, &DDRB, 0b00000100, 0b11111011, 15624, 31248},
        {&PORTB, &DDRB, 0b00001000, 0b11110111, 15624, 31248},
        {&PORTL, &DDRL, 0b00000001, 0b11111110, 15624, 31248},
        {&PORTL, &DDRL, 0b00000010, 0b11111101, 15624, 31248},
        {&PORTL, &DDRL, 0b00000100, 0b11111011, 15624, 31248},
        {&PORTL, &DDRL, 0b00001000, 0b11110111, 15624, 31248},
        {&PORTL, &DDRL, 0b00010000, 0b11101111, 15624, 31248},
        {&PORTL, &DDRL, 0b00100000, 0b11011111, 15624, 31248}
        })
        
    {
        for(int i =0; i<NUMBER_OF_ENGINES; i++)
        {
            pos[i] = 15624;
        }
        for (auto &e : engines)
        {
            *e.ddr |= e.mask_high_pin;
            *e.port ^= e.mask_low_pin;
        }
    };

    void handler();
    void load(uint16_t *ar);
};