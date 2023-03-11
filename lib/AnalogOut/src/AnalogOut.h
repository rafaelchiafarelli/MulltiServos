#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define MAX_CHANNELS 8
class AnalogOut{

    private:
    uint8_t CalcBin(int duty_cycle)
    {
        uint8_t ocr = duty_cycle*255/100;
        return ocr;
    };
    public:
    AnalogOut()
    {

        DDRB |= 0b11111000;
        DDRG |= 0b00100000;
        DDRH |= 0b01000000;
        DDRE |= 0b00111000;
        TCCR0A = 0b10100001; //clear oc0a pin at botton (oc0b disconnected), fast pwm
        TCCR0B = 0b11000011; //force outputcompare at A, no prescaler, overflow at 0xFF
        TCNT0 = 0;  // initialize counter value to 0
        // set compare match register for 1hz increments
        OCR0A = 0b10000000; // 15624 -- 1ms = (16*10^6) / (1*1024) - 1 (must be <65536)
        OCR0B = 0b10000000; // 15624 -- 1ms = (16*10^6) / (1*1024) - 1 (must be <65536)
   
        TCCR1A = 0b10100001; //only oc1a and oc1b working, oc1c is shared output with oc0a
                                //working mode is 8bits (TOP=0x00FF)
        TCCR1B = 0b00000011;
        TCCR1C = 0b11000000;
        TCNT1 = 0;
        OCR1A = 0b10000000; // 15624 -- 1ms = (16*10^6) / (1*1024) - 1 (must be <65536)
        OCR1B = 0b10000000; // 15624 -- 1ms = (16*10^6) / (1*1024) - 1 (must be <65536)
        
        
        TCCR2A = 0b10100011;
        TCCR2B = 0b11000101;
        OCR2B = 0b10000000; // 15624 -- 1ms = (16*10^6) / (1*1024) - 1 (must be <65536)
        OCR2A = 0b10000000; // 15624 -- 1ms = (16*10^6) / (1*1024) - 1 (must be <65536)

        TCNT2 = 0;

        TCCR3A = 0b10100001;
        TCCR3B = 0b00001011;
        TCCR3C = 0b11000000;
        OCR3B = 0b10000000; // 15624 -- 1ms = (16*10^6) / (1*1024) - 1 (must be <65536)
        OCR3A = 0b10000000; // 15624 -- 1ms = (16*10^6) / (1*1024) - 1 (must be <65536)

        TCNT3 = 0;                

    }
    
    void SetAnalog(uint8_t idx, int duty_cycle)
    {
        if(idx < MAX_CHANNELS)
        {
            uint8_t v = duty_cycle;
            switch(idx)
            {
                case 0:
                    OCR0A = v;
                break;
                case 1:
                    OCR0B = v;
                break;
                case 2:
                    OCR1AL = v;
                    OCR1AH = 0;
                break;
                case 3:
                    OCR1BL = v;
                    OCR1BH = 0;
                break;
                case 4:
                    OCR2A = v;
                break;
                case 5:
                    OCR2B = v;
                break;
                case 6:
                    OCR3AL = v;
                    OCR3AH = 0;
                break;
                case 7:
                    OCR3BL = v;
                    OCR3B = 0;
                break;                                                                                    
            }
        }
    }

};