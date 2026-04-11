import sys
import cv2
import numpy as np

COLOR_MAP = {
    0: (124, 255, 124), # GRASS
    1: (12, 87, 32),    # FOREST
    2: (255, 0, 0),     # WATER
    3: (0, 255, 255)    # SAND
}

class HexCell:

    x: int; y: int
    color: tuple[int, int, int]
    radius:int ; height: int

    def __init__(self, x: int, y: int, color: tuple[int, int, int], radius: int = 4, height: int = 1,):
        self.x = x; self.y = y
        self.color = color
        self.radius = radius; self.height = height

    def draw_isometric(self, map_image, offset_x=500, offset_y=200):

        iso_x = (self.x - self.y) + offset_x
        iso_y = (self.x + self.y) / 2 + offset_y
        
        h = self.height 
        w = self.radius * 2
        thick = 10 + h

        top_pts = np.array([
            [iso_x, iso_y - thick],
            [iso_x + w, iso_y - thick + w/2],
            [iso_x, iso_y - thick + w],
            [iso_x - w, iso_y - thick + w/2]
        ], np.int32)

        right_side = np.array([
            [iso_x, iso_y],
            [iso_x + w, iso_y + w/2],
            [iso_x + w, iso_y - thick + w/2],
            [iso_x, iso_y - thick]
        ], np.int32)

        color_top = self.color
        color_side = tuple([max(0, c - 40) for c in self.color])

        cv2.fillPoly(map_image, [right_side], color_side)
        cv2.fillPoly(map_image, [top_pts], color_top)

        cv2.polylines(map_image, [top_pts], True, (0, 0, 0), 1)

try:
    with open("bin/wfc_output", "r") as file:
        lines = file.readlines()
except Exception as e:
    print(f"Failed to open file: {e}")
    sys.exit(-1)

IMG_SIZE = (1000, 1000, 3)
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
            x, y, color, height=height
        ))

    except Exception:
        continue

hex_cell_map.sort(key=lambda cell: cell.x + cell.y)

for hex_cell in hex_cell_map:
    hex_cell.draw_isometric(map_image)

cv2.imwrite("isometric_hexmap.png", map_image)
