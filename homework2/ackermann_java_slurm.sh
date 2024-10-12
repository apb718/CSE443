#!/bin/bash

#SBATCH --account=PMIU0184
#SBATCH --job-name=hw3_cpp_java
#SBATCH --time=01:00:00
#SBATCH --mem=4GB
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1

# To keep the benchmarks more upto date, we load the more recent
# versions of the tools available on the cluster
module load gcc-compatibility/10.3.0 java/11.0.8 python/3.7-2019.10

# Function to run given command 3 times
function run3() {
    cmd="$*"
    for arg in 10 13 15 16; do
        echo "---------------------------------------------"
        echo "Running 3 reps of ${cmd} ${arg}"
        for rep in `seq 1 3`; do
           /usr/bin/time -v ${cmd} ${arg}
        done
    done
}

# This script assumes the benchmarks have been precompiled.

echo "==============================================="
run3 ./ackermann 

echo "==============================================="
run3 ./ackermann_pgo

echo "==============================================="
run3 java -Xss1G ackermann 

echo "==============================================="
run3 python3 ackermann.py

# echo "==============================================="
# Submit python jobs seperately to finish sooner. 
# /usr/bin/time -v python3 ackermann.py 13

# End of script

