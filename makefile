all:
	echo "Mirar makefile"
# omp:
# 	g++-11 -fopenmp quad_omp.cpp -o quad_omp.x
# 	for nthreads in 2 4 6 8 10 12 14 16; do\
# 		./quad_omp.x $$nthreads ; \
# 		done
mpi:
	mpic++ quad_mpi.cpp -o quad_mpi.x
	mpirun -np 2 ./quad_mpi.x 100
	rm quad_mpi.x

clean:
	rm *.txt