#!/bin/bash
set -euo pipefail

COMMAND=("./primers")

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

TIMEOUT=15s  
FAIL=0

function test_ok {
    rm -f "$SCRIPT_DIR/data-test/$1.out"

    timeout -k 0.1s "$TIMEOUT" "${COMMAND[@]}" <"$SCRIPT_DIR/data-test/$1.in" | head -c 1000000 >"$SCRIPT_DIR/data-test/$1.out"

    EXITCODE=${PIPESTATUS[0]}
    case "$EXITCODE" in
    0)
        CUR_OK=0
        diff "$SCRIPT_DIR/data-test/$1.sol" "$SCRIPT_DIR/data-test/$1.out" && CUR_OK=1
        if [[ $CUR_OK == 1 ]]; then
          echo PASS
        else
          echo -e "\033[31;1mWA\033[0m"
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