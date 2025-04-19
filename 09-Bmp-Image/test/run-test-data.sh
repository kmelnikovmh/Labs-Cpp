#!/bin/bash
set -euo pipefail

COMMAND=("./bmp-image")

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

TIMEOUT=15s  
FAIL=0

function test_ok {
    rm -f "$SCRIPT_DIR/data-test/$1.out"

    ARGS=$(<"$SCRIPT_DIR/data-test/$1.in")
    echo $ARGS

    timeout -k 0.1s "$TIMEOUT" "${COMMAND[@]}" $ARGS

    EXITCODE=${PIPESTATUS[0]}
    case "$EXITCODE" in
    0)
        if cmp -n 1000000 -s "$SCRIPT_DIR/data-test/$1.sol.bmp" "$SCRIPT_DIR/data-test/$1.out.bmp"; then
            echo PASS
        else
            echo -e "\033[31;1mWA\033[0m"
            cmp -n 1000000 "$SCRIPT_DIR/data-test/$1.sol.bmp" "$SCRIPT_DIR/data-test/$1.out.bmp" | head -n 10
            FAIL=1
        fi
        ;;
    *)
        echo -e "\033[31;1mRTE\033[0m (exit code $EXITCODE)"
        FAIL=1
        return 1
        ;;
    esac
    return 0
}

for in_file in "$SCRIPT_DIR"/data-test/*.in; do
    test_name=$(basename "$in_file" .in)
    echo ===== "$test_name" =====
    if ! test_ok "$test_name"; then break; fi
done

if [[ "$FAIL" == "0" ]]; then
    echo -e "===== \033[32;1mALL PASS\033[0m ====="
else
    echo -e "===== \033[31;1mSOME FAIL\033[0m ====="
fi
exit $FAIL
