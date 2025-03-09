#!/bin/sh

# Counter for results
PASS=0
FAIL=0

echo ""
echo "Running example programs (assert exit code)"

for test_file in $(find . -name '*.dg'); do
	expected_exit_file="${test_file%.*}.exitcode"

	if [[ ! -f "$expected_exit_file" ]]; then
		# echo "Missing expected exit code file for $test_file"
		# echo ${expected_exit_file}
		# FAIL=$((FAIL + 1))
		continue
	fi

	expected_exit=$(cat "$expected_exit_file")

	sd -x86 -o /tmp/program "$test_file"
	if [[ $? -ne 0 ]]; then
		echo "Compile failed for $test_file"
		FAIL=$((FAIL + 1))
		continue
	fi

	/tmp/program > /dev/null
	actual_exit=$?

	if [[ "$actual_exit" -eq "$expected_exit" ]]; then
		echo "[PASS][EXITCODE] $test_file"
		PASS=$((PASS + 1))
	else
		echo "[FAIL][EXITCODE] $test_file (expected: $expected_exit, got: $actual_exit)"
		FAIL=$((FAIL + 1))
	fi
done

echo "Running example programs (assert stdout)"

for test_file in $(find . -name '*.dg'); do
	expected_stdout_file="${test_file%.*}.stdout"

	if [[ ! -f "$expected_stdout_file" ]]; then
		continue
	fi

	sd -x86 -o /tmp/program "$test_file"
	if [[ $? -ne 0 ]]; then
		echo "Compile failed for $test_file"
		FAIL=$((FAIL + 1))
		continue
	fi

	/tmp/program > /tmp/program_stdout

	diff --ignore-trailing-space /tmp/program_stdout "$expected_stdout_file"
	stdout_res=$?
	if [[ stdout_res -eq 0 ]]; then
		echo "[PASS][STDOUT] $test_file"
		PASS=$((PASS + 1))
	else
		echo "[FAIL][STDOUT] $test_file"
		FAIL=$((FAIL + 1))
	fi
done

echo
echo "Tests completed: $PASS passed, $FAIL failed"
exit $FAIL

