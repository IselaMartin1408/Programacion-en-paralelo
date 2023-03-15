
//Rosa Isela Martin Felipe 7418
#include <mpi.h>
#include <stdio.h>
#include <math.h>

void Get_input(int my_rank, int comm_sz, float* a, float* b, int* n);
float trapezoidal(float a, float b, float h, int n);
float my_Function(float x);


int main(void) {
    int n = 0.0, local_n;
    int comm_sz; 
    int my_rank;
    float a = 0.0, b = 0.0, h = 0.0;
    float a_1, b_1;
    float local_inter, total_inter = 0;
    int i=1;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

  Get_input(my_rank, comm_sz, &a, &b, &n);  

  h = (b-a)/n;
  local_n = n/comm_sz;

  a_1 = a + my_rank * local_n *h; 
  b_1 = a_1 + h*local_n;

  local_inter = trapezoidal(a_1,b_1,h,local_n);
  total_inter = local_inter;

MPI_Reduce(&local_inter, &total_inter, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    
  
  if (my_rank == 0)
  {
   printf("We have %d trapzoids, our estimated for the integral from %f to %f is %f \n",n,a,b,total_inter);

  }
  
  MPI_Finalize();
  return 0;


}

void Get_input(int my_rank, int comm_sz, float* a_p, float* b_p, int* n_p)
  {
  int destino = 0;

    if (my_rank == 0)
  {
    printf("Digite limite a: ");
		scanf("%f",a_p);
		printf("\n Digite limite b: ");
		scanf("%f",b_p);
    printf("\n Digite numero de particiones: ");
		scanf("%d",n_p);
  }

      MPI_Bcast(a_p,1,MPI_FLOAT,0,MPI_COMM_WORLD);
      MPI_Bcast(b_p,1,MPI_FLOAT,0,MPI_COMM_WORLD);
      MPI_Bcast(n_p,1,MPI_INT,0,MPI_COMM_WORLD);
  }



float trapezoidal(float a, float b, float h, int n){
    float sum_area, x;
    int i;

    sum_area = (my_Function(a)+my_Function(b))/2.0;
    for ( i = 1; i < n; i++)
    {
        x = a + i*h;
        sum_area += my_Function(x);
    }

    sum_area = sum_area * h;

    return sum_area;
    

}

float my_Function(float x){

    return x*x;

}

