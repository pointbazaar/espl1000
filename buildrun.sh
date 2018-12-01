#!/bin/bash


mvn install

mvn exec:java -Dexec.mainClass="org.vanautrui.languages.App" -Dexec.args=test1.amanda

