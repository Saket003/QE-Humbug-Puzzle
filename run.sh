#!/bin/bash

# Check if the required number of arguments is passed
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <number_of_moves> <path_to_test_file>"
    exit 1
fi

# Extracting the arguments
numMoves=$1
testFilePath=$2

# Compiling the C++ program
g++ -o humbug humbug.cpp

# Running the compiled program with the provided arguments
./humbug $numMoves $testFilePath

# Keep the terminal open
exec bash