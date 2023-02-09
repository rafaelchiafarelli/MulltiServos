#include "engine.h"
void EngineControl::load(uint16_t *ar)
{
    for (int i = 0; i < 10; i++)
    {
        engines[i].pos = ar[i];
    }
}

void EngineControl::handler()
{
    switch (state)
    {
    case SERVO_UP:
        OCR4A = engines[counter].pos_0 + engines[counter].pos;
        *engines[counter].port ^= 1 << engines[counter].pin;
        state = SERVO_DOWN;
        break;
    case SERVO_DOWN:
        *engines[counter].port ^= 1 << engines[counter].pin;

        if (engines[counter].pos_max <= engines[counter].pos + engines[counter].pos_0)
        {
            counter++;
            if (counter >= sizeof(engines) / sizeof(engine))
            {
                counter = 0;
            }
            OCR4A = engines[counter].pos_0 + engines[counter].pos;
            *engines[counter].port ^= 1 << engines[counter].pin;
        }
        else
        {
            OCR4A = engines[counter].pos_max - engines[counter].pos_0 - engines[counter].pos;
            counter++;
            if (counter >= sizeof(engines) / sizeof(engine))
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