#!/bin/bash

cd "$(dirname "$0")/.."

make tests || exit 1

mkdir -p bin/output

for test_exe in bin/test_*; do
    if [ -x "$test_exe" ]; then
        TEST_NAME=$(basename "$test_exe")
        OUTPUT_FILE="bin/output/${TEST_NAME}_output"

        echo "------------------------------------------------"
        echo "Testing: $TEST_NAME with args: $@"

        "$test_exe" "$@" > "$OUTPUT_FILE"

        python3 scripts/visualize_hexmap.py "$OUTPUT_FILE"
        python3 scripts/visualize_isometric_hexmap.py "$OUTPUT_FILE"
    fi
done

echo "------------------------------------------------"
echo "All tests were done! yay"
