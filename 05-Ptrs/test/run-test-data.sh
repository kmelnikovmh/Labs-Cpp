#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

clang++ -std=c++20 -o ptr_test $SCRIPT_DIR/*.cpp
COMMAND=("./ptr_test")

TIMEOUT=15s  
timeout -k 0.1s "$TIMEOUT" "${COMMAND[@]}"
