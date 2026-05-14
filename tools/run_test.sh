#!/bin/bash

cd "$(dirname "$0")/.."

TEST_INPUT=${1:-"base_perlin"}
shift

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
    echo "--- Executing Test: $TEST_EXE ---"

    "$EXE_PATH" "$@" > "$OUTPUT_FILE"

    if [ ${PIPESTATUS[0]} -eq 0 ]; then
        echo "--- Generating Visuals ---"
        python3 scripts/visualize_hexmap.py "$OUTPUT_FILE"
        python3 scripts/visualize_isometric_hexmap.py "$OUTPUT_FILE"
    fi
else
    echo "Error: The test '$TEST_EXE' doesn't exist in bin/."
    exit 1
fi
