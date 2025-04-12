#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

clang++ -std=c++20 -o bigint_test $SCRIPT_DIR/test_main.cpp
COMMAND=("./bigint_test")

TIMEOUT=15s  
timeout -k 0.1s "$TIMEOUT" "${COMMAND[@]}"
