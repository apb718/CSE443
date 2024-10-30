#!/bin/bash

# Lines beginning with # are comments Only lines beginning #PBS are
# processed by qsub

#SBATCH --account=PMIU0184
#SBATCH --job-name=factor_count
#SBATCH --time=00:01:00
#SBATCH --mem=1GB
#SBATCH --nodes=1
#SBATCH --tasks-per-node=2

# Set number of OpenMP threads based on number of cores specified as
# part of PBS job.
g++ -fopenmp -g -Wall -std=c++17 -O3 *.cpp -o /users/PMIU0184/byrdap/cse443/exercise15/exercise15 -lboost_system -lpthread

export OMP_NUM_THREADS=2

echo "Test Start"
# Run the program 3 times to get measure consistent timings
./exercise15 | diff reference_output.txt -
./exercise15 | diff reference_output.txt -
./exercise15 | diff reference_output.txt -

echo "Test End"
#end of script