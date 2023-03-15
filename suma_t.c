#include <stdio.h> 
#include <string.h> 
#include <mpi.h>
#include <math.h>
#include <stdbool.h> 

bool isPowTwo(int n);

int main(void) {
    int comm_sz; 
    int my_rank;

    MPI_Init(NULL, NULL); 
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    int dig[32] = {9, 12, 7, 1, 2, 14, 9, 1, 9, 19, 5, 7, 1, 17, 11, 2, 8, 9, 1, 15, 11, 5, 7, 8, 9, 12, 20, 21, 7, 3, 8, 14};
    
    int sumt = 0;
    int num, max_recv;
    
    if (my_rank != 0) {
      if (my_rank % 2 != 0) {
             printf("Proceso %d: Envia %d al procesador %d\n", my_rank, dig[my_rank], my_rank - 1);
             MPI_Send(&dig[my_rank], 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
        }
        else if (my_rank % 2 == 0) {        
            sumt += dig[my_rank];
            MPI_Recv(&num, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sumt += num;
            printf("Proceso %d: Recibe el %d del proceso %d. La suma parcial es %d.\n", my_rank, num, my_rank + 1, sumt);

            if (my_rank % 4 == 0) {
                if (isPowTwo(my_rank)) {
                    int i = 1;
                    max_recv = my_rank / 4;

                    int proNumRecv = my_rank + pow(2, i);
                    while (i <= max_recv && proNumRecv < comm_sz) {
                        MPI_Recv(&num, 1, MPI_INT, proNumRecv, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        sumt += num;
                        printf("Proceso %d: Recibe %d del proceso %d. La suma parcial es %d.\n", my_rank, num, proNumRecv, sumt);
                        i++;
                        proNumRecv  = my_rank + pow(2, i);
                    }

                    MPI_Send(&sumt, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                    printf("Proceso %d: Recibe %d del procesador %d.\n", my_rank, sumt, 0); 
                }
                else {  
                    
                    if ((my_rank / 4) % 2 == 0) {
                        max_recv = 2;
                    }
                    else {
                        max_recv = 1;
                    }
                    
                    int i = 1;
                    while (i <= max_recv) {
                        MPI_Recv(&num, 1, MPI_INT, my_rank + pow(2, i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        sumt += num;
                        printf("Proceso %d: Recibe %d del proceso %d. La suma parcial es %d.\n", my_rank, num, my_rank + 2, sumt);
                        i++;
                    }

                    MPI_Send(&sumt, 1, MPI_INT, my_rank - 4 * max_recv, 0, MPI_COMM_WORLD);
                    printf("Proceso %d: Envia %d al procesador %d\n", my_rank, sumt, my_rank - 4 * max_recv); 
                } 
            }
            else {
                printf("Proceso %d: Envia %d al procesador %d\n", my_rank, sumt, my_rank - 2);
                MPI_Send(&sumt, 1, MPI_INT, my_rank - 2, 0, MPI_COMM_WORLD);
            }
        }
    }
    else {
        sumt += dig[my_rank];
        int i = 0;
        int pro_num = pow(2, i);
        while (pro_num < comm_sz) {
            MPI_Recv(&num, 1, MPI_INT, pro_num, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            sumt += num;
            printf("Proceso %d: Recibe %d del proceso %d. La suma parcial es %d.\n", my_rank, num, pro_num, sumt); 
            i++;
            pro_num = pow(2, i);
        }

        printf("LA SUMA TOTAL ES: %d.", sumt);
    }
    MPI_Finalize();
    return 0; 
}


bool isPowTwo(int n) 
{ 
   if(n==0) 
   {
   printf("Por favor digite un numero potencia de dos");
   }
   else
   {
    return true;
   }
} 