
#include <stdio.h>
#include <mpi.h>



int main(int argc, char** argv) {
     int comm_sz; // Number of processes 
    int my_rank; // My process rank
    int n, arr[100], la[100];
    int locals = 0, sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank == 0) {
        printf("Ingrese lla longitud del arreglo ", 100);
        scanf("%d", &n);

        printf("Ingrese los elementos del arreglo:\n");
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(arr, n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = my_rank; i < n; i += comm_sz) {
        if (arr[i] % 2 == 1) {
            la[locals++] = arr[i];
        }
    }

    int local_val_sum = 0, val_sum = 0;
    for (int i = 0; i < locals; i++) {
        local_val_sum += la[i];
    }
    MPI_Reduce(&local_val_sum, &val_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
      printf("Los numeros impares del arreglo son: ");
        for (int i = 0; i < locals; i++) {
            printf("%d ", la[i]);
        }
    }

    MPI_Finalize();
    return 0;
}