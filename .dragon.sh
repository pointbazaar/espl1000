#!/bin/bash

#executes the dragon compiler or interpreter

function dragon(){
	echo "dragon launched. "
	echo $1
	java -jar ~/dragon/target/dragon-0.1.jar
}
