#!/bin/bash

original_directory=$(pwd)

if [[ -f "$original_directory/build/CMakeCache.txt" ]]; then
    cd build

    current_directory=$(pwd)
    if [[ "$current_directory" != "*build" ]]; then
        echo "Error: The current directory is not 'build'."
        exit 1
    fi

    cmake ..
    cd $original_directory
    cp lib/*.dll build/Debug/
    exit 0
fi

cmake --build build/
cp lib/*.dll build/Debug/
