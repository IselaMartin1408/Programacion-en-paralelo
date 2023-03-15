//Rosa Isela Martin Felipe examen
#include <stdio.h>
#include <string.h> //  For strlen 
#include <mpi.h> // For MPI functions, etc 
#include <stdbool.h> 
#include <stdlib.h>
#include <math.h>


void Get_input(int my_rank, int comm_sz, float* x);

int main(void) {
  int comm_sz; // Number of processes 
  int my_rank; // My process rank
  float x = 0.0, sum;

int numbers[2][2] = {{1,2},{3,4}};

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

 Get_input(my_rank, comm_sz, &x);  
 MPI_Reduce(&numbers, &sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

 for(int i=0; i==x; i++)
{
   numbers[i]=rand()%100;  
}
  
if (my_rank == 0) {
 

    MPI_Send(numbers, sum, MPI_INT, 1, 0, MPI_COMM_WORLD);
} else if (my_rank == 1) {
    MPI_Status status;
    MPI_Recv(numbers, sum, MPI_INT, 0, 0, MPI_COMM_WORLD,
             &status);
    MPI_Get_count(&status, MPI_INT, &sum);
}

 
  MPI_Finalize();
  return 0;
} // main 



void Get_input(int my_rank, int comm_sz, float* x)
  {
  int destino = 0;

    if (my_rank == 0)
  {
    printf("Digite el tamaño de la matriz ");
		scanf("%f",x);
  }

      MPI_Bcast(x,1,MPI_FLOAT,0,MPI_COMM_WORLD);
  }


