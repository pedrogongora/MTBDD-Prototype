#!/bin/bash

X=$(ls ../models/n*.eqn)

for i in $X
do
# verificamos que $i sea un archivo y no un directorio
if [ -f $i ]; then
    (/usr/bin/time -f "real:\t%es\nsys:\t%Ss\nsize:\t%Mkb" ./MTBDD-Proto -ns $i) >> log.txt 2>&1
fi
done

