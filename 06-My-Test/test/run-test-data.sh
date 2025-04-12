#!/bin/bash
set -euo pipefail

COMMAND=("./test_main")

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

TIMEOUT=15s  
FAIL=0

function test_ok {
    rm -f "$SCRIPT_DIR/$1/stderr.out"

    clang++ -std=c++20 -o test_main test_main.cpp $1/*.cpp

    timeout -k 0.1s "$TIMEOUT" "${COMMAND[@]}" &>"$SCRIPT_DIR/$1/stderr.out"

    if diff -q "$SCRIPT_DIR/$1/stderr.sol" "$SCRIPT_DIR/$1/stderr.out"; then
        CUR_OK=1
    else
        CUR_OK=0
    fi

    EXITCODE=$?
    case "$EXITCODE" in
        0)
            if [[ $CUR_OK == 1 ]]; then
              echo PASS
            else
              echo -e "\033[31;1mWA\033[0m"
              FAIL=1
            fi
            ;;
        *)
            if [[ $CUR_OK == 1 ]]; then
                echo -e "\033[31;1mUnexpected: non-zero exit code but passing diff\033[0m"
                FAIL=1
            else
                echo -e "\033[31;1mWA + RTE\033[0m (exit code $EXITCODE)"
                FAIL=1
            fi
            ;;
    esac
    return 0
}

for test_folder in "$SCRIPT_DIR"/*; do
    if [[ -d "$test_folder" ]]; then # Check if it's a directory
        test_folder=$(basename "$test_folder")
        echo ===== "$test_folder" =====
        if ! test_ok "$test_folder"; then break; fi
    fi
done

if [[ "$FAIL" == "0" ]]; then
    echo -e "===== \033[32;1mALL PASS\033[0m ====="
else
    echo -e "===== \033[31;1mSOME FAIL\033[0m ====="
fi
exit $FAIL