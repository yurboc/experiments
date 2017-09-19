#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd ${DIR}

echo "Running from $( pwd )"

echo "===== Remove build directory"
rm -rf ./build/

echo "===== Create build directory"
mkdir build

echo "===== Run cmake in build directory"
cd build
cmake ..

echo "===== Run make in build directory"
make

echo "===== Run application"
./elevator_exec
