#!/bin/bash

#SBATCH --account=PMIU0184
#SBATCH --job-name=ex8
#SBATCH --time=00:15:00
#SBATCH --mem=2GB
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1

# Just recompiling to play it safe for this exercise.  Compiling in
# the script can be a negative or positive depending on the situation.

OPT_LEVEL="-O3"

g++ -g -Wall -std=c++17 ${OPT_LEVEL} Exercise7_sol.cpp -o Exercise8

echo "Running 3 experiments for switch at ${OPT_LEVEL}"
for rep in `seq 1 3`; do
    perf stat ./Exercise8 switch
done

echo "Running 3 experiments for if ${OPT_LEVEL}"
for rep in `seq 1 3`; do
    perf stat ./Exercise8 if
done

# Record detailed stats to review source code results
perf record -F 20 --call-graph dwarf -o "perf_switch.data" ./Exercise8 switch
perf record -F 20 --call-graph dwarf -o "perf_if.data" ./Exercise8 if

# End of script

