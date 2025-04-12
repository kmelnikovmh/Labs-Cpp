#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

COMMAND_BANK=("$SCRIPT_DIR/bank-test")
COMMAND_SERVER=("$SCRIPT_DIR/server-test")

# start server

TIMEOUT=15s  
timeout -k 0.1s "$TIMEOUT" "${COMMAND_BANK[@]}"
# timeout -k 0.1s "$TIMEOUT" "${COMMAND_SERVER[@]}"
