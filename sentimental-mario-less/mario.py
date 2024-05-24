# TODO
from cs50 import get_int
import sys

height = -1

while height not in range(1, 9):
    height = get_int("Height: ")


spaces = height - 1
hashes = 1
for h in range(height):
    for i in range(spaces):
        sys.stdout.write(" ")
    for k in range(hashes):
        sys.stdout.write("#")

    spaces -= 1
    hashes += 1
    print()