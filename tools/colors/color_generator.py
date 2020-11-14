#A simple script for generating file with colors
#Arguments:
# - Step for color change (1-255)
import sys
import random

if len(sys.argv) < 2:
    print('Error: Not enough arguments.')
    quit()
if len(sys.argv) > 3:
    print('Error: Too many arguments.')
    quit()
step = int(sys.argv[1])
if step < 1 or step > 255:
    print('Error: Incorrect argument.')
    quit()
colors = []
top = 255
for r in range(0, top, step):
    for g in range(0, top, step):
        for b in range(0, top, step):
            colors.append((str(r), str(g), str(b)))
random.shuffle(colors)
with open('colors.txt', 'w') as f:
    for c in colors:
        f.write('{} {} {}\n'.format(c[0], c[1], c[2]))