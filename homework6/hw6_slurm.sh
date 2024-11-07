#!/bin/bash

# Lines beginning with # are comments. Only lines beginning #SBATCH
# are processed by slurm

#SBATCH --account=PMIU0184
#SBATCH --job-name=hw5
#SBATCH --time=00:10:00
#SBATCH --mem=1GB
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1

# benchmark about 100 seconds

# You may uncomment the following line to compile the program
g++ -g -Wall -std=c++17 -O3 main.cpp PNG.cpp PNG.h Assert.h -o ImageChecker -fopenmp -lpng

#set core count to x
export OMP_NUM_THREADS=1

perf record -F 20 --call-graph fp ./ImageChecker images/Mammogram.png  images/Cancer_mask.png outputs/cancerout.png

# Run the script
# /usr/bin/time -v ./ImageChecker images/Mammogram.png  images/Cancer_mask.png outputs/cancerout.png true 75 32

#end of script
