import math
import numpy as np
import cv2

class HexCell:

    x: int; y: int
    color: tuple[int, int, int]
    radius:int ; height: int

    def __init__(self, x: int, y: int, color: tuple[int, int, int], radius: int = 4, height: int = 1,):
        self.x = x; self.y = y
        self.color = color
        self.radius = radius; self.height = height

    def draw_isometric(self, map_image, offset_x=500, offset_y=200):

        # Converts grid coordinates to isometric screen space
        # We use the standard 2:1 ratio (width:height) to get the isometric look
        iso_x = (self.x - self.y) + offset_x
        iso_y = (self.x + self.y) / 2 + offset_y
        
        r = self.radius 
        thick = self.height  # This determines how "tall" the pillar is

        # Generates the 6 points for the Top Face (Hexagon)
        # We loop every 60° to get the vertices. 
        # Adding 30° at the start ensures a "pointy" end faces down.
        top_pts = []
        for i in range(6):
            angle_rad = math.radians(60 * i + 30)
            px = iso_x + r * math.cos(angle_rad)
            # We divide Y by 2 to "squish" the circle into an isometric oval
            py = iso_y - thick + (r * math.sin(angle_rad) / 2)
            top_pts.append([px, py])
        
        top_pts = np.array(top_pts, np.int32)

        # Projects the Bottom Points
        # The base is just the top points shifted down by our 'thick' value
        bot_pts = np.array([[p[0], p[1] + thick] for p in top_pts], np.int32)

        # Defines the 3 visible side faces
        # Each side is a quad connecting two top vertices to their bottom counterparts
        # Order: Front-Left, Front-Center, and Front-Right
        face_left   = np.array([top_pts[1], top_pts[2], bot_pts[2], bot_pts[1]], np.int32)
        face_center = np.array([top_pts[0], top_pts[1], bot_pts[1], bot_pts[0]], np.int32)
        face_right  = np.array([top_pts[5], top_pts[0], bot_pts[0], bot_pts[5]], np.int32)

        # Creates a sense of depth with shading
        color_top    = tuple([max(0, c - 30) for c in self.color])
        color_main   = self.color
        color_shadow = tuple([max(0, c - 50) for c in self.color])
        color_dark   = tuple([max(0, c - 80) for c in self.color])

        # Rendering - Order matters!!
        cv2.fillPoly(map_image, [face_left], color_dark)
        cv2.fillPoly(map_image, [face_center], color_main)
        cv2.fillPoly(map_image, [face_right], color_shadow)
        cv2.fillPoly(map_image, [top_pts], color_top)

        # Adds a thin outline to the top to make the edges pop
        cv2.polylines(map_image, [top_pts], True, (0, 0, 0), 1)
