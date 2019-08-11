#!/bin/bash

#executes the dragon compiler or interpreter


function dragon(){
	#echo $1

	#https://www.devdungeon.com/content/taking-command-line-arguments-bash
	#https://stackoverflow.com/questions/456636/how-do-i-pass-parameters-to-a-jar-file-at-the-time-of-execution

	java -jar ~/dragon/target/dragon-0.1-jar-with-dependencies.jar "$@"
}

function dge-debug(){
    #dragon editor, with terminal output,
    #to debug it
    echo "starting dragon editor in debug mode"
    dragon -e "$@"
}

function dge(){
    #dragon editor without terminal output
    echo "starting dragon editor"
    dragon -e "$@" </dev/null &>/dev/null &
}

function dgc(){
    #run dragon compiler
    dragon -c "$@"
}

#function dg(){
#	dragon "$@"
#}

#function dri(){
#	~/dragon/Interpreter/dri "$@"
#}
