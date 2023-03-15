#include <stdio.h>
#include <string.h> //  For strlen 
#include <mpi.h> // For MPI functions, etc 

float trapezoidal(float a, float b, float h, int n);

float my_Function(float x);

int main(void) {
  int n = 1024, local_n;
  int comm_sz; // Number of processes 
  int my_rank; // My process rank 
  float a = 0.0, b= 3.0, h;
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

  if(my_rank == 0)
  {
    total_inter = local_inter;
    for (source = 1; source < comm_sz; source++)
    {
      MPI_Recv(&local_inter, 1, MPI_FLOAT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      total_inter += local_inter;
    }
  }


  else
  {
    MPI_Send(&local_inter, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
  }
  
  if(my_rank == 0)
  {
    printf(" We have %d trapzoids, our estimated for the integral from %f to %f is %f \n", n, a, b, total_inter);
  }

  MPI_Finalize();
  return 0;
} // main 

float trapezoidal (float a, float b, float h, int n){

    float sum_area,x;
    int i;

    sum_area=(my_Function(a)+my_Function(b))/2.0;
    for ( i = 1; i < n; i++) {
        x = a+i*h;
        sum_area += my_Function(x);
    }
    sum_area = sum_area * h;

    return sum_area;
    
}

float my_Function(float x)
{
  return x*x;
}

