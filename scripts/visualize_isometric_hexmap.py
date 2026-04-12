import sys
import cv2
import numpy as np

from models.HexCell import HexCell

COLOR_MAP = {
    0: (124, 255, 124), # GRASS
    1: (12, 87, 32),    # FOREST
    2: (255, 0, 0),     # WATER
    3: (0, 255, 255)    # SAND
}

try:
    with open("bin/wfc_output", "r") as file:
        lines = file.readlines()
except Exception as e:
    print(f"Failed to open file: {e}")
    sys.exit(-1)

NORMALIZATION_FACTOR = 5

IMG_SIZE = (1000 * NORMALIZATION_FACTOR, 1000 * NORMALIZATION_FACTOR, 3)
map_image = np.zeros(IMG_SIZE, np.uint8)

hex_cell_map: list[HexCell] = []

for line in lines:
    try:
        parts = line.split()
        if len(parts) < 4: continue
        
        x = int(float(parts[0]))
        y = int(float(parts[1]))
        height = int (float(parts[2]))
        tile = int(parts[3])

        color = COLOR_MAP.get(tile, (255, 255, 255))

        hex_cell_map.append(HexCell(
            x*NORMALIZATION_FACTOR, y*NORMALIZATION_FACTOR, color,
            height=height*NORMALIZATION_FACTOR, radius=7.5*NORMALIZATION_FACTOR
        ))

    except Exception:
        continue

hex_cell_map.sort(key=lambda cell: cell.x + cell.y)

for hex_cell in hex_cell_map:
    hex_cell.draw_isometric(map_image, offset_x=500*NORMALIZATION_FACTOR, offset_y=100*NORMALIZATION_FACTOR)

cv2.imwrite("isometric_hexmap.png", map_image)
