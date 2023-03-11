#include "engine.h"
void EngineControl::load(uint16_t *ar)
{
    for (int i = 0; i < NUMBER_OF_ENGINES; i++)
    {
        if (engines[i].pos_max <= engines[i].pos_0 + ar[i])
            pos[i] = engines[i].pos_0 + ar[i];
        else
            pos[i] = engines[i].pos_max;
        if (ar[i]<engines[i].pos_0)
            pos[i] = engines[i].pos_0;
    }
}

void EngineControl::handler()
{
    switch (state)
    {
    case SERVO_UP:
        OCR4A = pos[counter];
        *engines[counter].port |= engines[counter].mask_high_pin;
        state = SERVO_DOWN;
        break;
    
    case SERVO_DOWN:
        *engines[counter].port &= engines[counter].mask_low_pin;
        OCR4A = engines[0].pos_max - pos[counter];
        counter++;
        if (counter >= NUMBER_OF_ENGINES)
        {
            counter = 0;
        }
        
        state = SERVO_UP;
        break;
    case SERVO_WAIT:

        break;
    }
}