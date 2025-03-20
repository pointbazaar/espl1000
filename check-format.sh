#!/bin/bash

set -eux

./format-code.sh

format_exit_code=$?

if [ $format_exit_code -ne 0 ]; then
    exit 1
fi

dirty=$(git ls-files --modified)

if [ "$dirty" ]; then
	echo "check-format has failed"
	echo "$dirty"

	echo "diff:"
	diff=$(git diff)
	echo "$diff"

	exit 1
fi
