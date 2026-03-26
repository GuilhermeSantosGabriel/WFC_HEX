import cv2
import numpy as np

IMG_SIZE = (1000, 1000, 3)

map_image = np.zeros(IMG_SIZE, np.uint8)

hex_radius = int(input())

file = open("output", "r")

for line in file.readlines():
    x, y = line.split()
    x = int(float(x))
    y = int(float(y))
    cv2.circle(map_image, (x, y), hex_radius, (0, 0, 255), -1)

cv2.imwrite("hexmap.png", map_image)

