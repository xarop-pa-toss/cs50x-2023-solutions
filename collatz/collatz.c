#include <cs50.h>
#include <stdio.h>

int collatz(int n);

int main(void)
{
    printf("%i\n",collatz(get_int("Number to collatz: ")));
}

int collatz(int n)
{
    // base case
    if (n == 1)
        return 0;

    // two recursions (for odd and even)
    if ((n % 2) == 0)
        return 1 + collatz(n / 2);
    else
        return 1 + collatz((3 * n) + 1);

}