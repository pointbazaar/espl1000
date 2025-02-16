#!/bin/bash

# Counter for results
PASS=0
FAIL=0

echo ""
echo "running example programs (assert exit code)"

for test_file in $(find . -name '*.dg'); do
	expected_exit_file="${test_file%.*}.exitcode"

	if [ ! -f "$expected_exit_file" ]; then
		# echo "Missing expected exit code file for $test_file"
		# echo ${expected_exit_file}
		# FAIL=$((FAIL + 1))
		continue
	fi

	expected_exit=$(cat "$expected_exit_file")

	# Compile the test
	sd -x86 -o /tmp/program "$test_file"
	if [ $? -ne 0 ]; then
		echo "Compile failed for $test_file"
		FAIL=$((FAIL + 1))
		continue
	fi

	# Run the compiled program
	/tmp/program
	actual_exit=$?

	# Compare results
	if [ "$actual_exit" -eq "$expected_exit" ]; then
		echo "[PASS] $test_file"
		PASS=$((PASS + 1))
	else
		echo "[FAIL] $test_file (expected: $expected_exit, got: $actual_exit)"
		FAIL=$((FAIL + 1))
	fi
done

echo
echo "Tests completed: $PASS passed, $FAIL failed"
exit $FAIL

