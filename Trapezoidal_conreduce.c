//Rosa Isela Martin Felipe Trapezoidal con funcion reduce
#include <stdio.h>
#include <string.h> //  For strlen 
#include <mpi.h> // For MPI functions, etc 

float trapezoidal(float a, float b, float h, int n);

float my_Function(float x);

int main(void) {
  int n = 1024, local_n;
  int comm_sz; // Number of processes 
  int my_rank; // My process rank 
  float a = 0.0, b= 3.0, h,sum;
  float a_1, b_1;
  float local_inter, total_inter;
  int source;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  h = (b-a)/n;
  local_n = n/comm_sz;

  a_1 = a + my_rank * local_n * h;
  b_1 = a_1 + h*local_n;

  local_inter = trapezoidal(a_1,b_1,h, local_n);

  MPI_Reduce(&local_inter, &sum,1, MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
  
  if(my_rank == 0)
  {
    printf(" We have %d trapzoids, our estimated for the integral from %f to %f is %f \n", n, a, b, total_inter);
  }

  MPI_Finalize();
  return 0;
} // main 


float my_Function(float x)
{
  return x*x;
}

