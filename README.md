# WFC_HEX

## Setup

```bash

# ==== Installs source dependencies ====
sudo apt install libglfw3-dev libgl1-mesa-dev libx11-dev libxi-dev libxrandr-dev libxinerama-dev libxcursor-dev libglapi-mesa -y

# ==== Create virtual environment ====
python3 -m venv venv

# ==== Activate virtual environment ====
source venv/bin/activate

# ==== Download Python Libs ====
pip install -r requirements.txt
```

## Execute

```bash
# ==== Compile ====
make

# ==== Execute ====
./bin/wfc > bin/wfc_output

# ==== Visuzalize ====
python3 scripts/visualize_hexmap.py
```

---

## Useful Links

- https://www.redblobgames.com/grids/hexagons/#coordinates
- https://www.youtube.com/watch?v=rI_y2GAlQFM&t=564s
- https://blog.ptidej.net/procedural-generation-using-wave-function-collapse/
- [How does procedural generation work? | Bitwise - Youtube](https://youtu.be/-POwgollFeY?si=I9FvStpswLwmnykC)
