#!/bin/bash

# Lines beginning with # are comments Only lines beginning #SBATCH are
# processed by slurm

#SBATCH --account=PMIU0184
#SBATCH --job-name=ex14
#SBATCH --time=00:10:00
#SBATCH --mem=1GB
#SBATCH --nodes=1

# Setup the type of scheduling to be used by OpenMP-for
export OMP_SCHEDULE=dynamic

# Set number of OpenMP threads based on number of cores specified as
# part of SLURM job.
export OMP_NUM_THREADS=$SLURM_NTASKS

# Run the program 5 times to get measure consistent timings
perf stat ./exercise14 10000
perf stat ./exercise14 10000
perf stat ./exercise14 10000
perf stat ./exercise14 10000
perf stat ./exercise14 10000

#end of script

