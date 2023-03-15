#include <stdio.h> 
#include <string.h> 
#include <mpi.h>

float my_Function(float x);
float trapezoidal(float a, float b, float h, int n);

int main(void) {
    
    int comm_sz, local_n; 
    int my_rank;
    int n = 1024;
    float a = 0.0,b=3.0,h;
    float a_1, b_1;
    float local_inter, total_inter,tag, count;
    int source;

    MPI_Init(NULL, NULL); 
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    h=(b-a)/n;
    local_n = n/comm_sz;

    a_1 = a + my_rank * local_n*h;//cuantos cachos le toca a cada proceso punto inicual
    b_1 = a_1 + h*local_n;//punto final
    
    local_inter = trapezoidal(a_1,b_1,h,local_n);

    if(my_rank==0)
    {
        total_inter = local_inter;
     for (source = 1; source < comm_sz; source++)
     {
        /* code */
        MPI_Recv(&local_inter,1,MPI_FLOAT,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&local_inter,1,MPI_FLOAT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        total_inter += local_inter;
        source = status.MPI_SOURCE;
        tag = status.MPI_TAG;
        MPI_Get_count(&status,MPI_FLOAT,&count);
     }
     
    }
    else
    {
        MPI_Send(&local_inter,1,MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }
    if(my_rank == 0)
    {
        printf("We have %d trapezoids, out estimated for the integral from %f to %f is %f \n", n, a, b, total_inter);
    }
    MPI_Finalize();
    return 0; 
}
float my_Function(float x)
{
    return x*x;
}

float trapezoidal(float a, float b, float h, int n)
{
    float sum_area, x;
    int i;

    sum_area = (my_Function(a)+my_Function(b))/2.0;
    for(i=1; i<n; i++)
    {
        x = a + i*h;
        sum_area += my_Function(x);
    }
        sum_area = sum_area * h;

        return sum_area;
}
