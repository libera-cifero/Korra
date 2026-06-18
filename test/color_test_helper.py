#Script helps to generate test cases for color_test.cpp

import random
import colorsys
import string

COUNT = 10
BIT_RESOL = 6



def print_color_to_number_test_case(hex_color:int, bit_resolution:int):
    r = ((hex_color >> 16) & 0xFF) / 255.0
    g = ((hex_color >> 8) & 0xFF) / 255.0
    b = (hex_color & 0xFF) / 255.0

    h, s, v = colorsys.rgb_to_hsv(r, g, b)

    hue = h * 1530
    
    number = round(hue * (1 << bit_resolution) / 1530)

    print(f"{{{number}, 0x{hex_color:06x}, {bit_resolution}}},")

def gen_test_color_to_number():
    random_points = [ random.randint(0, 0xffffff) for _ in range(10)]

    for p in random_points:
        print_color_to_number_test_case(p, BIT_RESOL)

def print_number_to_color_test_case(number:int, bit_resolution:int):
    h = float(number) / (1 << bit_resolution)

    r, g, b = colorsys.hsv_to_rgb(h, 1, 1)

    r = int(round(r * 255))
    g = int(round(g * 255))
    b = int(round(b * 255))

    hex_color = (r << 16) | (g << 8) | b

    print(f"{{0x{hex_color:06x}, {number}, {bit_resolution}}},")

def gen_test_number_to_color():
    control_points = [0, 11, 21, 32, 43, 53, 63]
    around_control_points = [1, 10, 12, 20, 22, 31, 33, 40, 42, 52, 63]
    random_points = [ random.randint(0, (1 << BIT_RESOL) - 1) for _ in range(10)]

    for p in control_points:
        print_number_to_color_test_case(p, BIT_RESOL)
    print()
    for p in around_control_points:
        print_number_to_color_test_case(p, BIT_RESOL)
    print()
    for p in random_points:
        print_number_to_color_test_case(p, BIT_RESOL)

print("test_number_to_color")
gen_test_number_to_color()
print("\ntest_color_to_number")
gen_test_color_to_number()