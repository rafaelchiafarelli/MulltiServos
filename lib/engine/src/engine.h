#pragma once

typedef struct
{
    volatile unsigned char *port;
    volatile unsigned char *ddr;
    uint16_t pin;
    uint16_t pos_0;
    uint16_t pos_max;
    uint16_t pos;
} engine;