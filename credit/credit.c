#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    long ccNum = get_long("Insert credit card number please: ");
    long oddNum = 0;
    long evenNum = 0;

    // log10 of a number gives amount of times 10 has to be multiplied to achieve said number
    // Adding 1 to that will give us the digit total. Luckily the math library has a log10() function.
    int digits = (log10(ccNum)) + 1;

    // Step1 - get even positioned numbers, multiply by 2 and add together digits. Reset num at end.
    long num = ccNum; // used as proxy
    while (num)
    {
        num = num / 10;

        int lastDigitMulti = (num % 10) * 2;
        if (lastDigitMulti > 9) // if has 2+ digits subtract 9 to get both it's digits addition
        {
            evenNum = evenNum + (lastDigitMulti - 9);
        }
        else { evenNum = evenNum + lastDigitMulti;}

        num = num / 10;
    }
    num = ccNum;


    // Step 2 - get odd positioned numbers, check amount of digits and add digits together. Reset num
    while(num)
    {
        oddNum = oddNum + (num % 10);
        num = num / 10;
        num = num / 10;
    }
    num = ccNum;


    // Step 3 - Credit Card Validation.
    int checksum = (evenNum + oddNum) % 10; //gets final digit of checksum. Valid if 0
    int ccPrefix = num / pow(10, digits - 2);

    if (checksum == 0 && (digits >= 13 && digits <= 16))
    {
        if ((ccPrefix >= 51 && ccPrefix <=55) && digits == 16) {printf("MASTERCARD\n");}
        else if ((ccPrefix >= 40 && ccPrefix <= 49)  && (digits == 13 || digits == 16)) {printf("VISA\n");}
        else if ((ccPrefix == 34 || ccPrefix == 37) && digits == 15) {printf("AMEX\n");}
        else { printf("INVALID\n"); }
    }
    else { printf("INVALID\n"); }
}