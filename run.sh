#!/bin/bash
set -e

if [ -z "$1" ]; then
    echo "Usage: ./run.sh <COMMAND> [ARGS]"
    exit 0
fi

make -s
./build/$1 ${@:2}
