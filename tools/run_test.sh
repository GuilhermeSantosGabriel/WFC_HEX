#!/bin/bash

cd "$(dirname "$0")/.."

TEST_INPUT=${1:-"base_perlin"}
RADIUS=${2:-45}

if [[ $TEST_INPUT == test_* ]]; then
    TEST_EXE="$TEST_INPUT"
else
    TEST_EXE="test_$TEST_INPUT"
fi

EXE_PATH="bin/$TEST_EXE"
OUTPUT_FILE="bin/output/${TEST_EXE}_output"

make tests || exit 1

if [ -f "$EXE_PATH" ]; then
    mkdir -p bin/output
    echo "Executing: $TEST_EXE (Radius: $RADIUS)"
    
    "$EXE_PATH" > "$OUTPUT_FILE" <<< "$RADIUS"

    python3 scripts/visualize_hexmap.py "$OUTPUT_FILE"
    python3 scripts/visualize_isometric_hexmap.py "$OUTPUT_FILE"
    
else
    echo "Error: The test '$TEST_EXE' doesn't exists in bin/."
    echo "Verify if it exists in src/tests/."
    exit 1
fi