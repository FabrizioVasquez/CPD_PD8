#!/bin/bash
#SBATCH --job-name=ompQuad
#SBATCH -o outputQuad.out

module load gcc/10.1.0
module load mpich/4.0

g++ -fopenmp quad_omp.cpp -o quad_omp.x

for n in 100 1000 10000 100000 1000000 10000000 100000000
do
    for i in 2 4 6 8 10 12 14 16
        do
        ./quad_omp.x $i $n
        done
done

rm quad_omp.x

module unload mpich/4.0
module unload gcc/10.1.0