#!/bin/bash

make

# bodytrack blackscholes fluidanimate
# ./build/coherence MESI bodytrack 1024 1 16 4

echo "===blackscholes==="

./build/coherence MESI blackscholes 1024 1 16 4
./build/coherence MOESI blackscholes 1024 1 16 4
./build/coherence Dragon blackscholes 1024 1 16 4

echo "===fluidanimate==="

./build/coherence MESI fluidanimate 1024 1 16 4
./build/coherence MOESI fluidanimate 1024 1 16 4
./build/coherence Dragon fluidanimate 1024 1 16 4

echo "===bodytrack==="

./build/coherence MESI bodytrack 1024 1 16 4
./build/coherence MOESI bodytrack 1024 1 16 4
./build/coherence Dragon bodytrack 1024 1 16 4

# for development
# ./build/coherence MESI test 32 2 8 2
