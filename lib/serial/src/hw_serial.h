#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>
/**
 * protocol
 * header
 * semicolon separated values
 * footer
 * checksum
 * terminator
 *
 */
#define TERMINATOR '$'
#define HEADER '#'
#define FOOTER '*'
#define SEPARATOR ';'
#define CHECKSUM_SIZE 2

class hw_serial
{
private:
    uint8_t ReceivedBytes[256];
    unsigned int rcv_counter;
    uint8_t ParseBuffer[256];

    uint8_t OutBuffer[256];
    unsigned int out_index;
    unsigned int out_amount;
    bool able_to_parse = false;
    int checksum_counter = 0;
    unsigned int bytes_to_parse;

    typedef enum
    {
        Searching,
        Found,
        Ending

    } comm_type;
    comm_type comm_state = Searching;
    bool decode(uint16_t dest[], size_t max_size);
    bool Parse(uint16_t dest[], size_t max_size);
    int set(char *t, uint16_t dest[], int index);

    void uart_sendstr(uint8_t *data)
    {
        /*
        Use this to send a string, it will split it up into individual parts
        send those parts, and then send the new line code
        */
        UCSR0B &= 0b10111000;
        while (*data)
        {
            while ((UCSR0A & (1 << UDRE0)) == 0)
                ;         // make sure the data register is cleared
            UDR0 = *data; // goes through and splits the string into individual bits, sends them
            data += 1;    // go to new bit in string
        }
        while ((UCSR0A & (1 << UDRE0)) == 0)
            ;        // make sure the data register is cleared
        UDR0 = '\n'; // send a new line just to be sure
    }

public:
    void uart_sendstr(const char *data)
    {
        /*
        Use this to send a string, it will split it up into individual parts
        send those parts, and then send the new line code
        */
        UCSR0B &= 0b10111000;
        while (*data)
        {
            while ((UCSR0A & (1 << UDRE0)) == 0)
                ;         // make sure the data register is cleared
            UDR0 = *data; // goes through and splits the string into individual bits, sends them
            data += 1;    // go to new bit in string
        }
        while ((UCSR0A & (1 << UDRE0)) == 0)
            ;        // make sure the data register is cleared
        UDR0 = '\n'; // send a new line just to be sure
    }

    uint8_t send_one();
    void send(const char *str, size_t s);
    uint8_t receive(uint8_t d);
    bool handler(uint16_t dest[], size_t max_size);
    hw_serial(/* args */)
    {
        memset(ParseBuffer, 0, 256);
        memset(ReceivedBytes, 0, 256);
        memset(OutBuffer, 0, 256);
    };
    ~hw_serial(){};
};