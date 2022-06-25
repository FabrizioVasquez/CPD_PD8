#include <mpi.h>
#include <math.h>
#include <ctime>
#include <cstdlib>  
#include <iostream>
#include <fstream>

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
  int num = strtol(argv[1], NULL, 10);
  int n = num;
  int rank{}, size{};
  MPI_Init(&argc, &argv);                   // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of processors

  double a = 0.0,b = 10.0,error;
  double exact = 0.49936338107645674464;
  int i;
  int divide_data=0;
  double total, wtime,wtime1,wtime2, x;


  if(rank == 0){
    wtime1 = MPI_Wtime ();
    divide_data = n/(size-1); // divide the data
    n = (size - 1) * divide_data;
    printf ( "QUAD_SECUENCIAL:\n" );
    printf ( "  Integral de f(x) = 50 / (pi*(2500*x*x+1)).\n" );
    printf ( "  A, B, N  = %f, %f, %d\n", a,b,n );
    printf ( "  Valor exacto = %24.16f\n", exact );  
  }

  MPI_Bcast( &divide_data , 1 , MPI_INT , 0 , MPI_COMM_WORLD);

  double x1{};
  double x2{};
  int tag{};
  int to{};
  int p{};
  int target{};
  double my_total{};

  if ( rank == 0 )
  {
    for ( p = 1; p <= size - 1; p++ )
    {
      x1 = ( ( double ) ( size - p     ) * a   
             + ( double ) (         p - 1 ) * b ) 
             / ( double ) ( size     - 1 );

      target = p;
      tag = 1;
      MPI_Send ( &x1, 1, MPI_DOUBLE, target, tag, MPI_COMM_WORLD );

      x2 = ( ( double ) ( size - p - 1 ) * a   
             + ( double ) (         p     ) * b ) 
             / ( double ) ( size     - 1 );

      target = p;
      tag = 2;
      MPI_Send ( &x2, 1, MPI_DOUBLE, target, tag, MPI_COMM_WORLD );
    }
    total = 0.0;
    my_total = 0.0;
  }
/*
  Processes receive x1, x2, and compute their part of the integral.
*/
  else
  {
    tag = 1;
    MPI_Recv ( &x1, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

    tag = 2;
    MPI_Recv ( &x2, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

    my_total = 0.0;
    for ( i = 1; i <= divide_data; i++ )
    {
      x = ( ( double ) ( divide_data - i     ) * x1 
          + ( double ) (        i - 1 ) * x2 )
          / ( double ) ( divide_data     - 1 );
      my_total = my_total + f ( x );
    }

    my_total = ( x2 - x1 ) * my_total / ( double ) ( divide_data );

    //printf ( "  Process %d contributed MY_TOTAL = %f\n", rank, my_total );
  }

  MPI_Reduce ( &my_total, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );

  if ( rank == 0 )
  {
    std::ofstream file_out("tiempos_quad_mpi_"+std::to_string(num)+".txt",std::ios::out|std::ios::app);
    wtime2 = MPI_Wtime ();
    error = fabs ( total - exact );
    wtime = wtime2 - wtime1;
    file_out << size << "\t" << (wtime) <<std::endl;
    file_out.close();
    //printf ( "\n" );
    //printf ( "  Estimate = %24.16f\n", total );
    //printf ( "  Error = %e\n\n", error );
    printf ( "  Time = %f\n\n", wtime );
  }

  
  //total = 0.0;

  /*for (int i=0; i<n; i++){
    x = ((n-i-1)*a+i*b)/(n-1);
    total=total+f(x);
  }*/
  //wtime2 = cpu_time();

  /*total = (b-a)*total/n;
  error = fabs(total-exact);
  wtime = wtime2 - wtime1;*/

  //printf ( "  Valor estimado = %24.16f\n", exact );
  //printf ( "  Error    = %e\n", error );
  //printf ( "  Total     = %f\n", total );
  //printf ( "  Tiempo     = %f\n", wtime );


  /*if(rank == 0){
    std::cout<<"∑ßdx."<<std::endl;
  }*/

  MPI_Finalize();

}