# TODO
import cs50
import math
#include <stdio.h>
#include <math.h>

ccNum = int(input(("Insert credit card number please: ")))
oddNum = int(0)
evenNum = int(0)

# log10 of a number gives amount of times 10 has to be multiplied to achieve said number
# Adding 1 to that will give us the digit total. Luckily the math library has a log10() function.
digits = int(math.log10(ccNum) + 1)

# Step1 - get even positioned numbers, multiply by 2 and add together digits. Reset num at end.
num = int(ccNum); # used as proxy
while (num):
    num = int(num / 10)

    lastDigitMulti = int((num % 10) * 2)
    if (lastDigitMulti > 9): # if has 2+ digits subtract 9 to get both it's digits addition
        evenNum = int(evenNum + (lastDigitMulti - 9))
    else:
        evenNum = int(evenNum + lastDigitMulti)

    num = int(num / 10)

num = int(ccNum)


# Step 2 - get odd positioned numbers, check amount of digits and add digits together. Reset num
while(num):
    oddNum = int(oddNum + (num % 10))
    num = int(num / 10)
    num = int(num / 10)

num = int(ccNum)


# Step 3 - Credit Card Validation.
checksum = int((evenNum + oddNum) % 10) #gets final digit of checksum. Valid if 0
ccPrefix = int(num / math.pow(10, digits - 2))

if (checksum == 0 and (digits >= 13 and digits <= 16)):
    if (ccPrefix >= 51 and ccPrefix <=55) and digits == 16:
        print("MASTERCARD")
    elif (ccPrefix >= 40 and ccPrefix <= 49) and (digits == 13 or digits == 16):
        print("VISA")
    elif (ccPrefix == 34 or ccPrefix == 37) and digits == 15:
        print("AMEX")
    else:
        print("INVALID")
else:
    print("INVALID")
