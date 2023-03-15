//Rosa Isela Martin Felipe examen
#include <stdio.h>
#include <string.h> //  For strlen 
#include <mpi.h> // For MPI functions, etc 
#include <stdbool.h> 
#include <stdlib.h>
#include <math.h>


bool isPowTwo(int n);
void Get_input(int my_rank, int comm_sz, float* x);
float find_max(float* numbers, int size);




int main(void) {
  int comm_sz; // Number of processes 
  int my_rank; // My process rank
  float x = 0.0, numbers[10], n=10, max;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

 Get_input(my_rank, comm_sz, &x);  
 
 if (isPowTwo(comm_sz)==false)
 {
   printf("Favor de digitar una potencia de dos");
 } 
 else{
    
for(int i=0; i==n; i++)
{
   numbers[i]=rand()%100;  
}
  


 MPI_Reduce(&numbers, &max, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

  if(my_rank==0){
printf("El indici de mayor numero es %f",max );
  }
 
  MPI_Finalize();
  return 0;
} // main 

bool isPowTwo(int n) 
{
    if (n == 0)
        return false;
 
    return (ceil(log2(n)) == floor(log2(n)));
}


void Get_input(int my_rank, int comm_sz, float* x)
  {
  int destino = 0;

    if (my_rank == 0)
  {
    printf("Digite un numero: ");
		scanf("%f",x);
  }

      MPI_Bcast(x,1,MPI_FLOAT,0,MPI_COMM_WORLD);
  }


