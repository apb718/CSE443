#!/bin/bash

# Lines beginning with # are comments Only lines beginning #PBS are
# processed by qsub

#SBATCH --account=PMIU0184
#SBATCH --job-name=factor_count
#SBATCH --time=00:10:00
#SBATCH --mem=1GB
#SBATCH --nodes=1

# Set number of OpenMP threads based on number of cores specified as
# part of PBS job.
export OMP_NUM_THREADS=$SLURM_NTASKS

# Run the program 3 times to get measure consistent timings
/usr/bin/time -v ./exercise11
/usr/bin/time -v ./exercise11
/usr/bin/time -v ./exercise11

#end of script

