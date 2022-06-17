make:
	mpic++ quad_secuencial.cpp
	mpirun -np 4 ./a.out
	rm ./a.out