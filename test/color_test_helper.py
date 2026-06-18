import random
import colorsys
import string

COUNT = 10
BIT_RESOL = 6

def print_test_case(number:int, bit_resolution:int):
    h = float(number) / (1 << bit_resolution)

    s = random.random()          # random Saturation [0;1]
    v = random.random()          # random Value [0;1]

    r, g, b = colorsys.hsv_to_rgb(h, s, v)

    r = int(round(r * 255))
    g = int(round(g * 255))
    b = int(round(b * 255))

    hex_color = (r << 16) | (g << 8) | b

    print(f"{{0x{hex_color:06x}, {number}, {bit_resolution}}},")

control_points = [0, 11, 21, 32, 43, 53, 63]
around_control_points = [1, 10, 12, 20, 22, 31, 33, 40, 42, 52, 63]
random_points = [ random.randint(0, (1 << BIT_RESOL) - 1) for _ in range(10)]

for p in control_points:
    print_test_case(p, BIT_RESOL)
print()
for p in around_control_points:
    print_test_case(p, BIT_RESOL)
print()
for p in random_points:
    print_test_case(p, BIT_RESOL)