#!/bin/bash

cd "$(dirname "$0")/.."

RADIUS=${1:-30}
RENDER_VAL=$2
STEP_VAL=$3
shift 3

RENDER_FLAG=""
if [ "$RENDER_VAL" = "true" ]; then
    RENDER_FLAG="--opengl-render"
fi

mkdir -p bin/output

echo "--- Compiling ---"
make -s

echo "--- Running WFC ---"
./bin/wfc --map-radius $RADIUS $RENDER_FLAG --opengl-step-counter $STEP_VAL "$@" > bin/output/wfc_output

echo "--- Visualizing (Python) ---"
python3 scripts/visualize_hexmap.py
python3 scripts/visualize_isometric_hexmap.py
