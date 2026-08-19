from math import sin, cos, pi, floor

microstep_interval = 32

def clamp(data: float) -> int:
    return floor(data * (microstep_interval * 4 - 1))

sin_vals = []
cos_vals = []

for i in range(0, microstep_interval * 4):
    sin_vals.append(clamp(sin(i / (microstep_interval * 4) * 2 * pi)))
    cos_vals.append(clamp(cos(i / (microstep_interval * 4) * 2 * pi)))

for i in range(0, microstep_interval * 4):
    print(f"{sin_vals[i]}, ", end='')

print("\n\n")

for i in range(0, microstep_interval * 4):
    print(f"{cos_vals[i]}, ", end='')