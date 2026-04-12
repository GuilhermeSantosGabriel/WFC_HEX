#!/bin/bash

RADIUS=${1:-45}

make
./bin/wfc > bin/wfc_output <<< "$RADIUS"
echo
python3 scripts/visualize_hexmap.py
python3 scripts/visualize_isometric_hexmap.py

echo
