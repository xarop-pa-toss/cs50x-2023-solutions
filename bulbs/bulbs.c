#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // Ask for string
    string text = get_string("Insert text: ");
    int len = strlen(text);

    // Convert chars to correspondant ASCII values in a new array
    int ascii[len];

    for (int i = 0; i < len; i++)
    {
        ascii[i] = text[i];
    }

    // Create a second array that holds 8 bools
    int bulbs[8];

    for (int k = 0; k < len; k++)
    {
        int character = ascii[k];

        for (int i = 0; i < 8; i++)
        {
            bulbs[i] =  character % 2;
            character = character / 2;
        }

        for (int o = 7; o >= 0; o--)
        {
            print_bulb(bulbs[o]);
        }
        printf("\n");
    }
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
