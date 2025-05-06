#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

MAIN_TEST=("$SCRIPT_DIR/tape-sort-test")

# ...

TIMEOUT=15s  
timeout -k 0.1s "$TIMEOUT" "${MAIN_TEST[@]}"
