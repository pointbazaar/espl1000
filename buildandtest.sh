#!/bin/bash

#this may work:
#https://stackoverflow.com/questions/55303837/problem-running-tests-with-enabled-preview-features-in-surefire-and-failsafe
export MAVEN_OPTS='--enable-preview'

#package because there we create the fatJar
mvn package -T 1C


