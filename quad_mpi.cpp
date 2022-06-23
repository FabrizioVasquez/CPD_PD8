#include <mpi.h>
#include <math.h>
#include <ctime>
#include <cstdlib>  
#include <iostream>


double f(double x){
  double pi = 3.141592653589793;
  double value;
  value=50.0/(pi*(2500.0*x*x+1.0));
  return value;
}

double cpu_time (){
  double value;
  value = (double)clock()/(double)CLOCKS_PER_SEC;
  return value;
}


int main(int argc, char **argv) {

  int rank{}, size{};
  MPI_Init(&argc, &argv);                   // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of processors

  double a = 0.0,b = 10.0,error;
  double exact = 0.49936338107645674464;
  int i, n = 10000000;
  int divide_data=0;
  double total, wtime,wtime1,wtime2, x;


  if(rank == 0){
    divide_data = n/(size-1); // divide the data
    n = (size - 1) * divide_data;
    printf ( "QUAD_SECUENCIAL:\n" );
    printf ( "  Integral de f(x) = 50 / (pi*(2500*x*x+1)).\n" );
    printf ( "  A, B, N  = %f, %f, %d\n", a,b,n );
    printf ( "  Valor exacto = %24.16f\n", exact );  
  }

  MPI_Bcast( &divide_data , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
  wtime1 = cpu_time();

  total = 0.0;

  for (int i=0; i<n; i++){
    x = ((n-i-1)*a+i*b)/(n-1);
    total=total+f(x);
  }
  wtime2 = cpu_time();

  total = (b-a)*total/n;
  error = fabs(total-exact);
  wtime = wtime2 - wtime1;

  printf ( "  Valor estimado = %24.16f\n", exact );
  printf ( "  Error    = %e\n", error );
  printf ( "  Tiempo     = %f\n", wtime );

  if(rank == 0){
    std::cout<<"∑ßdx."<<std::endl;
  }

  MPI_Finalize();

}