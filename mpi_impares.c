#include <mpi.h>
#include <stdio.h>

int main(void) {
  int rank, size, i, sum = 0;
  int local_sum, global_sum;
  int arr[80];
  
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0){
        sum = 1;
    }else{
        if (rank % 2 == 0){ // proceso par
            MPI_Recv(&sum, 1, MPI_INT, rank - 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            printf("Soy el proceso %d, recibi el mensaje %d del proceso %d\n", rank, sum, rank - 2);
        }else{ // procesos impares
            if(rank == size - 1){ // si soy el último proceso
                MPI_Recv(&sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Soy el proceso %d, recibi el mensaje %d del proceso %d\n", rank, sum, rank - 1);
            }else if(rank == size - 2){ // si soy el penúltimo proceso
                MPI_Recv(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Soy el proceso %d, recibi el mensaje %d del proceso %d\n", rank, sum, rank + 1);
            }else{
                MPI_Recv(&sum, 1, MPI_INT, rank + 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Soy el proceso %d, recibi el mensaje %d del proceso %d\n", rank, sum, rank + 2);
            }
        }
    }

        if(rank == 0 && size > 2){
        MPI_Send(&sum, 1, MPI_INT, rank + 2, 0,MPI_COMM_WORLD);
    }else{
        if (rank % 2 == 0){ 
            if(rank == size - 1){ 
                MPI_Send(&sum, 1, MPI_INT, rank - 1, 0,MPI_COMM_WORLD);
                printf("Soy el proceso %d, envio el mensaje %d del proceso %d", rank, sum, rank -1);
            }else{
                if(rank == size - 2){ 
                MPI_Send(&sum, 1, MPI_INT, rank + 1, 0,MPI_COMM_WORLD);
                printf("Soy el proceso %d, envio el mensaje %d del proceso %d", rank, sum, rank +1);
                }else{
                MPI_Send(&sum, 1, MPI_INT, rank + 2, 0,MPI_COMM_WORLD);
                printf("Soy el proceso %d, envio el mensaje %d del proceso %d", rank, sum, rank +2);
                }
            }
        }else{ 
            if(rank == size - 1){ 
                MPI_Send(&sum, 1, MPI_INT, rank - 2, 0,MPI_COMM_WORLD);
                printf("Soy el proceso %d, envio el mensaje %d del proceso %d", rank, sum, rank -2);
            }else{
                if(rank == 0){ 
                printf("%d",sum);
                }else{ 
                    MPI_Send(&sum, 1, MPI_INT, rank - 2, 0,MPI_COMM_WORLD);
                }
            }
        }
    }

  MPI_Finalize();
  return 0;
}