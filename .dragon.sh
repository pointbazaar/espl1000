#!/bin/bash

#executes the dragon compiler or interpreter

function dragon(){
	#echo "dragon launched. "
	#echo $1


	#https://www.devdungeon.com/content/taking-command-line-arguments-bash
	#https://stackoverflow.com/questions/456636/how-do-i-pass-parameters-to-a-jar-file-at-the-time-of-execution

	java -jar ~/dragon/target/dragon-0.1-jar-with-dependencies.jar "$@"
}

function dg(){
	dragon "$@"
}
