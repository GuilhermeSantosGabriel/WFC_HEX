# WFC_HEX

## Setup

```bash
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
