#!/bin/bash

make
./bin/wfc > bin/wfc_output <<< "45"
echo
python3 scripts/visualize_hexmap.py <<< "4"
python3 scripts/visualize_isometric_hexmap.py

echo
