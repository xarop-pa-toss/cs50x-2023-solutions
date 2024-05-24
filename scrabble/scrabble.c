#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 == score2)
    {
        printf("Tie!");
    }
    else if(score1 > score2)
    {
        printf("Player 1 wins!");
    }
    else
    {
        printf("Player 2 wins!");
    }
}

int compute_score(string word)
{
    int wordPoints = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        char letter = word[i];
        //turns any lowercase to uppercase
        if (!isupper(letter))
        {
            letter = toupper(letter);
        }

        // A is ASCII 65, B is ASCII 66 and so on.
        // Doing any letter - 65 will subtract 65 from it's ASCII value, thus corresponding to it's POINTS position.
        // A - 65 = 0 -> POINTS[0], B - 65 = 1 -> POINTS[1], etc.
        // Ignores non-letters
        if (letter >= 65 && letter <= 90)
        {
            wordPoints = wordPoints + POINTS[letter - 65];
        }
    }
    return wordPoints;
}
