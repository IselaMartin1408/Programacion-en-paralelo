#include <stdio.h>
#include <string.h> //  For strlen 
#include <mpi.h> // For MPI functions, etc 

const int MAX_STRING = 50;


int main(void) {
  char greeting[MAX_STRING];
  int comm_sz; // Number of processes 
  int my_rank; // My process rank 

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank == 0) {
    sprintf("Holaaa mi inicial");
    if(my_rank % 2==0){
    if(!my_rank)
    {
    sprintf(greeting, "Holaaa mi rango es %d+2 ",my_rank);
    }
    else
    {
      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       printf("%s\n", greeting-2);
    }
    if(my_rank+2<=comm_sz)
    {
    sprintf(greeting, "Holaaa mi rango es %d+2 ",my_rank);
    MPI_Send(greeting, strlen(greeting)+2, MPI_CHAR, 0, 0,
    MPI_COMM_WORLD);}
    else
    {
      sprintf(greeting, "Holaaa mi rango es %d+2 ",my_rank);
       MPI_Send(greeting, strlen(greeting)-1, MPI_CHAR, 0, 0,
       MPI_COMM_WORLD);}
    }
    else{
      if(my_rank)
      {
       MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       printf("%s\n", greeting+2);
      }
    }
    else{
      if(my_rank+2>=comm_sz)
      {
          MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       printf("%s\n", greeting+1);
      }
      else
      {
         MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       printf("%s\n", greeting+2);
       sprintf(greeting, "Holaaa mi rango es %d+2 ",my_rank);
       MPI_Send(greeting, strlen(greeting)-1, MPI_CHAR, 0, 0,
       MPI_COMM_WORLD);}
      }
    
    }



  MPI_Finalize();
  return 0;
} // main 

