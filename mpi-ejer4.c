#include <mpi.h>
#include <stdio.h>

// Función a integrar
double f(double x) {
    return x * x;
}

int main(int argc, char** argv) {
    // Inicializa MPI
    MPI_Init(NULL, NULL);

    // Obtener el número de procesos y el ID del proceso actual
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Límites de integración
    double a = 0;
    double b = 1;

    // Número de trapecios
    int n = 100;

    // Tamaño del trapecio
    double h = (b - a) / n;

    // Cada proceso calcula el área de un conjunto de trapecios
    int local_n = n / world_size;
    double local_a = a + world_rank * local_n * h;
    double local_b = local_a + local_n * h;
    double integral = 0;
    for (int i = 0; i < local_n; i++) {
        integral += h * (f(local_a + i * h) + f(local_a + (i + 1) * h)) / 2;
    }

    // Los procesos envían su resultado al proceso 0 y este los suma
    if (world_rank == 0) {
        double total_integral = integral;
        for (int i = 1; i < world_size; i++) {
            MPI_Recv(&integral, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_integral += integral;
        }
        printf("Valor de la integral: %f\n", total_integral);
    } else {
        MPI_Send(&integral, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    // Finaliza MPI
    MPI_Finalize();
}
