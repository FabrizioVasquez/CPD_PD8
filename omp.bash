
module load gcc/10.1.0
module load mpich/4.0

g++-11 -fopenmp quad_omp.cpp -o quad_omp.x

for i in 2 4 6 8 10 12
do
 ./quad_omp $i
done

module unload mpich/4.0
module unload gcc/10.1.0