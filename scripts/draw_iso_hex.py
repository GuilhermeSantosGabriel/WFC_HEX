import cv2
import numpy as np

from models.HexCell import HexCell

ZOOM_FACTOR = 50

IMG_SIZE = (1000, 1000, 3)
map_image = np.zeros(IMG_SIZE, np.uint8)

hex_cell = HexCell(
    x=500, y=500, color=(0, 255, 0), radius=1*ZOOM_FACTOR, height=3*ZOOM_FACTOR
)

hex_cell.draw_isometric(map_image)

cv2.imwrite("isometric_hex_cell.png", map_image)
