#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>
#define _PORTK 0x108
#define _DDRK 0x107
#define _PORTF 0x31
#define _DDRF 0x30
#define _PORTA 0x22
#define _DDRA 0x21
#define _PORTC 0x28
#define _DDRC 0x27
typedef struct
{
    volatile uint8_t * port;
    volatile uint8_t * ddr;
    
} out_port;

typedef union 
{
    
    uint8_t output[4];
    uint32_t inputvalue;
}outputs;


class BinaryOutputs
{
public:
    const out_port ports[4];
    outputs out;
    void handler()
    {
        *ports[0].port = out.output[0];
        *ports[1].port = out.output[1];
        *ports[2].port = out.output[2];
        *ports[3].port = out.output[3];
    };
    void load(uint32_t v)
    {
        out.inputvalue = v;
    }
    BinaryOutputs() : ports({{(volatile uint8_t *)(_PORTK), (volatile uint8_t *)(_DDRK)},   /* PORTK */
                            {(volatile uint8_t *)(_PORTF), (volatile uint8_t *)(_DDRF)},    /* PORTF */
                            {(volatile uint8_t *)(_PORTA), (volatile uint8_t *)(_DDRA)},    /* PORTA */
                            {(volatile uint8_t *)(_PORTC), (volatile uint8_t *)(_DDRC)}     /* PORTC */
                            })
    {
        *ports[0].ddr = 0xFF;
        *ports[1].ddr = 0xFF;
        *ports[2].ddr = 0xFF;
        *ports[3].ddr = 0xFF;
        
        *ports[0].port = 0x00;
        *ports[1].port = 0x00;
        *ports[2].port = 0x00;
        *ports[3].port = 0x00;
        
    };
};