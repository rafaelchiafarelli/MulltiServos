#include <hw_serial.h>
uint8_t hw_serial::receive(uint8_t rcv)
{
    rcv_counter++;
    ReceivedBytes[rcv_counter] = rcv; // Fetch the received byte value into the variable "ByteReceived"
    switch (comm_state)
    {
    case Searching:
        if (ReceivedBytes[rcv_counter] == HEADER)
        {
            comm_state = Found;
            rcv_counter = 0;
            ReceivedBytes[0] = rcv;
        }
        break;
    case Found:
        if (ReceivedBytes[rcv_counter] == FOOTER)
        {
            comm_state = Ending;
        }
        else if (ReceivedBytes[rcv_counter] == HEADER)
        {
            rcv_counter = 0;
            ReceivedBytes[0] = rcv;
        }

        break;
    case Ending:
        checksum_counter += 1;
        if (ReceivedBytes[rcv_counter] == TERMINATOR)
        {

            sprintf((char *)ParseBuffer, "%s", ReceivedBytes); //
            bytes_to_parse = rcv_counter;
            checksum_counter = 0;
            comm_state = Searching;

            able_to_parse = true;

        }
        else if (checksum_counter > CHECKSUM_SIZE)
        {
            comm_state = Searching;
            checksum_counter = 0;
        }
        else if (ReceivedBytes[rcv_counter] == HEADER)
        {
            rcv_counter = 0;
            ReceivedBytes[0] = rcv;
        }
        break;
    default:
        break;
    }
    return rcv;
}
uint8_t hw_serial::send_one()
{
    uint8_t ret = 0;
    if (out_index >= out_amount)
    {
        UCSR0B &= 0b10111111;
    }
    else
    {
        ret = OutBuffer[out_index];
        out_index += 1;
    }

    return ret;
}

bool hw_serial::Parse(uint32_t dest[], size_t max_size)
{
    uint8_t checksum = 0;
    uint8_t *ptr = ParseBuffer;
    for (; ptr < ParseBuffer + bytes_to_parse; ptr++)
    {
        checksum ^= *ptr;
        if (*ptr == '*')
            break;
    }
    ptr++;

    const char *check = (const char *)ptr;

    uint8_t cs = atoi(check);

    if (cs == checksum)
    {

        return decode(dest, max_size);
    }

    return false;
}

void hw_serial::send(const char *str, size_t s)
{
    memcpy(OutBuffer, str, s);
    out_index = 1;
    out_amount = s;
    UCSR0B |= (1 << TXEN0);
    UCSR0B |= (1 << TXCIE0);
    UDR0 = OutBuffer[0];
}

bool hw_serial::handler(uint32_t dest[], size_t max_size)
{

    if (able_to_parse)
    {
        able_to_parse = false;

        return Parse(dest, max_size);
    }
    return false;
}

bool hw_serial::decode(uint32_t dest[], size_t max_size)
{
    char *token, *str, *tofree;

    tofree = str = strdup((const char *)(ParseBuffer + 1)); // We own str's memory now.
    int i = 0;
    while ((token = strsep(&str, ";")) && i < (int)max_size)
        i += set(token, dest, i);
    free(tofree);
    return true;
}

int hw_serial::set(char *t, uint32_t dest[], int index)
{
    uint16_t tmp = atol(t);
    dest[index] = tmp;
    return 1;
}