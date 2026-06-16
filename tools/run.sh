#!/bin/bash

cd "$(dirname "$0")/.."

MENU_VAL=$1
RADIUS=$2
RENDER_VAL=$3
STEP_VAL=$4
shift 4

MENU_FLAG=""
if [ "$MENU_VAL" = "true" ]; then
    MENU_FLAG="--open-menu"
fi

RENDER_FLAG=""
if [ "$RENDER_VAL" = "true" ]; then
    RENDER_FLAG="--opengl-render"
fi

RADIUS_FLAG=""
if [ -n "$RADIUS" ]; then
    RADIUS_FLAG="--map-radius $RADIUS"
fi

STEP_FLAG=""
if [ -n "$STEP_VAL" ]; then
    STEP_FLAG="--opengl-step-counter $STEP_VAL"
fi

mkdir -p bin/output

echo "--- Compiling ---"
make -s

echo "--- Running WFC ---"
./bin/wfc $MENU_FLAG $RENDER_FLAG $RADIUS_FLAG $STEP_FLAG "$@" > bin/output/wfc_output

echo "--- Visualizing (Python) ---"
python3 scripts/visualize_hexmap.py
python3 scripts/visualize_isometric_hexmap.py