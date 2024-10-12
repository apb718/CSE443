#!/bin/bash

#SBATCH --account=PMIU0184
#SBATCH --job-name=ex7
#SBATCH --time=00:05:00
#SBATCH --mem=2GB
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1

# Just recompiling to play it safe for this exercise.  Compiling in
# the script can be a negative or positive depending on the situation.

OPT_LEVEL="-O3"

g++ -g -Wall -std=c++17 ${OPT_LEVEL} main.cpp -o Exercise7

echo "Running 3 experiments for switch at ${OPT_LEVEL}"
for rep in `seq 1 3`; do
    /usr/bin/time -v ./Exercise7 switch
done

echo "Running 3 experiments for if ${OPT_LEVEL}"
for rep in `seq 1 3`; do
    /usr/bin/time -v ./Exercise7 if
done

# End of script
