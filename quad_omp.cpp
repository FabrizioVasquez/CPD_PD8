#include <iostream>
#include <cmath>
#include <omp.h>
#include<fstream>

using namespace std;

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
/*
void timestamp (){
# define TIME_SIZE 40
  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  time_t now;
  now = time ( NULL );
  tm = localtime ( &now );
  strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );
  printf ( "%s\n", time_buffer );
  return;
# undef TIME_SIZE
}
*/
int main(int argc, char *argv[]){
  long num = strtol(argv[1], NULL, 10);
  int num2 = num;
  double a = 0.0,b = 10.0,error;
  double exact = 0.49936338107645674464;
  int i, n = 10000000;
  double total, wtime,wtime1,wtime2, x;

  //timestamp ( );
  printf ( "QUAD_SECUENCIAL:\n" );
  printf ( "  Integral de f(x) = 50 / (pi*(2500*x*x+1)).\n" );
  printf ( "  A, B, N  = %f, %f, %d\n", a,b,n );
  printf ( "  Valor exacto = %24.16f\n", exact );

  wtime1 = cpu_time();

  total = 0.0;
  #pragma omp parallel shared (a,b,n) num_threads(num2)
  #pragma omp for private (i) reduction(+:total)
    for (int i=0; i<n; i++){
        x = ((n-i-1)*a+i*b)/(n-1);
        total=total+f(x);
  }
  wtime2 = cpu_time();
  
  #pragma omp master
  {
    std::ofstream file_out("tiempos_quad_omp_"+std::to_string(n)+".txt",std::ios::out|std::ios::app);
    total = (b-a)*total/n;
    error = fabs(total-exact);
    wtime = wtime2 - wtime1;
    file_out <<num2<<"\t"<< wtime<< std::endl;
  }

  printf ( "  Valor estimado = %24.16f\n", exact );
  printf ( "  Error    = %e\n", error );
  printf ( "  Tiempo     = %f\n", wtime );


  return 0;
}


