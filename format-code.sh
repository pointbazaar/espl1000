#!/bin/bash

set -eux

SCRIPT_DIR=$(realpath $(dirname $0))

echo "running from" $SCRIPT_DIR

SRC=$(find $SCRIPT_DIR -type d \( -name 'build' -o -name 'dependencies' -o -name 'stdlib' -o -name 'examples' \) -prune -o \( -name '*.c' -o -name '*.h' \) -print)

echo $SRC

clang-format -i --style='file:.clang-format' $SRC
