#include <stdio.h>
#include <string.h> //  For strlen 
#include <mpi.h> // For MPI functions, etc 
#include <math.h>
#include <stdbool.h> 

float trapezoidal (float a, float b, float h, int n);
float my_Function(float x);
bool Potencia2(int p);

bool Potencia2(int p) 
{ 
   if(p==0) 
    return false; 
  
   return (ceil(log2(p)) == floor(log2(p))); 
} 
int main(void) {
  //VARIABLES
  int n=1024, local_n;//numero de divisiones/numero de procesos
  int comm_sz; // Number of processes 
  int my_rank; // My process rank 
  float a=0.0,b=3.0,h;
  float a_1,b_1;
  float local_inter,total_inter;
  int source;


  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  h=(b-a)/n;

  local_n = n/comm_sz;

  a_1 = a + my_rank * local_n * h;
  b_1 = a_1 + h*local_n;
  
  local_inter = trapezoidal(a_1,b_1,h,local_n);
  
  if (my_rank != 0) {
        // Los numeros impares le mandan su valor al numero par anterior a ellos 
        if (my_rank % 2 != 0) {
             printf("Procesador #%d: enviando %f al procesador #%d!\n", my_rank, local_inter, my_rank - 1);
             MPI_Send(&local_inter, 1, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD);
        }
        // Para numeros pares
        else if (my_rank % 2 == 0) {        
            total_inter += local_inter;
            
            // EL procesador 2 recibe el dato del porcesador 3 y asi sucesivamente
            MPI_Recv(&local_inter, 1, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_inter += local_inter;
            printf("Procesador #%d: Recibio %f del Procesador #%d. Suma parcial es: %f.\n", my_rank, local_inter, my_rank + 1, total_inter);

            // Procesadores multiplos de 4
            if (my_rank % 4 == 0) {
                if (Potencia2(my_rank)) {
                    int i = 1;
                    int max_receives = my_rank / 4;

                    int coreNumToReceive = my_rank + pow(2, i);
                    while (i <= max_receives && coreNumToReceive < comm_sz) {
                        int number;
                        MPI_Recv(&local_inter, 1, MPI_FLOAT, coreNumToReceive, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        total_inter += local_inter;
                        printf("Procesador #%d: Recibio %f del Procesador #%d. Suma parcial es: %f.\n", my_rank, local_inter, coreNumToReceive, total_inter);
                        i++;
                        coreNumToReceive  = my_rank + pow(2, i);
                    }

                    MPI_Send(&total_inter, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
                    //printf("Procesador #%d: enviando %f al procesador #%d.\n", my_rank, total_inter, 0); 
                }

                else {
                    int max_receives;
                    if ((my_rank / 4) % 2 == 0) {
                        max_receives = 2;
                    }
                    else {
                        max_receives = 1;
                    }
                    
                    int i = 1;
                    while (i <= max_receives) {
                
                        MPI_Recv(&local_inter, 1, MPI_FLOAT, my_rank + pow(2, i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        total_inter += local_inter;
                        //printf("Procesador #%d: Recibio %f del Procesador #%d. Suma parcial es: %f.\n", my_rank, local_inter, my_rank + 2, total_inter);
                        i++;
                    }

                    MPI_Send(&total_inter, 1, MPI_INT, my_rank - 4 * max_receives, 0, MPI_COMM_WORLD);
                    //printf("Procesador #%d: enviando %f al procesador #%d.\n", my_rank, total_inter, my_rank - 4 * max_receives); 
                } 
            }
            else {
                //printf("Procesador #%d: enviando %f al procesador #%d.\n", my_rank, total_inter, my_rank - 2);
                MPI_Send(&total_inter, 1, MPI_INT, my_rank - 2, 0, MPI_COMM_WORLD);
            }
        }
    }
    else {
        total_inter += my_rank;
        int i = 0;
        int coreNum = pow(2, i);
        while (coreNum < comm_sz) {
            int number;
            MPI_Recv(&local_inter, 1, MPI_INT, coreNum, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            total_inter += local_inter;
            //printf("Procesador #%d: Recibio %f del Procesador #%d. Suma parcial es: %f.\n", my_rank, local_inter, coreNum, total_inter); 
            i++;
            coreNum = pow(2, i);
        }
        printf("Suma global es: %f.", total_inter);
    }
    MPI_Finalize();
    return 0; 
}

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

float my_Function(float x){
  return x*x;
}