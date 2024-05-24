#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    float avgLetters = ((float) letters / (float) words) * 100;
    float avgSentences = ((float) sentences / (float) words) * 100;
    int index = round(0.0588 * avgLetters - 0.296 * avgSentences - 15.8);

    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_sentences(string text)
{
    // ASCII !,.,? -> 33, 46, 63
    // All words except the first are preceded by a space. As such, 1 is added to the return value.
    int i = 0;
    int sentences = 0;
    do
    {
        char ch = text[i];
        if (ch == 33 || ch == 46 || ch == 63)
        {
            sentences++;
        }
        i++;

    }
    while(text[i] != 0);

    return sentences;
}

int count_words(string text)
{
    // ASCII Space -> 32
    // All words except the first are preceded by a space. As such, 1 is added to the return value.
    int i = 0;
    int words = 0;
    do
    {
        char ch = text[i];
        if (ch == 32)
        {
            words++;
        }
        i++;

    }
    while(text[i] != 0);

    return words + 1;
}

int count_letters(string text)
{
    // ASCII A-Z -> 65 to 90
    // ASCII a-z -> 97 to 122
    int letters = 0;
    int i = 0;
    do
    {
        char ch = text[i];
        if (isalpha(ch))
        {
            letters++;
        }
        i++;

    }
    while (text[i] != 0);

    return letters;
}