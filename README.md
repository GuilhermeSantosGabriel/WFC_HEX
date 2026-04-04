# WFC_HEX

## Setup

```bash
# ==== Create virtual environment ====
python3 -m venv venv

# ==== Activate virtual environment ====
# Linux
source venv/bin/activate
# Windows
venv\Scripts\activate
```

## Execute

```bash
# ==== Compile ====
# Linux
make
# Windows
mingw32-make

# ==== Execute ====
# Linux
./hexmap > output
# Windows
./hexmap.exe > output

# ==== Visuzalize ====
python3 visualize_hexmap.py
```

---

## Useful Links

- https://www.redblobgames.com/grids/hexagons/#coordinates
- https://www.youtube.com/watch?v=rI_y2GAlQFM&t=564s
- https://blog.ptidej.net/procedural-generation-using-wave-function-collapse/
