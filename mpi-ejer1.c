#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[]) {
    int rank, size;
    MPI_Init (&argc, &argv); /* iniciar MPI */
    MPI_Comm_rank (MPI_COMM_WORLD, &rank); /* obtener id del proceso */
    MPI_Comm_size (MPI_COMM_WORLD, &size); /* obtener número de procesos */
    printf("Hello world from process %d of %d\n", rank, size );
    MPI_Finalize();
    return 0;
}