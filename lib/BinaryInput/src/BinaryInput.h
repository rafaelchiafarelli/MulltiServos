#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>

#ifndef _PIND
#define _PIND 0x09
#define _PORTD 0x0B
#endif
#ifndef _DDRD
#define _DDRD 0x0A
#endif
#ifndef _PING
#define _PING 0x12
#define _PORTG 0x14
#endif
#ifndef _DDRG
#define _DDRG 0x13
#endif
#ifndef _PINL
#define _PINL 0x109
#define _PORTL 0x10B
#endif
#ifndef _DDRL
#define _DDRL 0x10a
#endif
#ifndef _PINB
#define _PINB 0x03
#define _PORTB 0x05

#endif
#ifndef _DDRB
#define _DDRB 0x04
#endif

#define MAX_PIN_AMOUNT 20
#define MAX_PIN_PORT 4
typedef struct
{
    volatile uint8_t * pins;
    volatile uint8_t * pull_up;
    volatile uint8_t * ddr;
    uint8_t ddr_mask;
    
    
} input_port;

typedef struct
{
    uint8_t value;
}input_value;

typedef struct 
{
    uint8_t idx;
    uint8_t mask;
    
}pin_to_index;

class BinaryInputs
{
public:
    input_value in[MAX_PIN_PORT];
    const input_port ports[MAX_PIN_PORT];
    const pin_to_index pins[MAX_PIN_AMOUNT];
    
    void handler()
    {
        in[0].value = *ports[0].pins;
        in[1].value = *ports[1].pins;
        in[2].value = *ports[2].pins;
        in[3].value = *ports[3].pins;
        
    };
    int get_pin(uint8_t idx){
        if(idx<MAX_PIN_AMOUNT)
            return ((in[pins[idx].idx].value & pins[idx].mask) == 0);
        else
            return false;
    }
    BinaryInputs() : ports({
                            {&PINA,&PORTA, &DDRA, 0b00000000},    /* PORTD */
                            {&PINC,&PORTC, &DDRC, 0b00000000},    /* PORTG */
                            {&PIND,&PORTD, &DDRD, 0b10111111},    /* PORTL */
                            {&PING,&PORTG, &DDRG, 0b11111000},    /* PORTB */
                            }),
                    pins({  {0,0b00000001},
                            {0,0b00000010},
                            {0,0b00000100},
                            {0,0b00001000},
                            {0,0b00010000},
                            {0,0b00100000},
                            {0,0b01000000},
                            {0,0b10000000},
                            {1,0b00000001},
                            {1,0b00000010},
                            {1,0b00000100},
                            {1,0b00001000},
                            {1,0b00010000},
                            {1,0b00100000},
                            {1,0b01000000},
                            {1,0b10000000},
                            {2,0b01000000},
                            {3,0b00000001},
                            {3,0b00000010},
                            {3,0b00000100}
                            })

    {
        DDRA = ports[0].ddr_mask;
        DDRC = ports[1].ddr_mask;
        DDRD = ports[2].ddr_mask;
        DDRG = ports[3].ddr_mask;
        MCUCR |= ~(1<<PUD);
        *ports[0].pull_up = ~ports[0].ddr_mask;
        *ports[1].pull_up = ~ports[1].ddr_mask;
        *ports[2].pull_up = ~ports[2].ddr_mask;
        *ports[3].pull_up = ~ports[3].ddr_mask;

    };
};
