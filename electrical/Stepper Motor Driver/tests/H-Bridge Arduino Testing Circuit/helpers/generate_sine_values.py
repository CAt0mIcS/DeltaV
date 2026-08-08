from math import sin, cos, pi, floor


def clamp(data: float) -> int:
    return floor(data * 127)

sin_vals = []

for i in range(0, 32):
    sin_vals.append(clamp(sin(i / 124 * 2 * pi)))

for i in range(0, 32):
    print(f"{sin_vals[i]}, ", end='')

for i in range(30, -1, -1):
    print(f"{sin_vals[i]}, ", end='')

for i in range(1, 31):
    print(f"{-sin_vals[i]}, ", end='')

for i in range(31, -1, -1):
    print(f"{-sin_vals[i]}, ", end='')


print("\n\n")


for i in range(31, -1, -1):
    print(f"{sin_vals[i]}, ", end='')

for i in range(1, 31):
    print(f"{-sin_vals[i]}, ", end='')

for i in range(31, 1, -1):
    print(f"{-sin_vals[i]}, ", end='')

for i in range(0, 32):
    print(f"{sin_vals[i]}, ", end='')

