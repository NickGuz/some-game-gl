#!/bin/bash

original_directory=$(pwd)

if [[ ! -f "$original_directory/build/CMakeCache.txt" ]]; then
    cd build

    current_directory=$(pwd)
    if [[ "$current_directory" != "*build" ]]; then
        echo "Error: The current directory is not 'build'."
        exit 1
    fi

    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
    cd $original_directory
    cp lib/ikpMP3.so .
    exit 0
fi

cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
cd $original_directory
cmake --build build/
cp lib/ikpMP3.so .
