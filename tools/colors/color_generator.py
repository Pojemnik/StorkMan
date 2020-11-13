#A simple script for generating file with colors
#Arguments:
# - Step for color change (1-255)
import sys

if len(sys.argv) < 2:
    print('Error: Not enough arguments.')
    quit()
if len(sys.argv) > 3:
    print('Error: Too many arguments.')
    quit()
if sys.argv[1] < 1 or sys.argv[1] > 255:
    print('Error: Incorrect argument.')
    quit()
colors = []
top = 255
step = int(sys.argv[1])
for r in range(0, top, step):
    for g in range(0, top, step):
        for b in range(0, top, step):
            colors.append((str(r), str(g), str(b)))
with open('colors.txt', 'w') as f:
    for c in colors:
        f.write('{} {} {}\n'.format(c[0], c[1], c[2]))