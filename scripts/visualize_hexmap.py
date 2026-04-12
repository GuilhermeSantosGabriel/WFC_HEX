import sys
import cv2
import numpy as np
import math
import os

from models.HexCell import COLOR_MAP

def draw_hexagon(img, center, size, color, border_thickness):

    points = []
    for i in range(6):
        angle_rad = math.radians(60 * i)
        px = center[0] + size * math.cos(angle_rad)
        py = center[1] + size * math.sin(angle_rad)
        points.append([px, py])
    
    pts = np.array(points, np.int32)

    cv2.fillPoly(img, [pts], color)

    if border_thickness > 0:
        cv2.polylines(img, [pts], isClosed=True, color=(0, 0, 0), thickness=border_thickness)

try:
    if len(sys.argv) > 1: file_path = sys.argv[1]
    else: file_path = "bin/output/wfc_output"

    with open(file_path, "r") as file:
        lines = file.readlines()
except Exception as e:
    print(f"Erro ao abrir arquivo: {e}")
    sys.exit(-1)

raw_data = []
for line in lines:
    parts = line.split()
    if len(parts) < 4: continue
    raw_data.append([float(parts[0]), float(parts[1]), int(parts[3])])

if not raw_data:
    sys.exit(0)

coords_x = [d[0] for d in raw_data]
coords_y = [d[1] for d in raw_data]
min_x, max_x = min(coords_x), max(coords_x)
min_y, max_y = min(coords_y), max(coords_y)

TARGET_RES = 2000
width_raw = max_x - min_x
height_raw = max_y - min_y
SCALE = TARGET_RES / max(width_raw, height_raw) if max(width_raw, height_raw) > 0 else 10

def get_unit_dist(data):
    sample = data[:100]
    min_d = float('inf')
    for i in range(len(sample)):
        for j in range(i+1, len(sample)):
            d = math.sqrt((sample[i][0]-sample[j][0])**2 + (sample[i][1]-sample[j][1])**2)
            if d > 0.1: min_d = min(min_d, d)
    return min_d

dist_centros = get_unit_dist(raw_data) * SCALE
hex_size = dist_centros / math.sqrt(3)

if hex_size > 5:
    border_thickness = max(1, int(hex_size * 0.05))
else:
    border_thickness = 0 

PADDING = int(hex_size * 2)
img_w = int(width_raw * SCALE) + 2 * PADDING
img_h = int(height_raw * SCALE) + 2 * PADDING
map_image = np.zeros((img_h, img_w, 3), np.uint8)

for d in raw_data:
    cx = int((d[0] - min_x) * SCALE) + PADDING
    cy = int((d[1] - min_y) * SCALE) + PADDING
    color = COLOR_MAP.get(d[2], (255, 255, 255))
    
    draw_hexagon(map_image, (cx, cy), hex_size, color, border_thickness)

os.makedirs("img", exist_ok=True)
if len(sys.argv) > 1:
    os.makedirs("img/test", exist_ok=True)
    base_name = os.path.basename(sys.argv[1]).replace("_output", "")
    file_name = f"img/test/{base_name}.png"
else:
    file_name = "img/hexmap.png"

cv2.imwrite(file_name, map_image)
