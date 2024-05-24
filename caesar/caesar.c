#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>\
#include <stdlib.h>

bool only_digits(string inputText, int length);
char rotate(char c, int key);

int main(int argc, string argv[])
{
    //Check # of arguments and if the argument is made up of only digits.
    if (argc != 2 || !only_digits(argv[1], strlen(argv[1])))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    string plaintext = get_string("plaintext:  ");
    int length = strlen(plaintext);

    int key = atoi(argv[1]);
    char ciphertext[length];

    //Filling ciphertext string with rotated letters from plaintext as per key
    for (int i = 0; i < length; i++)
    {
        ciphertext[i] = rotate(plaintext[i], key);
    }

    printf("ciphertext: %s", ciphertext);
    printf("\n");
    return 0;
}


char rotate(char c, int key)
{
    char x = 0;

    if (isupper(c))
    {
        //ASCII 'A' = 65. We use it as base 0 to facilitate the math.
        x = c - 65;
        char newChar = (x + key) % 26;

        return newChar + 65;
    }
    else if (islower(c))
    {
        //ASCII 'a' = 97. We use it as base 0 to facilitate the math.
        x = c - 97;
        char newChar = (x + key) % 26;

        return newChar + 97;
    }
    else
    {
        return c;
    }
}


bool only_digits(string inputText, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (!isdigit(inputText[i]))
        {
            return false;
        }
    }
    return true;
}