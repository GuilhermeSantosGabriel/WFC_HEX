import sys
import cv2
import numpy as np
import os

from models.HexCell import HexCell, COLOR_MAP

try:
    if len(sys.argv) > 1: file_path = sys.argv[1]
    else: file_path = "bin/output/wfc_output"

    with open(file_path, "r") as file:
        lines = file.readlines()
except Exception as e:
    print(f"Erro ao abrir arquivo: {e}")
    sys.exit(-1)

num_cells = len(lines)
NORMALIZATION_FACTOR = max(1.5, min(10, 400 / (num_cells ** 0.5))) if num_cells > 0 else 5

raw_data = []
for line in lines:
    parts = line.split()
    if len(parts) < 4: continue
    raw_data.append([float(parts[0]), float(parts[1]), float(parts[2]), int(parts[3])])

if not raw_data:
    print("No data to render")
    sys.exit(0)

final_coords_x = []
final_coords_y = []

for d in raw_data:
    nx = d[0] * NORMALIZATION_FACTOR
    ny = d[1] * NORMALIZATION_FACTOR
    nh = d[2] * NORMALIZATION_FACTOR
    
    iso_x = (nx - ny)
    iso_y = (nx + ny) / 2 - nh
    
    final_coords_x.append(iso_x)
    final_coords_y.append(iso_y)

min_f_x, max_f_x = min(final_coords_x), max(final_coords_x)
min_f_y, max_f_y = min(final_coords_y), max(final_coords_y)

PADDING = 15 * NORMALIZATION_FACTOR 

width = int(max_f_x - min_f_x + 2 * PADDING)
height = int(max_f_y - min_f_y + 2 * PADDING)

map_image = np.zeros((height, width, 3), np.uint8)

hex_cell_map: list[HexCell] = []
for d in raw_data:
    x_pos, y_pos, h_pos, tile = d
    color = COLOR_MAP.get(tile, (255, 255, 255))
    
    hex_cell_map.append(HexCell(
        x_pos * NORMALIZATION_FACTOR, 
        y_pos * NORMALIZATION_FACTOR, 
        color,
        height = h_pos * NORMALIZATION_FACTOR, 
        radius = 7.5 * NORMALIZATION_FACTOR
    ))

hex_cell_map.sort(key=lambda cell: cell.x + cell.y)

for hex_cell in hex_cell_map:
    hex_cell.draw_isometric(
        map_image,
        offset_x = -min_f_x + PADDING,
        offset_y = -min_f_y + PADDING
    )

os.makedirs("img", exist_ok=True)
if len(sys.argv) > 1:
    os.makedirs("img/test", exist_ok=True)
    base_name = os.path.basename(sys.argv[1]).replace("_output", "")
    file_name = f"img/test/isometric_{base_name}.png"
else:
    file_name = "img/isometric_hexmap.png"

cv2.imwrite(file_name, map_image)
