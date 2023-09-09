SCRIPT_DIR=$(realpath $(dirname $0))

echo "running from" $SCRIPT_DIR

SRC=$(find $SCRIPT_DIR -type d \( -name 'build' -o -name 'dependencies' \) -prune -o \( -name '*.c' -o -name '*.h' \) -print)

#SRC=$(find rat token -type f \( -name "*.c" -o -name "*.h" \))

#echo $SRC

clang-format -i --style='file:.clang-format' $SRC
