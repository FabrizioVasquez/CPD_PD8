#!/bin/bash
#SBATCH --job-name=secuencialQuad
#SBATCH -o outputQuad.out

module load gcc/10.1.0
module load mpich/4.0

g++ -fopenmp quad_secuencial.cpp -o quad_secuencial.x

for n in 100 1000 10000 100000 1000000 10000000 100000000
do
    ./quad_secuencial.x $n
done

rm quad_secuencial.x

module unload mpich/4.0
module unload gcc/10.1.0