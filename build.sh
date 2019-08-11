#!/bin/bash


#compile David's code
#cd Interpreter
#g++ -o dri main.cpp gtree.cpp
#cd .. 

#cd CodeRenderer
#g++ -o crend main.cpp pixmap.cpp renderer.cpp
#cd ..

#package because there we create the fatJar
mvn package -T 1C -DskipTests


