#!/bin/bash

python3 ./tools/dependencies/deps.py

dot -Tpng -Gsmoothing=graph -Gdpi=150 ./tools/dependencies/uml_com_main.dot -o /tmp/uml_temp.png

echo "Abrindo o grafo UML... Pressione 'Q' ou 'ESC' para fechar."
feh -. -g 1280x720 --zoom fill /tmp/uml_temp.png
