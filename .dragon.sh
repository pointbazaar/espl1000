#!/bin/bash

#executes the dragon compiler or interpreter


function dragon(){
	#echo "dragon launched. "
	#echo $1

	#https://www.devdungeon.com/content/taking-command-line-arguments-bash
	#https://stackoverflow.com/questions/456636/how-do-i-pass-parameters-to-a-jar-file-at-the-time-of-execution

	java -jar ~/dragon/target/dragon-0.1-jar-with-dependencies.jar "$@"
}

function dge(){
    #dragon editor
    dragon -e "$@"
}

function dgc(){
    #run dragon compiler
    dragon -c "$@"
}

function dg(){
    echo "to open project: "
	echo "dge (file* | directory) "
	echo ""
	echo "to open editor with empty buffer: "
	echo "dge"
	echo ""
	echo "to compile:"
	echo "dgc (file* | directory) "
}

function dri(){
	~/dragon/Interpreter/dri "$@"
}
