#!/bin/sh

# Counter for results
export PASS=0
export FAIL=0

echo ""
echo "Running example programs (assert exit code)"

files=$(find . -name '*.dg')

for test_file in $files; do
	expected_exit_file="${test_file%.*}.exitcode"

	if [ ! -f "$expected_exit_file" ]; then
		# Missing exit code file for $test_file
		continue
	fi

	STDLIB=""
	use_stdlib_file="${test_file%.*}.stdlib"
	if [ -f "$use_stdlib_file" ]; then
		STDLIB="-stdlib"
	fi

	expected_exit=$(cat "$expected_exit_file")

	sd -x86 ${STDLIB} -o /tmp/program "$test_file"
	# shellcheck disable=SC2181
	if [ $? -ne 0 ]; then
		echo "Compile failed for $test_file"
		export FAIL=$((FAIL + 1))
		continue
	fi

	/tmp/program > /dev/null
	actual_exit=$?

	if [ "$actual_exit" -eq "$expected_exit" ]; then
		echo "[PASS][EXITCODE] $test_file"
		export PASS=$((PASS + 1))
	else
		echo "[FAIL][EXITCODE] $test_file (expected: $expected_exit, got: $actual_exit)"
		export FAIL=$((FAIL + 1))
	fi
done

echo "Running example programs (assert stdout)"

for test_file in $files; do
	expected_stdout_file="${test_file%.*}.stdout"

	if [ ! -f "$expected_stdout_file" ]; then
		continue
	fi

	STDLIB=""
	use_stdlib_file="${test_file%.*}.stdlib"
	if [ -f "$use_stdlib_file" ]; then
		STDLIB="-stdlib"
	fi

	sd -x86 ${STDLIB} -o /tmp/program "$test_file"
	# shellcheck disable=SC2181
	if [ $? -ne 0 ]; then
		echo "Compile failed for $test_file"
		export FAIL=$((FAIL + 1))
		continue
	fi

	/tmp/program > /tmp/program_stdout

	diff /tmp/program_stdout "$expected_stdout_file"
	stdout_res=$?
	if [ ${stdout_res} -eq 0 ]; then
		echo "[PASS][STDOUT] $test_file"
		export PASS=$((PASS + 1))
	else
		echo "[FAIL][STDOUT] $test_file"
		export FAIL=$((FAIL + 1))
	fi
done

echo
echo "Tests completed: $PASS passed, $FAIL failed"
exit $FAIL

