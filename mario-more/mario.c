#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int rows;
    do {
        rows = get_int("How tall/long should the pyramid be? ");
    } while (rows < 1 || rows > 8);

    for (int i = 1; i <= rows; i++)
    {
        //Print Spaces for first pyramid padding.
        //Since rows/columns are directly proportionate, Spaces = total rows - current row (#)
        for (int spaces = 0; spaces < rows - i; spaces++ ) { printf(" ");}

        //Print first pyramid
        for (int h = 0; h < i; h++) { printf("#");}

        // Print gap
        printf("  ");

        //Print second pyramid
        for (int x = 0; x < i; x++) { printf("#"); }

        //Break line
        printf("\n");
    }
}
