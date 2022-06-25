#!/bin/bash
#SBATCH --job-name=mpiQuad
#SBATCH -o outputQuad.out

module load gcc/10.1.0
module load mpich/4.0

mpic++ quad_mpi.cpp -o quad_mpi.x

for n in 100 1000 10000 100000 1000000 10000000 100000000
do
    for p in 2 3 4 5 6 7 8 9 10 11 12
        do
        mpirun -np $p ./quad_mpi.x $n
        done
done

rm quad_mpi.x

module unload mpich/4.0
module unload gcc/10.1.0