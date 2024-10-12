#!/bin/bash

#SBATCH --account=PMIU0184
#SBATCH --job-name=ex9
#SBATCH --time=00:30:00
#SBATCH --mem=2GB
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1

# Just recompiling to play it safe for this exercise.  Compiling in
# the script can be a negative or positive depending on the situation.

# NOTE: This benchmarks is intentionally compiled at -O2 to avoid 
# confounding issues with SSE instructions at -O3 level.
OPT_LEVEL="-O2"

# Compile the supplied microbenchmark
g++ -g -Wall -std=c++17 ${OPT_LEVEL} MatrixColumnSum.cpp -o MatrixColumnSum

# Run 5 replications for column-major access pattern.
echo "Running experiments for Col-Major access at ${OPT_LEVEL}"
for rep in `seq 1 5`; do
    perf stat -e cache-references,cache-misses,branches,branch-misses ./MatrixColumnSum colMajor
done

# Run 5 replications for row-major access pattern.
echo "----------------------------------------------------------------------"
echo "Running experiments for row-major access at ${OPT_LEVEL}"
for rep in `seq 1 5`; do
    perf stat -e cache-references,cache-misses,branches,branch-misses ./MatrixColumnSum rowMajor
done

# End of script
