#!/bin/bash

cd "$(dirname "$0")/.."

RADIUS=${1:-45}

make
mkdir -p bin/output

./bin/wfc > bin/output/wfc_output <<< "$RADIUS"
echo
python3 scripts/visualize_hexmap.py
python3 scripts/visualize_isometric_hexmap.py

echo
