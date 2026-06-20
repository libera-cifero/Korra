rows = 10
groups_per_row = 12
group_size = 3

for _ in range(rows):
    group = "0," * group_size  # "0,0,0,"
    line = "  ".join([group] * groups_per_row)
    print(line)