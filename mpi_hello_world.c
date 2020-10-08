#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int process_id;
    int num_processes;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    printf("Hello World from rank %d of %d processes!\n", process_id, num_processes);

    return 0;
}
