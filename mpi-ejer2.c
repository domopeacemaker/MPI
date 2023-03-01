#include <mpi.h>
#include <stdio.h>

#define PING_PONG_LIMIT 10

int main(int argc, char** argv) {
    // Inicializar MPI
    MPI_Init(NULL, NULL);

    // Obtener el número de procesos y el ID del proceso actual
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Comprobar si hay solo dos procesos
    if (world_size != 2) {
        fprintf(stderr, "Este programa solo funciona con dos procesos.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Realizar el ping-pong entre los dos procesos
    int ping_pong_count = 0;
    int partner_rank = (world_rank + 1) % 2;
    while (ping_pong_count < PING_PONG_LIMIT) {
        // Si soy el proceso 0, envío un mensaje al proceso 1
        if (world_rank == 0) {
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            printf("Proceso 0 envió y aumentó ping_pong_count %d al proceso 1\n", ping_pong_count);
        } else {
            // Si soy el proceso 1, recibo un mensaje del proceso 0
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Proceso 1 recibió ping_pong_count %d del proceso 0\n", ping_pong_count);
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0,/*tag*/ MPI_COMM_WORLD);
            printf("Proceso 1 envió y aumentó ping_pong_count %d al proceso 0\n", ping_pong_count);
        }
    }

    // Finalizar MPI
    MPI_Finalize();
}

