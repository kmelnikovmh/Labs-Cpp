#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="$SCRIPT_DIR/../src"

clang++ -std=c++20 -o widgets_test $SCRIPT_DIR/*.cpp $SRC_DIR/*.cpp
COMMAND=("./widgets_test")

TIMEOUT=15s  
timeout -k 0.1s "$TIMEOUT" "${COMMAND[@]}"
