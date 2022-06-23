make:
	mpic++ quad_mpi.cpp
	mpirun -np 3 ./a.out
	rm ./a.out