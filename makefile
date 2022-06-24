make:
	g++-11 -fopenmp quad_omp.cpp -o quad_omp.x
	for nthreads in 2 4 6 8 10 12 14 16; do\
		./quad_omp.x $$nthreads ; \
		done