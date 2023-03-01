#include <mpi.h>
#include <stdio.h>

// No funciona bien del todo

int main(int argc, char** argv) {
    // Inicializa MPI
    MPI_Init(NULL, NULL);

    // Obtiene el número de procesos y el ID del proceso actual
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Crea un comunicador en anillo
    MPI_Comm ring_comm;
    MPI_Comm_split(MPI_COMM_WORLD, world_rank, 0, &ring_comm);

    // Envia y recibe el mensaje en anillo
    int message = 13;
    int next_rank = (world_rank + 1) % world_size;
    int prev_rank = (world_rank - 1 + world_size) % world_size;
    if (world_rank == 0) {
        // Si soy el proceso 0, envío el mensaje al proceso 1 y luego lo recibo del último proceso
        MPI_Send(&message, 1, MPI_INT, next_rank, 0, ring_comm);
        MPI_Recv(&message, 1, MPI_INT, prev_rank, 0, ring_comm, MPI_STATUS_IGNORE);
    } else {
        // Si no soy el proceso 0, primero recibo el mensaje del proceso anterior y luego lo envío al siguiente
        MPI_Recv(&message, 1, MPI_INT, prev_rank, 0, ring_comm, MPI_STATUS_IGNORE);
        MPI_Send(&message, 1, MPI_INT, next_rank, 0, ring_comm);
    }

    // Imprime el mensaje recibido
    printf("Proceso %d recibió el mensaje %d del proceso %d\n", world_rank, message, prev_rank);

    // Finaliza MPI
    MPI_Finalize();
}
