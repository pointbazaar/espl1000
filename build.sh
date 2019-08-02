#!/bin/bash


#compile David's code
cd Interpreter
g++ -o dri main.cpp gtree.cpp
cd .. 

#package because there we create the fatJar
mvn package -T 1C -DskipTests


