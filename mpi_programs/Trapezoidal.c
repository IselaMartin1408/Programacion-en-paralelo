//Rosa Isela Martin Felipe Trapezoidal con global sum
#include <stdio.h>
#include <string.h> //  For strlen 
#include <mpi.h> // For MPI functions, etc 
#include <stdbool.h> 

bool isPowTwo(int n);
float my_Function(float x);
float sumacool(float a, float b, float h, int n);
int comm_sz; // Number of processes 
int my_rank,j=1, k=0; // My process rank 
float dig[50], sumt;

int main(void) {
  int n = 1024, local_n;
  
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

  local_inter = sumacool(a_1,b_1,h, local_n);

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


float sumacool(float a, float b, float h, int n)
{
    int num, max_recv;
    float sum_area[50],x,area;
 
  sum_area[k] = (my_Function(a)+my_Function(b))/2.0;
  k++;
    
    if (my_rank != 0) {
      if (my_rank % 2 != 0) {
             MPI_Send(&sum_area[my_rank], 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
        }
        else if (my_rank % 2 == 0) { 
            x = a + j*h;       
            sumt += sum_area[my_rank];
            num = my_Function(x);
            MPI_Recv(&num, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sumt += num;
            j++;

            if (my_rank % 4 == 0) {
                if (isPowTwo(my_rank)) {
                    int i = 1;
                    max_recv = my_rank / 4;

                    int proNumRecv = my_rank + pow(2, i);
                    while (i <= max_recv && proNumRecv < comm_sz) {
                        x = a + j*h; 
                        num = my_Function(x);
                        MPI_Recv(&num, 1, MPI_INT, proNumRecv, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        sumt += num;
                        i++;
                        j++;
                        proNumRecv  = my_rank + pow(2, i);
                    }

                    MPI_Send(&sumt, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
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
                        x = a + j*h;
                        num = my_Function(x);
                        MPI_Recv(&num, 1, MPI_INT, my_rank + pow(2, i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        sumt += num;
                        i++;
                        j++;       
                    }

                    MPI_Send(&sumt, 1, MPI_INT, my_rank - 4 * max_recv, 0, MPI_COMM_WORLD);
                } 
            }
            else {
                MPI_Send(&sumt, 1, MPI_INT, my_rank - 2, 0, MPI_COMM_WORLD);
            }
        }
    }
    else {
        sumt += dig[my_rank];
        int i = 0;
        int pro_num = pow(2, i);
        while (pro_num < comm_sz) {
            x = a + j*h; 
            num = my_Function(x);
            MPI_Recv(&num, 1, MPI_INT, pro_num, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            sumt += num;
            i++;
            j++;
            pro_num = pow(2, i);
        }
        area = sumt*h;
        printf("El area bajo la curva es: %f.", area);
    }
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