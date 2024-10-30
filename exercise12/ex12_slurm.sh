#!/bin/bash

# Lines beginning with # are comments Only lines beginning #SBATCH are
# processed by slurm

#SBATCH --account=PMIU0184
#SBATCH --job-name=ex12
#SBATCH --time=00:10:00
#SBATCH --mem=1GB
#SBATCH --nodes=1

# Compile just to ensure correct executable
g++ -g -Wall -std=c++17 -O3 -fopenmp main.cpp -o exercise12

# Set number of OpenMP threads based on number of cores specified as
# part of PBS job.
export OMP_NUM_THREADS=$SLURM_NTASKS

# Run the program 5 times to get measure consistent timings
/usr/bin/time -v ./exercise12
/usr/bin/time -v ./exercise12
/usr/bin/time -v ./exercise12
/usr/bin/time -v ./exercise12
/usr/bin/time -v ./exercise12

#end of script
