#!/bin/bash

# Lines beginning with # are comments. Only lines beginning #SBATCH
# are processed by slurm

#SBATCH --account=PMIU0184
#SBATCH --job-name=hw5
#SBATCH --time=00:10:00
#SBATCH --mem=1GB
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1

# You may uncomment the following line to compile the program
g++ -g -Wall -std=c++17 -O3 Matrix.cpp NeuralNet.cpp main.cpp -o homework5

# Setup the mnist image files for testing and training on local
# temporary storage to reduce I/O times.  If it is not on local
# storage it takes a looooong time for I/O.
unzip -q /fs/ess/PMIU0184/cse443/data/mnist_images.zip -d "${TMPDIR}"

# Uncomment the following for profiling 
# perf record -F 20 --call-graph dwarf ./homework5 "${TMPDIR}/data"

# Comment out the time lines when profiling.
# Run the program 5 times to get measure consistent timings
/usr/bin/time -v ./homework5 "${TMPDIR}/data"
/usr/bin/time -v ./homework5 "${TMPDIR}/data"
/usr/bin/time -v ./homework5 "${TMPDIR}/data"
/usr/bin/time -v ./homework5 "${TMPDIR}/data"
/usr/bin/time -v ./homework5 "${TMPDIR}/data"

#end of script
