def get_r(color): 
    return (color & 0xff0000) >> 16
def get_g(color): 
    return (color & 0x00ff00) >> 8
def get_b(color): 
    return color & 0x0000ff

def all_colors():
    yield from range(0x1000000)  # от 0x000000 до 0xFFFFFF

def distance(color0, color1):
    r0 = get_r(color0)
    g0 = get_g(color0)
    b0 = get_b(color0)

    r1 = get_r(color1)
    g1 = get_g(color1)
    b1 = get_b(color1)

    return (r1 - r0)**2 + (g1 - g0)**2 + (b1 - b0)**2
selected = [ 0xffffff ]
while len(selected) < 16:
    best = None
    best_dist = -1
for candidate in all_colors():
    d = min(distance(candidate, c) for c in selected)
    if d > best_dist:
        best_dist = d
    best = candidate
    selected.append(best)
print(selected)