import sys

import cv2
import numpy as np

try:
    file = open("bin/wfc_output", "r")
except Exception as e:
    print(f"Failed to open 'bin/wfc_output': {e}")
    sys.exit(-1)

IMG_SIZE = (1000, 1000, 3)

map_image = np.zeros(IMG_SIZE, np.uint8)

hex_radius = int(input("Type Hex Radius: "))


#0 -> GRASS,
#1 -> FOREST,
#2 -> WATER,
#3 -> SAND


for line in file.readlines():
    x, y, tile = line.split()
    x = int(float(x))
    y = int(float(y))
    tile = int(tile)

    match tile:

        case 0:
            color = (124, 255, 124)
        case 1:
            color = (0, 255, 0)
        case 2:
            color = (255, 0, 0)
        case 3:
            color = (0, 255, 255)
        
    cv2.circle(map_image, (x, y), hex_radius, color, -1)

    cv2.imwrite("hexmap.png", map_image)
