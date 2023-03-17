#include "engine.h"
void EngineControl::load(uint16_t *ar)
{
    for (int i = 0; i < NUMBER_OF_ENGINES; i++)
    {
        if (ar[i] <= pos_max )
            pos[i] = ar[i]+pos_0;
        else
            pos[i] = pos_max+pos_0;
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

        if (pos[counter] >= pos_max + pos_0)
        {
            
            counter++;
            if (counter >= NUMBER_OF_ENGINES)
            {
                counter = 0;
            }
            OCR4A = pos[counter];
            *engines[counter].port |= engines[counter].mask_high_pin;
        }
        else
        {
            OCR4A = pos_max + pos_0 - pos[counter];
            counter++;
            if (counter >= NUMBER_OF_ENGINES)
            {
                counter = 0;
            }
            state = SERVO_UP;
        }
        break;
    case SERVO_WAIT:

        break;
    }
}