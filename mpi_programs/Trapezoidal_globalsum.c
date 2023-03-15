//Rosa Isela Martin Felipe IECSA10
#include <stdio.h>
#include <string.h> //  For strlen 
#include <mpi.h> // For MPI functions, etc 
#include <math.h>
#include <stdbool.h> 

float trapezoidal(float a, float b, float h, int n);
bool isPowTwo(int n);
float my_Function(float x);

int main(void) {
  int n = 1024, local_n;
  int comm_sz; // Number of processes 
  int my_rank; // My process rank 
  float a = 0.0, b= 3.0, h;
  float a_1, b_1;
  float local_inter, total_inter;
  int source, max_recv;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  h = (b-a)/n;
  local_n = n/comm_sz;

  a_1 = a + my_rank * local_n * h;
  b_1 = a_1 + h*local_n;

  local_inter = trapezoidal(a_1,b_1,h, local_n);
    
    if (my_rank != 0) {
      if (my_rank % 2 != 0) {
             //printf("Proceso %d: Envia %f al procesador %d\n", my_rank, local_inter, my_rank - 1);
             MPI_Send(&local_inter, 1, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD);
        }
        else if (my_rank % 2 == 0) {        
            total_inter += local_inter;
            MPI_Recv(&local_inter, 1, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_inter += local_inter;
            //printf("Proceso %d: Recibe el %d del proceso %d. La suma parcial es %d.\n", my_rank, local_inter, my_rank + 1, total_inter);

            if (my_rank % 4 == 0) {
                if (isPowTwo(my_rank)) {
                    int i = 1;
                    max_recv = my_rank / 4;

                    int proNumRecv = my_rank + pow(2, i);
                    while (i <= max_recv && proNumRecv < comm_sz) {
                        MPI_Recv(&local_inter, 1, MPI_FLOAT, proNumRecv, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        total_inter += local_inter;
                        //printf("Proceso %d: Recibe %d del proceso %d. La suma parcial es %d.\n", my_rank, local_inter, proNumRecv, total_inter);
                        i++;
                        proNumRecv  = my_rank + pow(2, i);
                    }

                    MPI_Send(&total_inter, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
                    //printf("Proceso %d: Recibe %d del procesador %d.\n", my_rank, total_inter, 0); 
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
                        MPI_Recv(&local_inter, 1, MPI_FLOAT, my_rank + pow(2, i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        total_inter += local_inter;
                        //printf("Proceso %d: Recibe %d del proceso %d. La suma parcial es %d.\n", my_rank, local_inter, my_rank + 2, total_inter);
                        i++;
                    }

                    MPI_Send(&total_inter, 1, MPI_FLOAT, my_rank - 4 * max_recv, 0, MPI_COMM_WORLD);
                    //printf("Proceso %d: Envia %d al procesador %d\n", my_rank, total_inter, my_rank - 4 * max_recv); 
                } 
            }
            else {
                //printf("Proceso %d: Envia %d al procesador %d\n", my_rank, total_inter, my_rank - 2);
                MPI_Send(&total_inter, 1, MPI_FLOAT, my_rank - 2, 0, MPI_COMM_WORLD);
            }
        }
    }
    else {
        total_inter += total_inter;
        int i = 0;
        int pro_num = pow(2, i);
        while (pro_num < comm_sz) {
            MPI_Recv(&local_inter, 1, MPI_FLOAT, pro_num, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            total_inter += local_inter;
            //printf("Proceso %d: Recibe %d del proceso %d. La suma parcial es %d.\n", my_rank, local_inter, pro_num, total_inter); 
            i++;
            pro_num = pow(2, i);
        }
        printf("We have %d trapzoids, our estimated for the integral from %f to %f is %f \n", n, a, b, total_inter);
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