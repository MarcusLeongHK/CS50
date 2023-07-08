from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break

row = 0
for i in range(height):
        print(" " * (height - 1 - row) , end='')
        print("#" * (row + 1) , end=' ')
        print(end=' ')
        print("#" * (row + 1))
        row += 1