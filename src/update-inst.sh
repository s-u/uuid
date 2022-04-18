#!/bin/bash

a=$(dirname $0)
if [ -z "$a" ]; then a=.; fi
src=`(cd $a && pwd)`
if [ -z "$src" ]; then src=`pwd`; fi

if [ ! -e $src/uuid.h ]; then
    echo "ERROR: cannot determine src directory. If in doubt, run from it." >&2
    exit 1
fi

cd "$src/.."

echo "Processing uuid package in `pwd`"

if [ ! -e DESCRIPTION ]; then
    echo "ERROR: invalid package structure, cannot find $src/../DESCRIPTION" >&2
    exit 1
fi

if [ ! -e inst/include ]; then
    echo Creating inst/include
    mkdir -p inst/include
fi

echo Copying LinkingTo header files
cp -p src/*uuid.h inst/include/

echo Done
