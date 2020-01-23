#!/bin/bash

#package because there we create the fatJar
mvn package -T 1C -DgeneratePom=true

#copy the executable to /usr/bin
gcc -o dragon-parser dragon-parser.c
cp dragon-parser /usr/bin/

dragon-parser -test