#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int start;
    int end;

    // TODO: Prompt for start size
    do
    {
        start = get_int("Start size of population: ");
    }
    while (start < 9);

    int current = start;

    // TODO: Prompt for end size
    do
    {
        end = get_int("End size of population: ");
    }
    while (end < start);

    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    while (current < end)
    {
        current = current + (current / 3) - (current / 4);
        years++;
    }

    // TODO: Print number of years
    printf("Years: %i", years);
}
