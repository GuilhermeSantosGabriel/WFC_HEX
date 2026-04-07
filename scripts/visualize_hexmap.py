import sys
import cv2
import numpy as np

COLOR_MAP = {
    0: (124, 255, 124), # GRASS
    1: (0, 255, 0),     # FOREST
    2: (255, 0, 0),     # WATER
    3: (0, 255, 255)    # SAND
}

try:
    with open("bin/wfc_output", "r") as file:
        lines = file.readlines()
except Exception as e:
    print(f"Failed to open file: {e}")
    sys.exit(-1)

IMG_SIZE = (1000, 1000, 3)
map_image = np.zeros(IMG_SIZE, np.uint8)

try:
    hex_radius = int(input("Type Hex Radius: "))
except ValueError:
    hex_radius = 5

for line in lines:
    try:
        parts = line.split()
        if len(parts) < 3: continue
        
        x = int(float(parts[0]))
        y = int(float(parts[1]))
        tile = int(parts[2])

        color = COLOR_MAP.get(tile, (255, 255, 255))
        
        cv2.circle(map_image, (x, y), hex_radius, color, -1)
    except Exception:
        continue

cv2.imwrite("hexmap.png", map_image)
